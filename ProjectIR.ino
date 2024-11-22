#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "WPA";
const char* password = "12345678";

// IR sensor pins
const int sensor1 = 27;
const int sensor2 = 26;
const int sensor3 = 25;

// Light control pins
const int relay1 = 23;  // Controls Light1
const int relay2 = 22;  // Controls Light2
const int led3 = 21;    // Light3 controlled directly by GPIO

// States and modes
bool motion1 = false, motion2 = false, motion3 = false;
bool light1 = false, light2 = false, light3 = false; // Light states for manual mode
bool isAutomatic = true; // Mode: true = Auto, false = Manual

// Web server on port 80
WebServer server(80);

// Generate HTML for the web page
String generateHTML() {
  String html = "<!DOCTYPE html><html><head><title>Light Control</title>";
  html += "<style>body {font-family: Arial; text-align: center;}";
  html += ".btn {padding: 10px 20px; margin: 5px; font-size: 16px;}";
  html += ".on {background: yellow;} .off {background: gray;}";
  html += ".light {width: 50px; height: 50px; display: inline-block; margin: 10px;}";
  html += "</style></head><body>";
  html += "<h1>Motion-Activated Lights</h1>";
  html += "<h2>Mode: " + String(isAutomatic ? "Automatic" : "Manual") + "</h2>";
  
  html += "<button class='btn' onclick=\"location.href='/toggleMode'\">Switch to " + 
          String(isAutomatic ? "Manual" : "Automatic") + " Mode</button>";
  
  html += "<div class='light " + String(light1 ? "on" : "off") + "'></div>";
  html += "<div class='light " + String(light2 ? "on" : "off") + "'></div>";
  html += "<div class='light " + String(light3 ? "on" : "off") + "'></div>";

  if (!isAutomatic) {
    html += "<br><button class='btn' onclick=\"location.href='/toggleLight?light=1'\">Toggle Light 1</button>";
    html += "<button class='btn' onclick=\"location.href='/toggleLight?light=2'\">Toggle Light 2</button>";
    html += "<button class='btn' onclick=\"location.href='/toggleLight?light=3'\">Toggle Light 3</button>";
  }
  
  html += "</body></html>";
  return html;
}

// Handle root request
void handleRoot() {
  server.send(200, "text/html", generateHTML());
}

// Toggle between Manual and Automatic modes
void handleToggleMode() {
  isAutomatic = !isAutomatic;
  if (isAutomatic) {
    // Turn off all lights when switching back to Auto
    light1 = light2 = light3 = false;
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(led3, LOW);
  }
  handleRoot();
}

// Toggle individual light in Manual mode
void handleToggleLight() {
  if (!isAutomatic) {
    String light = server.arg("light");
    if (light == "1") light1 = !light1;
    else if (light == "2") light2 = !light2;
    else if (light == "3") light3 = !light3;

    // Update light states
    digitalWrite(relay1, light1 ? HIGH : LOW);
    digitalWrite(relay2, light2 ? HIGH : LOW);
    digitalWrite(led3, light3 ? HIGH : LOW);
  }
  handleRoot();
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set up Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/toggleMode", handleToggleMode);
  server.on("/toggleLight", handleToggleLight);
  server.begin();
  Serial.println("Web server started.");

  // Initialize pins
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Turn off all lights initially
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(led3, LOW);
}

void loop() {
  // Handle motion in Automatic mode
  if (isAutomatic) {
    motion1 = digitalRead(sensor1);
    motion2 = digitalRead(sensor2);
    motion3 = digitalRead(sensor3);

    digitalWrite(relay1, motion1 ? LOW : HIGH);
    digitalWrite(relay2, motion2 ? HIGH : LOW);
    digitalWrite(led3, motion3 ? HIGH : LOW);
  }

  // Handle web requests
  server.handleClient();
}
