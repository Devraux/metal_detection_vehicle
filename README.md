# Metal Detection Vehicle
![Visitor Badge](https://visitor-badge.laobi.icu/badge?page_id=Devraux.metal_detection_vehicle)

This project is a metal detection vehicle built using the Raspberry Pi Pico W
(the version with the CYW43439 chip).
I mainly wrote the code in C and did most of the development myself,
without relying on any pre-existing libraries.

## Applications

- **Mine detection:** Non-invasive and human-safe detection of mines.
- **Archaeological scanning:** Automated scanning of large areas for archaeological findings.

## Key Components

- **Raspberry Pi Pico 2**
- **u-blox NEO-6M(GPS module)**
- **49e sensor (Hall effect sensors)**
- **MPU6050 (Accelerometer and Gyroscope)**
- **Battery Management System (BMS)**
- **Power supply:** Lithium-ion batteries in a 2s2p configuration
- **Custom analog circuit:** A metal detection system based on a Beat Frequency Oscillator (BFO)

## Project Breakdown

The project is split into two main parts:

1. **Computer Application:** A multithreaded C application that runs on the computer.
You can find it in my other repository called - "metal_detection_app".
   
2. **Metal Detection Vehicle:** The metal detection system consists of two main 
parts: the hardware (including the vehicle’s drive system, the 
metal detection circuit, etc.) and the software (algorithms for mapping, control, 
regulation, data acquisition, metal detection, communication via Wi-Fi and UDP, etc.).

## System Features

The vehicle system includes the following functionalities:

- **Wireless communication system** (UDP protocol over Wi-Fi), implemented on a separate core to minimize latency and prevent overloading the main core.
- **Trajectory control and stabilization system** – a PID algorithm that ensures the vehicle moves straight, even on uneven surfaces.
- **Vehicle control system** – a Hall sensor-based system that enables the vehicle to turn in place, thanks to 4 independent servos.
- **Metal detection system** – a custom solution based on a Beat Frequency Oscillator (BFO) circuit for metal detection.
- **Mapping systems** – an inertial mapping system based on an MPU6050 module and a u-blox NEO-6M v2 GPS module for positioning.
- **Other smaller systems** – additional systems supporting the overall functionality.

## Metal detection vehicle - final solution
![IMG20241218144151](https://github.com/user-attachments/assets/5702dc3f-5967-4787-a29f-2595336bd6fa)

## Meatl detection vehicle user's app
You can find it in my other repository called - "metal_detection_app".
![przykład dzialania mapa](https://github.com/user-attachments/assets/01a88951-3ff1-42ea-82ec-06b63fb4c1b5)

## Metal detection circuit
![image](https://github.com/user-attachments/assets/ef177c45-a90c-4874-95d2-c3efc5f0d88f)

## Metal detection - MCU Core task distribution 
![image](https://github.com/user-attachments/assets/b5099f34-a946-463b-a2e5-895f7d775561)


