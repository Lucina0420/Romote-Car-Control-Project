#include <WiFiS3.h> // Library for Wi-Fi support
#include "DHT.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "Luc2007";        // Replace with your Wi-Fi network name
const char* password = "KaNaTa-2022"; // Replace with your Wi-Fi password

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Connect to Wi-Fi as a client
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

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
      // String htmlPage = "<!DOCTYPE html>"
      //                   "<html>"
      //                   "<head>"
      //                   "<title>Sensor Data</title>"
      //                   "<script>"
      //                   "function updateData() {"
      //                   "  fetch('/update')"
      //                   "    .then(response => response.json())"
      //                   "    .then(data => {"
      //                   "      document.getElementById('temp').innerText = data.temperature + ' °C';"
      //                   "      document.getElementById('humi').innerText = data.humidity + ' %';"
      //                   "    })"
      //                   "    .catch(error => console.error('Error fetching data:', error));"
      //                   "} "
      //                   "</script>"
      //                   "</head>"
      //                   "<body>"
      //                   "<h1>Temperature and Humidity</h1>"
      //                   "<p>Temperature: <span id='temp'>-- °C</span></p>"
      //                   "<p>Humidity: <span id='humi'>-- %</span></p>"
      //                   "<button onclick='updateData()'>Update</button>"
      //                   "</body>"
      //                   "</html>";


      String htmlPage = "<!DOCTYPE html>"
                        "<html>"
                        "<head>"
                        "<title>Sensor Data</title>"
                        "<style>"
                        "* { margin: 0; padding: 0; box-sizing: border-box; }"
                        "p { margin-top: 2em; text-align: left; margin-left: 2em; font-size: 30px; }"
                        "h1 { text-align: center; font-size: 3.5em; color: white; background-color: #333; }"
                        ".main-nav { background-color: #333; padding: 20px 0; }"
                        ".navbar-nav { list-style-type: none; display: flex; justify-content: center; margin: 0; }"
                        ".navbar-nav li { margin: 0 15px; }"
                        ".navbar-nav a { text-decoration: none; color: white; padding: 10px 20px; font-size: 1.2em; border-radius: 5px; transition: background-color 0.3s ease; }"
                        ".navbar-nav a:hover { background-color: #575757; }"
                        ".navbar-nav .active { background-color: #04AA6D; }"
                        ".large-text {font-size: 2em; text-align: center; margin-top: 10px;}"
                        "</style>"
                        "<script>"
                        "function updateData() {"
                        "  fetch('/update')"
                        "    .then(response => response.json())"
                        "    .then(data => {"
                        "      document.getElementById('temp').innerText = data.temperature + 'C';"
                        "      document.getElementById('humi').innerText = data.humidity + '%';"
                        "    })"
                        "    .catch(error => console.error('Error fetching data:', error));"
                        "} "
                        "</script>"
                        "</head>"
                        "<body>"
                        "<header>"
                        "<h1>Attic Robot</h1>"
                        "</header>"
                        "<nav class='main-nav'>"
                        "<ul class='navbar-nav'>"
                        "<li><a>Homepage</a></li>"
                        "<li><a href='features.html'>Features</a></li>"
                        "<li><a>Remote Car Control</a></li>"
                        "<li><a class='active'>Sensors</a></li>"
                        "<li><a href='process.html'>Design Process</a></li>"
                        "<li><a href='gallery.html'>Gallery</a></li>"
                        "<li><a href='improvements.html'>Future Improvements</a></li>"
                        "<li><a href='contact.html'>Contact Page</a></li>"
                        "</ul>"
                        "</nav>"
                        "<p class='large-text'>Temperature and Humidity</p>"
                        // "<p>Temperature: <span id='temp'>-- °C</span></p>"
                        "<p>Temperature: <span id='temp'>--</span></p>"
                        // "<p>Humidity: <span id='humi'>-- %</span></p>"
                        "<p>Humidity: <span id='humi'>--</span></p>"
                        // "<button onclick='updateData()'>Update</button>"
                        "<button onclick='updateData()' style='font-size: 1.5em; padding: 15px 30px; margin-top: 20px; vertical-align: top; margin-right: 30px; display: inline-block;'>Update</button>"
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
