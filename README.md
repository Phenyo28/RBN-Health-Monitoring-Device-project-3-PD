# 🩺RBN-Health-Monitoring-Device-project-3-PD
---
This project is an IoT-enabled healthcare monitoring device designed to measure and record vital signs, specifically pulse rate and body temperature, in real time. Using sensors connected to an ESP32 microcontroller, the system collects health data and transmits it over Wi-Fi to a mobile application, where it is displayed for easy monitoring. The device provides a portable and affordable solution for continuous health tracking, making it useful for patients, caregivers, and healthcare providers. By enabling timely detection of irregularities and supporting remote access, the project contributes to improved preventive healthcare and has potential for future expansion with additional sensors and advanced features.

## Who is project is designed for?
- Patients who need continuous monitoring of vital signs.
- Caregivers and family members responsible for health supervision.
- Healthcare providers who want remote access to patient data for timely interventions.
---


## Repository Contents
| File / Folder                 | Description                                       |
| ----------------------------- | ------------------------------------------------- |
| `Assembly 1.zip`              | Complete 3D assembly files                        |
| `Assembly STL.zip`            | STL files for 3D printing the case                |
| `Bill of material`            | List of all required electronic components        |
| `Soil Detector Schematic.pdf` | Circuit wiring diagram in PDF format              |
| `Soil Detector.png`           | Circuit schematic (image version)                 |
| `soil_detector1.ino`          | Arduino source code for running the soil detector |
 

---
## Features

- Sends health data wirelessly via Wi-Fi
- Measures heart rate (BPM) in real time
- Tracks body temperature with digital sensors
-  Shows live readings directly on the device
- Enables remote access to health data through a mobile app and the device itself
- Designed for everyday use in healthcare or home settings
---
## Components Used
| Component Name               | Purpose in Project                                            | Quantity |
| ---------------------------- | ------------------------------------------------------------- | -------- |
| `ESP32`                        | Main microcontroller; collects sensor data & controls outputs | 1        |
| `9 V Battery`                  | Powers the system                                             | 1        |
| `Pulse Sensor`                 | Measures the user’s heart rate                                | 1        |
| `IR Thermometer Sensor`        | Measures body temperature                                     | 1        |
| `Breadboard`                   | Measures body temperature                         | 1        |
| `USB A-Male to Micro USB Male` | Powers or programs the ESP32 via USB                          | 1        |
| `LCD Screen`                   | Displays real-time sensor readings                            | 1        |
| `On/Off Switch`                | Turns the device on or off                                    | 1        |

---

## Software Development
-Arduino IDE (with ESP32 board support)

-Libraries:
- WiFi.h
- PulseSensorPlayground.h
- OneWire.h
- LiquidCrystal_I2C.h
  
## How It Works
1. Sensors collect pulse rate and temperature data.
2. The ESP32 processes the readings and displays them on the LCD screen.
3. Data is transmitted over Wi-Fi to a cloud service (ThingSpeak) or mobile app.
4. Users can track vitals in real time and review historical trends.

## The Code
<img width="1640" height="970" alt="image" src="https://github.com/user-attachments/assets/91e897da-ef18-4ce5-a578-928b8809669c" />

## How the Code Works

1. Initialization (setup)
- Connects to Wi-Fi
- Initializes Pulse Sensor, Temperature Sensor, and LCD display
- Starts a web server on the ESP32
2. Loop Operations (continuous monitoring)
- Reads pulse sensor raw values and detects heartbeats
- Calculates BPM (Beats Per Minute)
- Reads object temperature (body) and ambient temperature
- Prints results to the Serial Monitor for debugging
- Updates the LCD with current BPM & body temperature
- Sends data every 15 seconds to ThingSpeak (API call via HTTP)
- Hosts a web dashboard showing real-time BPM, temperature, signal strength, and health state
3. Health Logic & Thresholds
- Pulse thresholds:
- `< 60 BPM` → Low (bradycardia risk)
- `60–100 BPM` → Normal
- `> 100 BPM` → High (tachycardia risk)
- Temperature thresholds:
- `< 36°C` → Low (hypothermia risk)
- `36–37.5°C` → Normal
- `> 37.5°C` → High (fever/infection risk)
- The system explains the possible causes of abnormal readings (e.g., stress, infection, dehydration).
4. Web Dashboard
- Auto-refreshes every 2 seconds
- Displays:
- Heartbeat animation ♥ when pulse is detected
- BPM, temperature, and signal strength
- Current health state messages (normal, high, low)
- ThingSpeak connection status & next update timer

