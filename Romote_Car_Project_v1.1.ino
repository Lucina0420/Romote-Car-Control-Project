#include <WiFi.h>
#include <WebServer.h>

// Set WiFi credentials for access point
const char *ssid = "MyWiFiCar";
const char *password = "12345678";

// Initialize the web server on port 80
WebServer server(80);

// HTML content to display on the webpage
const char* htmlContent = R"HTML(
<!DOCTYPE html>
<html>
  <head>
    <title>Remote Car</title>
  </head>
  <body>
    <h1 style="text-align: center; font-family: Arial, sans-serif;">Hello! From My Remote Car!</h1>
  </body>
</html>
)HTML";

// Function to handle the root URL
void handleRoot() {
  server.send(200, "text/html", htmlContent); // Send the HTML content
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Configure the ESP32 enable WiFi
  WiFi.softAP(ssid, password);
  Serial.println("Access point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP()); // Display the IP address

  // Define the HTTP route
  server.on("/", handleRoot);

  // Start the web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}