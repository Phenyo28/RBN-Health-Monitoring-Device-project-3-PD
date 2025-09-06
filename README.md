# RBN-Health-Monitoring-Device-project-3-PD
---
This project is an IoT-enabled healthcare monitoring device designed to measure and record vital signs, specifically pulse rate and body temperature, in real time. Using sensors connected to an ESP32 microcontroller, the system collects health data and transmits it over Wi-Fi to a mobile application, where it is displayed for easy monitoring. The device provides a portable and affordable solution for continuous health tracking, making it useful for patients, caregivers, and healthcare providers. By enabling timely detection of irregularities and supporting remote access, the project contributes to improved preventive healthcare and has potential for future expansion with additional sensors and advanced features.

## Who is project is desiigned for?
- Patients who need continuous monitoring of vital signs.
- Caregivers and family members responsible for health supervision.
- Healthcare providers who want remote access to patient data for timely interventions.
---


## Repository Contents

---
## Features

- Sends health data wirelessly via Wi-Fi
- Measures heart rate (BPM) in real time
- Tracks body temperature with digital sensors
-  Shows live readings directly on the device
- Enables remote access to health data through a obile app and the device itself
- Designed for everyday use in healthcare or home settings
---
## Components Used
---

## Schematic Design. 
I created  the schematic design of this project using draw.io. I started by mapping out all the components, including the ESP32 microcontroller, pulse sensor, temperature sensor, switch, LCD, and a 9v battery, then arranged them to make connections. This schematic helped plan the physical wiring, guided the coding process, and ensured that the system could reliably collect and transmit data.

<img width="1117" height="620" alt="Screenshot 2025-09-06 192206" src="https://github.com/user-attachments/assets/9bfbd855-af8f-4252-8542-5281151fc399" />

## Software Requirements
-Arduino IDE (with ESP32 board support)
-Libraries:
-- WiFi.h
-- PulseSensorPlayground.h
-- OneWire.h and DallasTemperature.h (for DS18B20)
-- LiquidCrystal_I2C.h

