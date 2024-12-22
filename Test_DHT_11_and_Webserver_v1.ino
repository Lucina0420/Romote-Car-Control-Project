#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "DHT.h"

// DHT11 Pin and Type
#define DHTPIN 16
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
const char* ssid = "DHT11";
const char* password = "12345678";

// WebSocket server
AsyncWebServer server(80);
AsyncWebSocket ws("/data");

// HTML to display temperature and humidity
const char* htmlPage PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DHT11 Sensor Data via WebSocket (by Lucina) </title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 50px;
    }
    .data {
      font-size: 24px;
      margin: 20px 0;
    }
  </style>
</head>
<body>
  <h1>DHT11 Sensor Data via WebSocket (by Lucina) </h1>
  <div class="data" id="temperature">Temperature: -- °C</div>
  <div class="data" id="humidity">Humidity: -- %</div>
  <script>
    const ws = new WebSocket('ws://' + window.location.hostname + '/data');
    ws.onmessage = (event) => {
      const data = JSON.parse(event.data);
      document.getElementById('temperature').innerText = 'Temperature:' + data.temperature + '°C';
      document.getElementById('humidity').innerText = 'Humidity:' + data.humidity + '%';
    };
  </script>
</body>
</html>
)rawliteral";

// Send sensor data to WebSocket clients
void notifyClients() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String data = "{\"temperature\":" + String(temperature, 1) + ",\"humidity\":" + String(humidity, 1) + "}";
  ws.textAll(data);
}

// Handle WebSocket events
void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", htmlPage);
  });

  // Add WebSocket
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // Start server
  server.begin();
}

void loop() {
  notifyClients();
  delay(2000); // Update every 2 seconds
}
