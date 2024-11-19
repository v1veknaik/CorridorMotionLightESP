#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "LNT";
const char* password = "123456789";

// Web server running on port 80
WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello, ESP32!");
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up web server route
  server.on("/", handleRoot);

  // Start the server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
