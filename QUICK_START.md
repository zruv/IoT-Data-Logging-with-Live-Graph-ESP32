# Quick Start Guide

## What Is This Project?

This is an **ESP32-based IoT Data Logger** that:
- 📊 Collects temperature and humidity data from a DHT11 sensor
- 📱 Displays readings on a local LCD screen
- 🌐 Hosts a web server with live-updating graphs
- 🔔 Alerts you with a buzzer when sensor errors occur

## 5-Minute Setup

### 1. Hardware Setup (5 minutes)
Connect your components:
```
DHT11 → GPIO 27 (data), 3.3V (power), GND
LCD   → GPIO 25 (SDA), GPIO 26 (SCL), 5V, GND
Buzzer → GPIO 14 (positive), GND (negative)
```
See [assets/circuit.png](assets/circuit.png) for diagram.

### 2. Software Setup (3 minutes)
1. Open Arduino IDE
2. Install ESP32 board support
3. Install libraries: `DHT sensor library` (Adafruit) and `LiquidCrystal_I2C`
4. Open `task2.ino`
5. **IMPORTANT**: Change these lines:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";        // Your Wi-Fi name
   const char* password = "YOUR_WIFI_PASSWORD"; // Your Wi-Fi password
   ```

### 3. Upload & Run (2 minutes)
1. Connect ESP32 via USB
2. Select your board and port in Arduino IDE
3. Click Upload
4. Open Serial Monitor (115200 baud)
5. Find the IP address displayed (e.g., `192.168.1.100`)
6. Open that IP in your browser

## What You'll See

### On the LCD:
```
Online!
192.168.1.100
```
Then every 5 seconds:
```
Temp: 25.3C
Hum:  60.5%
```

### In Your Browser:
- Live-updating line charts
- Temperature trend (red line)
- Humidity trend (blue line)
- Auto-refresh every 5 seconds

### In Serial Monitor:
```
WiFi connected
IP address: 192.168.1.100
Temperature: 25.3°C
Humidity: 60.5%
```

## Common Issues

### "Failed to read from DHT sensor!"
- Check wiring, especially GPIO 27
- Try adding a 10kΩ pull-up resistor between VCC and Data
- Check if sensor is 3.3V or 5V compatible

### LCD shows nothing
- Adjust potentiometer on LCD backpack
- Try changing address: `0x3F` → `0x27` in line 24 of code

### Can't connect to web page
- Ensure device is on same Wi-Fi network
- Check IP address in Serial Monitor
- Try pinging the IP address

## Learn More

- **[EXPLANATION.md](EXPLANATION.md)** - Deep dive into how everything works
- **[SECURITY.md](SECURITY.md)** - Important security information
- **[README.md](README.md)** - Full documentation and troubleshooting

## Quick Reference

| Component | Pin | Purpose |
|-----------|-----|---------|
| DHT11 Data | GPIO 27 | Temperature & humidity sensor |
| LCD SDA | GPIO 25 | I2C data line |
| LCD SCL | GPIO 26 | I2C clock line |
| Buzzer | GPIO 14 | Error alert |

| Endpoint | Description |
|----------|-------------|
| `http://<IP>/` | Main dashboard with graphs |
| `http://<IP>/data` | JSON API for sensor history |

## Project Status

✅ **Working and tested**  
⚠️ **Remember to update Wi-Fi credentials**  
📊 **Monitoring 2 sensor values**  
🔄 **Updates every 5 seconds**  
📈 **Stores last 20 readings**

---

**Need Help?** Check the full README.md or open an issue on GitHub.
