#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <vector>

// Replace with your network credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Define sensor and actuator pins
#define DHTPIN 27       // DHT11 data pin
#define BUZZER_PIN 14   // Buzzer pin
#define I2C_SDA 25      // I2C SDA pin
#define I2C_SCL 26      // I2C SCL pin

#define DHTTYPE DHT11

// DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// I2C LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Address 0x3F for 16x2 LCD (use 0x27 if your LCD uses that address)

WebServer server(80);

// Data history
std::vector<float> tempHistory;
std::vector<float> humHistory;
const int maxHistorySize = 20;

// Timer for reading sensor
unsigned long lastReadTime = 0;
const long readInterval = 5000; // 5 seconds

// --- Function Prototypes ---
void updateLcd(float temp, float hum);
void triggerBuzzer();

void readSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Fail");
    triggerBuzzer();
    return;
  }

  // Update LCD
  updateLcd(temperature, humidity);

  // Add to history
  tempHistory.push_back(temperature);
  humHistory.push_back(humidity);

  // Limit history size
  if (tempHistory.size() > maxHistorySize) {
    tempHistory.erase(tempHistory.begin());
  }
  if (humHistory.size() > maxHistorySize) {
    humHistory.erase(humHistory.begin());
  }
}

void updateLcd(float temp, float hum) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(temp, 1) + "C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:  " + String(hum, 1) + "%");
}

void triggerBuzzer() {
  // Turn buzzer on for a short duration
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>Sensor Readings with Chart</title>";
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f2f2f2; }";
  html += "h1 { color: #333; }";
  html += ".chart-container { width: 80%; margin: 0 auto; }";
  html += "</style></head><body>";
  html += "<h1>ESP32 Sensor Readings</h1>";
  html += "<div class='chart-container'><canvas id='sensorChart'></canvas></div>";
  html += "<script>";
  html += "const ctx = document.getElementById('sensorChart').getContext('2d');";
  html += "const sensorChart = new Chart(ctx, { type: 'line', data: { labels: [], datasets: [ { label: 'Temperature (°C)', data: [], borderColor: 'rgba(255, 99, 132, 1)', borderWidth: 1, fill: false }, { label: 'Humidity (%)', data: [], borderColor: 'rgba(54, 162, 235, 1)', borderWidth: 1, fill: false } ] }, options: { scales: { x: { type: 'linear', position: 'bottom', ticks: { stepSize: 1, callback: function(value, index, values) { return index; } } } } } });";
  html += "function updateChart() { fetch('/data').then(response => response.json()).then(data => { sensorChart.data.labels = data.labels; sensorChart.data.datasets[0].data = data.temperatures; sensorChart.data.datasets[1].data = data.humidities; sensorChart.update(); }); }";
  html += "setInterval(updateChart, 5000); window.onload = updateChart;";
  html += "</script></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{\"labels\":[";
  for (size_t i = 0; i < tempHistory.size(); ++i) {
    json += String(i);
    if (i < tempHistory.size() - 1) {
      json += ",";
    }
  }
  json += "],\"temperatures\":[";
  for (size_t i = 0; i < tempHistory.size(); ++i) {
    json += String(tempHistory[i]);
    if (i < tempHistory.size() - 1) {
      json += ",";
    }
  }
  json += "],\"humidities\":[";
  for (size_t i = 0; i < humHistory.size(); ++i) {
    json += String(humHistory[i]);
    if (i < humHistory.size() - 1) {
      json += ",";
    }
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C bus and LCD
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Booting...");

  // Initialize DHT Sensor
  dht.begin();

  // Initialize Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Online!");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastReadTime >= readInterval) {
    lastReadTime = currentTime;
    readSensorData();
  }
  server.handleClient();
}
