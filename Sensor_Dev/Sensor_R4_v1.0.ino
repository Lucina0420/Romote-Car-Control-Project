#include "esp_camera.h"
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>
#include <DHT.h>  // Include the DHT library

#define DHTPIN 16       // Pin where DHT11 is connected
#define DHTTYPE DHT11   // Define DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

// Your existing code for pins, motors, camera setup, etc.

void setup() {
  // Initialize DHT sensor
  dht.begin();

  // Initialize Serial for printing
  Serial.begin(115200);

  // Other setup code...
}

void readDHT11() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check if any reading failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Print the temperature and humidity values to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void loop() {
  // Read DHT11 every 5 seconds
  readDHT11();
  delay(5000); 
}
