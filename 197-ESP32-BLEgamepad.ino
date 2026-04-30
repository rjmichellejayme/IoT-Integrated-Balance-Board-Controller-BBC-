#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <BLE2902.h>
#include <MPU6050.h>

MPU6050 mpu;
BLEHIDDevice* hid;
BLECharacteristic* input;
BLEServer* server;

uint8_t joyReport[2];
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Calibration offsets
int16_t offsetX = 0;
int16_t offsetY = 0;

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device Connected");
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device Disconnected");
  }
};

void calibrateMPU() {
  Serial.println("========================================");
  Serial.println("CALIBRATION STARTING");
  Serial.println("Place device on flat surface...");
  Serial.println("========================================");
  
  // Wait 3 seconds for user to place device
  for (int i = 3; i > 0; i--) {
    Serial.print("Starting in ");
    Serial.print(i);
    Serial.println("...");
    delay(1000);
  }
  
  Serial.println("Calibrating... (reading 100 samples)");
  
  long sumX = 0;
  long sumY = 0;
  const int samples = 100;
  
  // Take 100 readings
  for (int i = 0; i < samples; i++) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    
    sumX += ax;
    sumY += ay;
    
    // Progress indicator
    if (i % 10 == 0) {
      Serial.print(".");
    }
    
    delay(10);
  }
  
  Serial.println();
  
  // Calculate average (center position)
  offsetX = sumX / samples;
  offsetY = sumY / samples;
  
  Serial.println("========================================");
  Serial.println("CALIBRATION COMPLETE!");
  Serial.print("X Offset: ");
  Serial.println(offsetX);
  Serial.print("Y Offset: ");
  Serial.println(offsetY);
  Serial.println("========================================");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Give time for Serial Monitor to open
  
  Wire.begin(8, 9);

  // MPU6050 init
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 failed");
    while (1);
  }
  Serial.println("MPU6050 OK");
  
  // === CALIBRATION ROUTINE ===
  calibrateMPU();
  
  // BLE Init
  BLEDevice::init("ESP32 Gamepad");
  
  server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
  
  hid = new BLEHIDDevice(server);
  input = hid->inputReport(1);
  
  hid->manufacturer()->setValue("Espressif");
  hid->pnp(0x02, 0x05ac, 0x820a, 0x0210);
  hid->hidInfo(0x00, 0x02);
  
  const uint8_t reportMap[] = {
    0x05, 0x01, 0x09, 0x05, 0xA1, 0x01, 0x85, 0x01,
    0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x15, 0x81,
    0x25, 0x7F, 0x75, 0x08, 0x95, 0x02, 0x81, 0x02, 0xC0
  };

  hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
  hid->startServices();
  hid->setBatteryLevel(100);

  BLESecurity* security = new BLESecurity();
  security->setAuthenticationMode(ESP_LE_AUTH_BOND);

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->setAppearance(0x03C4);
  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);
  
  BLEDevice::startAdvertising();
  
  Serial.println("BLE HID Gamepad Ready");
  Serial.println("Waiting for connection...");
}

void loop() {
  // Handle reconnection
  if (!deviceConnected && oldDeviceConnected) {
    Serial.println("Restarting advertising...");
    delay(500);
    server->startAdvertising();
    oldDeviceConnected = deviceConnected;
  }
  
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    Serial.println("Gamepad connected!");
  }

  if (deviceConnected) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    // Apply calibration offsets
    ax -= offsetX;
    ay -= offsetY;

    // Map to joystick range
    int8_t x = map(ax, -17000, 17000, -127, 127);
    int8_t y = map(ay, -17000, 17000, -127, 127);
    
    joyReport[0] = -x;
    joyReport[1] = y;

    input->setValue(joyReport, 2);
    input->notify();

    delay(20);
  } else {
    delay(100);
  }
}