---

## Schematic Design. 
I created  the schematic design of this project using draw.io. I started by mapping out all the components, including the ESP32 microcontroller, pulse sensor, temperature sensor, switch, LCD, and a 9v battery, then arranged them to make connections. This schematic helped plan the physical wiring, guided the coding process, and ensured that the system could reliably collect and transmit data.

<img width="1117" height="620" alt="Screenshot 2025-09-06 192206" src="https://github.com/user-attachments/assets/9bfbd855-af8f-4252-8542-5281151fc399" />


## App Development
I sed the MIT App inventor to create my app. The pulse rate and body temperature readings from the ESP32 are displayed on the LCD screen of the device and in the mobile app for real-time monitoring. The same data is also sent to ThingSpeak, where it is visualized in real-time graphs, allowing users to track trends over time and monitor their health data effectively both locally and via the cloud.

<img width="655" height="816" alt="Screenshot 2025-09-11 001952" src="https://github.com/user-attachments/assets/5069208c-3399-4872-8972-acca0efbef1c" /> <img width="655" height="816" alt="Screenshot 2025-09-11 001952" src="https://github.com/user-attachments/assets/544e0dd1-4572-4b86-ad80-ec90e4fe2d43" />

---

## How the ThingSpeak Readings Work
The ESP32 collects pulse and temperature values from the sensors and connects to Wi-Fi. These readings are then sent to ThingSpeak, an IoT analytics platform, using its API. Each data value is uploaded to a specific channel field (for example, Field 1 = Pulse Rate, Field 2 = Signal Strength, Field 3 = Object Temperature, Field 4 = Ambient Temperature). ThingSpeak stores the data in the cloud and displays it in real-time through interactive graphs and charts. This allows continuous monitoring of trends over time, remote access to health data, and easy integration with data analysis or alert systems.
<img width="1298" height="806" alt="Screenshot 2025-09-09 091009" src="https://github.com/user-attachments/assets/3277fdbe-5d2a-492c-a488-6d55d139bcd5" />

---
## Process of working on the Project

## Soldering:
![IMG-20250901-WA0003](https://github.com/user-attachments/assets/e612b328-15d6-4712-b2cf-38692746efca)

---
## Component Testing:
---
![IMG-20250905-WA0006](https://github.com/user-attachments/assets/38d5bf0b-ecc3-4149-ab6e-30b127555615) ![IMG-20250905-WA0009](https://github.com/user-attachments/assets/ad44780d-0bfd-4865-8fd2-258017accac9)


## Challenges I faced when building this project
During this project, I faced several challenges. The LCD screen was not working properly, it would turn on and off (and not work for a long time). Some components were overheating and some would not work when I connected them to my breadboard. The serial monitor failed to recognize certain components sometimes, to solve this I used an I2C scanner to correctly identify and connect the devices, which helped stabilize the system. The readings of my components were displaying slow. My ESP32 microcontroller burnt and was not responsive, this delayed the process as I had to source a replacement device before I could continue testing and integrating the components.

## Future Improvements
For future improvements, I will add SpO₂ (oxygen saturation) and blood pressure monitoring to provide more comprehensive health tracking. I will also improve my mobile app to send push alerts for abnormal readings and implement AI-based data analysis to offer predictive health insights. I will expand cloud support beyond ThingSpeak to platforms like Firebase or AWS IoT, enabling more data storage, accessibility, and advanced analytics.
