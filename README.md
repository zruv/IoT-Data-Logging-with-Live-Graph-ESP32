# Task 2: IoT Data Logging (ESP32)

This document guides you through setting up a local web server on an ESP32 to display sensor data.

## Goal

Send sensor readings to a local web dashboard.

## Requirements

*   ESP32 Development Board
*   Any one sensor (in this guide, we will use a DHT11 sensor)
*   Arduino IDE with ESP32 board support

## Steps

1.  **Set up the Arduino IDE:**
    *   Install the ESP32 board package:
        1.  Go to `File > Preferences`.
        2.  Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to the "Additional Boards Manager URLs".
        3.  Go to `Tools > Board > Boards Manager`.
        4.  Search for `esp32` and install the package.
        5.  Select your ESP32 board from `Tools > Board`.
    *   Install the DHT sensor library:
        1.  Go to `Sketch > Include Library > Manage Libraries`.
        2.  Search for `DHT sensor library` by Adafruit and install it.

2.  **Connect the hardware:**
    *   Connect the DHT11 sensor to the ESP32 as follows:
        *   Sensor VCC to ESP32 3V3
        *   Sensor GND to ESP32 GND
        *   Sensor Data to ESP32 GPIO23 (or any suitable digital pin)

3.  **Write the code:**
    *   Open the `task2.ino` file in the Arduino IDE.
    *   Update the `ssid` and `password` variables with your Wi-Fi credentials.
    *   Update the `DHTPIN` define to match the GPIO pin you chose (e.g., `#define DHTPIN 23`).
    *   Upload the code to your ESP32.

4.  **Test the setup:**
    *   Open the Serial Monitor to see the IP address of the ESP32.
    *   Open a web browser and enter the IP address to see the sensor readings.

## Code Explanation

The `task2.ino` sketch does the following:

*   Includes the necessary libraries: `WiFi.h` for Wi-Fi, `WebServer.h` for the web server, and `DHT.h` for the sensor.
*   Defines the Wi-Fi credentials and the DHT sensor pin.
*   Initializes the DHT sensor and the web server.
*   In the `setup()` function, it connects to Wi-Fi and starts the web server.
*   The `handleRoot()` function is called when a request is made to the root URL. It reads the sensor data, creates an HTML page with the data, and sends it to the client. The HTML includes a meta tag that automatically refreshes the page every 5 seconds.
*   The `loop()` function continuously handles client requests.

## Deliverables

1.  Code (Arduino sketch)
2.  Screenshot of the live data (local web page)
3.  1-2 minute video explaining the working setup
4.  Well-documented `README.md` report (this file)
