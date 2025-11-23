#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Airtel_Systumm HANG";
const char* password = "Bvcoe@123";

// Define the DHT sensor pin and type
#define DHTPIN 23 // GPIO23 for ESP32
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

void handleRoot() {
  // Read sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    server.send(200, "text/html", "Failed to read from DHT sensor!");
    return;
  }

  // Create the HTML for the web page
  String html = "<!DOCTYPE html><html><head><title>Sensor Readings</title><meta http-equiv='refresh' content='5'>";
  html += "<style>body { font-family: Arial, sans-serif; text-align: center; background-color: #f2f2f2; }";
  html += "h1 { color: #333; } .sensor-data { background-color: #fff; padding: 20px; margin: 20px auto; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); width: 300px; }";
  html += ".sensor-item { margin: 10px 0; } .label { font-weight: bold; } .value { color: #007BFF; }</style>";
  html += "</head><body>";
  html += "<h1>ESP32 Sensor Readings</h1>";
  html += "<div class='sensor-data'>";
  html += "<div class='sensor-item'><span class='label'>Temperature:</span> <span class='value'>" + String(temperature) + " &deg;C</span></div>";
  html += "<div class='sensor-item'><span class='label'>Humidity:</span> <span class='value'>" + String(humidity) + " %</span></div>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

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
  server.begin();
}

void loop() {
  server.handleClient();
}
