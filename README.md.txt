# Sarsabz Zameen - Smart Agriculture Project
**Team**: Easha Zeb Satti, Ayesha Asif, Rida Kazmi  
**Description**: IoT-based system for automatic/manual watering and future crop/fertilizer recommendations.

## Features
- **Auto-Watering**: Pump triggers based on soil moisture.
- **Manual Control**: App toggles pump via Firebase.
- **In Progress**: Crop and fertilizer recommendations.

## Hardware
- ESP8266 NodeMCU
- Soil Moisture Sensor
- DHT11 (Temp/Humidity)
- 5V Relay + Pump
- [Circuit.png](Circuit.png)

## Software
- **IDE**: Arduino IDE (C++)
- **Libraries**: Firebase_ESP_Client, DHT
- **Firebase**: /sensors/temperature, /sensors/humidity, /pump

## Setup
1. Upload `SarsabzZameen.ino` to ESP8266.
2. Set Wi-Fi and Firebase credentials (redacted).
3. Connect as per `Circuit.png`.

## Status
- **Done**: Auto/manual watering, Firebase integration.
- **Next**: Crop logic (e.g., wheat if moisture < 30%), fertilizer suggestions.

## Files
- `SarsabzZameen.ino`: Main code
- `Circuit.png`: Diagram
- `Firebase_Structure.png`: DB snapshot
- `SarsabzZameen_Report.docx`: Full report