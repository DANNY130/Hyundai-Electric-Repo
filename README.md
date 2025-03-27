# Hyundai Electric Projects Repository

This repository contains multiple Arduino-based projects for various functionalities, such as GPS tracking, energy monitoring, SMS-based control, and ThingsBoard integration. Each subfolder represents a specific project, and the main folder contains a standalone project.

## Folder Structure

### 1. **Cellular_with_ThinkSpeak.ino**
- **Description**: This standalone project uses a SIM900 GSM module to control a car remotely via SMS commands. It also integrates a DS18B20 temperature sensor to send temperature readings via SMS.
- **Key Features**:
  - Remote car control (start, lock, unlock, trunk release).
  - Temperature monitoring.
  - SMS-based communication.

---

### 2. **Energy_Monitor_with_cellular**
- **Description**: This project monitors energy parameters like voltage, current, and power using the `EmonLib` library and sends data to the Blynk IoT platform via a SIM800L GSM module.
- **Files**:
  - `Energy_Monitor_with_cellular.ino`: Main Arduino sketch for energy monitoring and GSM communication.
  - `utilities.h`: Contains utility functions and hardware configurations for the SIM800L module.
- **Key Features**:
  - Energy monitoring (real power, apparent power, voltage, current).
  - Remote relay control via Blynk.
  - SMS notifications for relay status.

---

### 3. **GPS_Call_Relay_SMS**
- **Description**: This project integrates GPS tracking with SMS and call functionalities. It allows sending the GPS location via SMS and controlling a relay remotely.
- **Files**:
  - `GPS_Call_Relay_SMS.ino`: Main Arduino sketch for GPS and GSM functionalities.
  - `utilities.h`: Contains utility functions for the SIM800L module.
- **Key Features**:
  - GPS tracking using the `TinyGPS++` library.
  - SMS and call functionalities.
  - Remote relay control.

---

### 4. **GPS_Test**
- **Description**: A simple project to test GPS functionality using the `TinyGPS++` library.
- **Files**:
  - `GPS_Test.ino`: Main Arduino sketch for GPS testing.
  - `utilities.h`: Contains utility functions for power management.
- **Key Features**:
  - Displays GPS location, date, and time on the serial monitor.
  - Validates GPS module connectivity.

---

### 5. **MCCB_ON_OFF_CONTROL**
- **Description**: This project controls a relay (MCCB) remotely using the Blynk IoT platform and a SIM800L GSM module.
- **Files**:
  - `MCCB_ON_OFF_CONTROL.ino`: Main Arduino sketch for relay control.
  - `utilities1.h`: Contains utility functions for the SIM800L module.
  - `utilities.h`: Empty file (placeholder or unused).
- **Key Features**:
  - Remote relay control via Blynk.
  - SMS notifications for relay status.

---

### 6. **My_Multimeter**
- **Description**: This project acts as a digital multimeter, measuring voltage, current, and power parameters using the `EmonLib` library. It integrates with the Arduino IoT Cloud for remote monitoring.
- **Files**:
  - `My_Multimeter_oct05a.ino`: Main Arduino sketch for multimeter functionality.
  - `thingProperties.h`: Contains Arduino IoT Cloud configurations.
- **Key Features**:
  - Measures voltage, current, real power, apparent power, and power factor.
  - Sends data to the Arduino IoT Cloud.

---

### 7. **Read_SMS_ESP32**
- **Description**: This project reads SMS messages and controls a relay based on the received commands. It also provides GPS location via SMS.
- **Files**:
  - `Read_SMS_ESP32.ino`: Main Arduino sketch for SMS and GPS functionalities.
  - `utilities.h`: Contains utility functions for the SIM800L module.
- **Key Features**:
  - Reads SMS commands to control a relay.
  - Sends GPS location via SMS.
  - Handles incoming calls to toggle relay state.

---

### 8. **ThingsBoard_connection_GPS**
- **Description**: This project integrates GPS tracking with the ThingsBoard IoT platform for real-time location monitoring.
- **Files**:
  - `ThingsBoard_connection_Gps.ino`: Main Arduino sketch for GPS and ThingsBoard integration.
  - `utilities.h`: Contains utility functions for the SIM800L module.
- **Key Features**:
  - GPS tracking using the `TinyGPS++` library.
  - Sends GPS data (latitude, longitude) to ThingsBoard via MQTT.

---

## Common Files and Libraries
- **`utilities.h` and `utilities1.h`**:
  - These files contain utility functions for power management and hardware configurations for the SIM800L GSM module.
  - Functions like `setupPMU()` and `setupModem()` are used across multiple projects.

- **Libraries Used**:
  - `TinyGPS++`: For GPS data parsing.
  - `TinyGsmClient`: For GSM communication.
  - `Blynk`: For IoT platform integration.
  - `EmonLib`: For energy monitoring.
  - `ArduinoIoTCloud`: For Arduino IoT Cloud integration.

---

## How to Use
1. Open the desired project folder in your Arduino IDE.
2. Install the required libraries mentioned in the sketch.
3. Configure the hardware connections as per the pin definitions in the code.
4. Upload the sketch to your microcontroller.
5. Monitor the serial output or use the respective IoT platform for interaction.

---

## Notes
- Ensure the correct hardware model is selected in the code (e.g., SIM800L version).
- Update the GSM credentials (APN, username, password) and IoT platform tokens as required.
- Some projects require additional hardware like GPS modules, relays, or sensors.

---

## License
This repository is for educational and personal use. Please ensure proper attribution if reused or modified.
