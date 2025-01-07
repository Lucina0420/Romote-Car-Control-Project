#include <WiFiS3.h>
#include "DHT.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "Arduino_AP";      // Access Point SSID
const char* password = "12345678";   // Access Point Password

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Start WiFi in Access Point mode
  Serial.println("Setting up Access Point...");
  if (!WiFi.beginAP(ssid, password)) {
    Serial.println("Failed to start Access Point");
    while (true);
  }
  Serial.println("Access Point started");
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.localIP()); // Get the IP address of the AP

  // Start the web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("GET /update") >= 0) {
      Serial.println("Update request received");

      // Read DHT11 data
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();

      // Check if data is valid
      if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor");
        client.println("HTTP/1.1 500 Internal Server Error");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("Failed to read from DHT sensor");
        client.stop();
        return;
      }

      // Send updated sensor data as JSON
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println();
      client.print("{\"temperature\": ");
      client.print(temperature);
      client.print(", \"humidity\": ");
      client.print(humidity);
      client.println("}");
    } else {
      // Serve the HTML page
      Serial.println("Serving HTML page");
      String htmlPage = "<!DOCTYPE html>"
                        "<html>"
                        "<head>"
                        "<title>Sensor Data</title>"
                        "<script>"
                        "function updateData() {"
                        "  fetch('/update')"
                        "    .then(response => response.json())"
                        "    .then(data => {"
                        "      document.getElementById('temp').innerText = data.temperature + ' °C';"
                        "      document.getElementById('humi').innerText = data.humidity + ' %';"
                        "    })"
                        "    .catch(error => console.error('Error fetching data:', error));"
                        "}"
                        "</script>"
                        "</head>"
                        "<body>"
                        "<h1>Temperature and Humidity</h1>"
                        "<p>Temperature: <span id='temp'>-- °C</span></p>"
                        "<p>Humidity: <span id='humi'>-- %</span></p>"
                        "<button onclick='updateData()'>Update</button>"
                        "</body>"
                        "</html>";

      // Send response to client
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println(htmlPage);
    }

    // Confirm client disconnected
    client.stop();
    Serial.println("Client disconnected");
  }
}
