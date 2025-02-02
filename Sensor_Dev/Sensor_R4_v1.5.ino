#include <WiFiS3.h> // Library for Wi-Fi support
#include "DHT.h"

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "Luc2007";        // Replace with your Wi-Fi network name
const char* password = "KaNaTa-2022"; // Replace with your Wi-Fi password

const char* scriptURL = "https://script.google.com/macros/s/AKfycbxjJEQd_Z4vDUzGa-82PrZ8QMNJuK0nl7fF0z8LMoqO2_H_2dxFOqFuVAu04WDA-awa/exec";

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
  // Read DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  // Send data to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    
    if (client.connect(scriptURL, 80)) {
      // Create the GET request URL with query parameters
      String url = String(scriptURL) + "?temperature=" + String(temperature) + "&humidity=" + String(humidity);

      // Send the GET request
      client.print("GET " + url + " HTTP/1.1\r\n");
      client.print("Host: script.google.com\r\n");
      client.print("Connection: close\r\n\r\n");

      // Wait for the server to respond
      delay(1000);
      
      // Read the response
      String response = "";
      while (client.available()) {
        response = client.readString();
      }

      // Print the response
      Serial.println("Response: " + response);
    } else {
      Serial.println("Connection failed.");
    }
    
    client.stop(); // Close the connection
  } else {
    Serial.println("Wi-Fi disconnected");
  }

  delay(60000); // Wait 1 minute before sending data again
}
