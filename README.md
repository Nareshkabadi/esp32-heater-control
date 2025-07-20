
# ESP32 Heater Control System
This project implements a temperature-based heater control system using an ESP32 microcontroller and a DHT22 sensor.
It uses FreeRTOS for multitasking and Serial communication for real-time logging and debugging.

## Features
### 1. Temperature Monitoring
+ Reads temperature continuously from a DHT22 sensor.
+ Uses temperature data to determine the heater's operational state.

### 2. Temperature-Based State Machine
The system operates with five states:
+ IDLE: Temperature below 25°C (heater OFF).
+ HEATING: Temperature between 25°C and 34.9°C (heater ON).
+ STABILIZING: Temperature between 35°C and 44.9°C (heater ON, close to target).
+ TARGET_REACHED: Temperature between 45°C and 54.9°C (heater ON, maintaining target).
+ OVERHEAT: Temperature 55°C or above (heater OFF, buzzer alarm ON).

### 3. Heater Control
+ Simulated heater control (LED indicator) based on current system state.
+ Buzzer alarm activates during the OVERHEAT state.

### 4. Visual & Audible Feedback
+ LED (GPIO 2): Turns ON during heating or target reached states.
+ Buzzer (GPIO 4): Alerts when temperature exceeds the safety threshold (55°C).

### 5. Serial Logging
+ Outputs temperature readings and system states to the Serial Monitor for real-time monitoring.

### 6. FreeRTOS Integration
+ tempTask: Reads temperature every 2 seconds.
+ controlTask: Manages LED & buzzer every 0.5 seconds.

## Hardware Requirements
+ ESP32 Development Board
+ DHT22 Temperature Sensor
+ 1 LED (heater indicator)
+ 1 Buzzer

## Pin Configuration
+ DHTPIN: GPIO 15 (DHT22 Data)
+ LED_PIN: GPIO 2 (Heater Indicator)
+ BUZZER_PIN: GPIO 4 (Buzzer Alarm)

## How to Use (Simulation)
1. Open the Wokwi Simulation
2. Click Play in Wokwi to run the system.
3. Open the Serial Monitor to see real-time temperature and state logs.
4. Adjust the DHT22 temperature slider to test different states and observe LED and buzzer behavior.

## Development Environment
+ To upload the code to an ESP32 board:
+ Add DHT sensor library (via Library Manager).

## Future Enhancements
+ Add Relay Control: For switching real heating elements.
+ Wi-Fi Integration: Monitor & control via mobile/web app (MQTT or HTTP).
+ Multiple Heating Profiles: (Low/Medium/High).
+ Scheduled Heating: Add RTC or NTP-based time scheduling.
+ PID Control: Smooth & precise temperature control.
+ OLED/LCD Display: For real-time temperature display.

## Wokwi Simulation Link
[Click Here to View the Simulation](https://wokwi.com/projects/437001551170110465)
