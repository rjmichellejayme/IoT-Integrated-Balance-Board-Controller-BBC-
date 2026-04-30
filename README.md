# IoT-Integrated-Balance-Board-Controller-BBC

## Overview
The IoT-Integrated Balance Board Controller (BBC) is a hardware-software system that transforms physical balance movements into digital gamepad inputs. Built with an MPU6050 motion sensor, ESP32-C3 microcontroller, and a 3.7V Li-Po battery, the controller leverages Bluetooth Low Energy (BLE) to provide immersive gameplay while promoting physical activity and rehabilitation.

This project bridges entertainment and health, offering a portable, low-cost, and energy-efficient solution for interactive gaming and fitness applications.

---

## Features
- **Sensor-Based Motion Tracking**  
  Captures tilt, orientation, and balance shifts using the MPU6050 accelerometer and gyroscope.

- **BLE Gamepad Emulation**  
  Transmits motion data wirelessly via BLE, recognized as a standard HID gamepad by PCs, smartphones, and consoles.

- **Health-Oriented Gameplay**  
  Encourages core muscle engagement, posture improvement, and rehabilitation exercises while gaming.

- **Power Management System**  
  Operates on a 3.7V Li-Po battery with optimized firmware for low-power consumption and safe energy use.

- **User Interaction & Accessibility**  
  Body movements replace traditional button presses, enhancing immersion and accessibility.

---

## Hardware Components
- ESP32-C3 – Microcontroller for sensor data processing and BLE communication  
- MPU6050 – 6-axis motion sensor (accelerometer + gyroscope)  
- 3.7V 150mAh Li-Po Battery – Portable power supply  
- Manual Switch – Regulates energy flow for safe operation  

---

## Software Components
- **Firmware**: Developed using Arduino IDE with libraries:  
  `<Wire.h>`, `<BLEDevice.h>`, `<BLEServer.h>`, `<BLEHIDDevice.h>`, `<BLE2902.h>`, `<MPU6050.h>`  

- **Game Demo**: Browser-based 2D flight simulator (HTML + JavaScript)  
  - BLE gamepad input for aircraft pitch/roll  
  - Collision detection and scoring system  
  - Local leaderboard stored in JSON  

---

## System Requirements
- BLE-compatible host device (PC, smartphone, or gaming console)  
- Stable indoor flooring for safe balance training  
- Browser supporting HTML5, JavaScript, and gamepad APIs (for demo game)  

---

## Setup & Usage
1. **Power On** the balance board using the manual switch.  
2. **Pair via Bluetooth** with a BLE-enabled host device.  
3. **Calibration**: Place the board on a flat surface; firmware collects baseline offsets.  
4. **Play**: Use tilt and orientation to control in-game actions.  
5. **Monitor Battery**: BLE HID profile reports battery status to host device.  

---

## Assumptions & Dependencies
- Users have basic familiarity with BLE pairing and gamepad controllers.  
- MPU6050 sensor must be calibrated for accurate motion detection.  
- ESP32-C3 firmware must include BLE HID libraries.  
- Battery limitations support short gaming sessions; future iterations may extend runtime.  

---

## References
- InvenSense. (2023). MPU-6050 | TDK. [Link](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)  
- Wang, Z. et al. (2025). *The effects of balance training on physical fitness and skill-related performance in basketball players: a systematic review.* BMC Sports Science, Medicine & Rehabilitation, 17(1), 108. [DOI](https://doi.org/10.1186/s13102-025-01164-9)  
