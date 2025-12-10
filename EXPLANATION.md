# What's Happening Here - Project Explanation

## Overview

This repository contains an **IoT Data Logging System** that uses an **ESP32 microcontroller** to collect environmental data from sensors and display it both locally (on an LCD) and remotely (via a web interface with live graphs).

## Hardware Components

The system uses the following hardware:

1. **ESP32 Development Board** - The main microcontroller that runs the code
2. **DHT11 Sensor** - Measures temperature and humidity
3. **16x2 I2C LCD Display** - Shows sensor readings locally
4. **Buzzer** - Provides audible alerts when sensor errors occur

## Pin Configuration

```
DHT11 Sensor:
- VCC → ESP32 3.3V
- GND → ESP32 GND
- Data → GPIO 27

I2C LCD Display:
- VCC → ESP32 5V/Vin
- GND → ESP32 GND
- SDA → GPIO 25
- SCL → GPIO 26

Buzzer:
- Positive (+) → GPIO 14
- Negative (-) → ESP32 GND
```

## How the System Works

### 1. **System Initialization (setup())**
   - Connects to Wi-Fi network using provided credentials
   - Initializes the I2C LCD display
   - Starts the DHT11 sensor
   - Sets up the buzzer pin
   - Creates a web server on port 80
   - Displays "Online!" and IP address on LCD

### 2. **Continuous Monitoring (loop())**
   - Reads sensor data every 5 seconds (non-blocking timer)
   - Handles incoming web requests from clients
   - Updates sensor data history

### 3. **Sensor Data Collection (readSensorData())**
   - Reads temperature and humidity from DHT11
   - If sensor fails, displays error on LCD and triggers buzzer
   - If successful, updates LCD with current readings
   - Stores data in history vectors (max 20 readings)

### 4. **Local Display (updateLcd())**
   - Shows current temperature in Celsius
   - Shows current humidity percentage
   - Updates every 5 seconds

### 5. **Web Interface**
   - **Root endpoint (/)**: Serves an HTML page with live-updating charts
   - **Data endpoint (/data)**: Provides JSON data with sensor history
   - Uses Chart.js library for visualization
   - Auto-refreshes every 5 seconds

## Code Architecture

```
Main Components:
├── WiFi Connection
├── Web Server (Port 80)
│   ├── GET / → HTML page with chart
│   └── GET /data → JSON sensor data
├── DHT11 Sensor Interface
├── I2C LCD Display
├── Buzzer Alert System
└── Data Storage (Vectors for history)
```

## Data Flow

```
DHT11 Sensor → ESP32 → {
    ├→ LCD Display (local)
    ├→ Serial Monitor (debugging)
    └→ Web Server → {
        ├→ Browser (chart visualization)
        └→ Data History (JSON API)
    }
}
```

## Current Issues Identified

### 1. **Hardcoded Wi-Fi Credentials (Security Risk)**
   - **Location**: Lines 9-10 in task2.ino
   - **Issue**: Wi-Fi SSID "Airtel_Systumm HANG" and password "Bvcoe@123" are hardcoded in the code
   - **Risk**: Credentials are exposed in the repository (public security risk)
   - **Recommendation**: Move credentials to a separate config file or use environment variables

### 2. **I2C LCD Address Mismatch Comment**
   - **Location**: Line 24 in task2.ino
   - **Issue**: Code uses address `0x3F` but comment says `0x27`
   - **Code**: `LiquidCrystal_I2C lcd(0x3F, 16, 2); // Address 0x27 for 16x2 LCD`
   - **Impact**: Misleading comment (minor issue)
   - **Recommendation**: Update comment to match actual address

### 3. **Limited Error Handling**
   - **Issue**: Only basic sensor error handling exists
   - **Missing**: No Wi-Fi reconnection logic if connection drops
   - **Missing**: No web server error handling
   - **Recommendation**: Add reconnection logic and error recovery

### 4. **Blocking Delay in Buzzer Function**
   - **Location**: Line 82 in task2.ino
   - **Issue**: Uses `delay(100)` which blocks the entire program
   - **Impact**: Web server cannot handle requests during buzzer activation
   - **Recommendation**: Use non-blocking timing or reduce delay

### 5. **Memory Management**
   - **Issue**: Vectors grow indefinitely until maxHistorySize
   - **Current**: Limited to 20 readings (good)
   - **Consideration**: On ESP32, this is acceptable but could be optimized

## Features

✅ **Real-time monitoring** - Updates every 5 seconds
✅ **Local display** - Shows readings on LCD
✅ **Remote access** - View data from any device on same network
✅ **Live graphs** - Visual representation of temperature and humidity trends
✅ **History tracking** - Stores last 20 readings
✅ **Error alerts** - Buzzer notification for sensor failures
✅ **Responsive design** - Clean web interface

## How to Use

1. **Upload the code**: Flash task2.ino to ESP32 using Arduino IDE
2. **Check Serial Monitor**: View connection status and IP address (115200 baud)
3. **Read LCD**: See IP address displayed locally
4. **Open browser**: Navigate to ESP32's IP address (e.g., http://192.168.1.x)
5. **Monitor data**: Watch live-updating temperature and humidity graphs

## Technologies Used

- **Arduino Framework** for ESP32
- **C++** programming language
- **Chart.js** for data visualization
- **WebServer** library for HTTP server
- **DHT sensor library** by Adafruit
- **LiquidCrystal_I2C** for LCD control
- **Wire** library for I2C communication

## Project Status

✅ **Functional**: The code is complete and working
⚠️ **Security Issue**: Wi-Fi credentials exposed
⚠️ **Minor Bug**: Comment mismatch on LCD address
✅ **Documentation**: Comprehensive README available
✅ **Hardware Diagrams**: Circuit diagrams in assets folder

## Recommendations for Improvement

1. **Security**: Remove hardcoded credentials immediately
2. **Robustness**: Add Wi-Fi reconnection logic
3. **Performance**: Use non-blocking buzzer alerts
4. **Features**: Add data export functionality
5. **UI**: Add threshold alerts on web interface
6. **Storage**: Consider using SPIFFS for persistent data storage
7. **API**: Add RESTful endpoints for better integration
8. **Monitoring**: Add system health metrics (uptime, signal strength)

## Summary

This is a **well-structured IoT project** that demonstrates:
- Sensor integration with ESP32
- Local display using I2C LCD
- Web server hosting on microcontroller
- Real-time data visualization
- Multi-component hardware integration

The main concern is the **exposed Wi-Fi credentials** which should be addressed immediately for security reasons.
