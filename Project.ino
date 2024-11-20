#include <WiFi.h>
#include <WebServer.h>

// PIR sensor pins
#define IR1_PIN 12
#define IR2_PIN 13
#define IR3_PIN 14

// Relay pins for two lights
#define RELAY1_PIN 27
#define RELAY2_PIN 26

// Direct GPIO pin for the third light
#define LIGHT3_PIN 15

// Wi-Fi credentials
const char* ssid = "LNT";
const char* password = "123456789";

// Web server on port 80
WebServer server(80);

// Light control states
enum LightMode { AUTO, ON, OFF };
LightMode currentMode = AUTO;

// Motion detection and light timers
bool motionDetected1 = false;
bool motionDetected2 = false;
bool motionDetected3 = false;

unsigned long lastMotionTime1 = 0;
unsigned long lastMotionTime2 = 0;
unsigned long lastMotionTime3 = 0;

const unsigned long LIGHT_ON_TIME = 5000; // Duration to keep lights on after motion

void setup() {
  Serial.begin(115200);

  // PIR sensors
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(IR3_PIN, INPUT);

  // Relay pins
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);

  // Direct pin for the third light
  pinMode(LIGHT3_PIN, OUTPUT);
  digitalWrite(LIGHT3_PIN, LOW);

  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Web server endpoints
  server.on("/", handleRoot);
  server.on("/mode/auto", setAutoMode);
  server.on("/mode/on", setOnMode);
  server.on("/mode/off", setOffMode);
  
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  if (currentMode == AUTO) {
    checkForMotion();
    manageLights();
  } else if (currentMode == ON) {
    activateLights(true); // All lights ON
  } else if (currentMode == OFF) {
    activateLights(false); // All lights OFF
  }
}

// Example: IR sensor motion detection logic
void checkForMotion() {
  if (digitalRead(IR1_PIN) == HIGH) {
    motionDetected1 = true;
    lastMotionTime1 = millis();
    Serial.println("Motion detected on IR1");
  }
  if (digitalRead(IR2_PIN) == HIGH) {
    motionDetected2 = true;
    lastMotionTime2 = millis();
    Serial.println("Motion detected on IR2");
  }
  if (digitalRead(IR3_PIN) == HIGH) {
    motionDetected3 = true;
    lastMotionTime3 = millis();
    Serial.println("Motion detected on IR3");
  }
}


// Manage each light independently based on motion timers
void manageLights() {
  if (motionDetected1 && millis() - lastMotionTime1 <= LIGHT_ON_TIME) {
    digitalWrite(RELAY1_PIN, HIGH);  // Turn on light 1
  } else {
    digitalWrite(RELAY1_PIN, LOW);   // Turn off light 1
    motionDetected1 = false;
  }

  if (motionDetected2 && millis() - lastMotionTime2 <= LIGHT_ON_TIME) {
    digitalWrite(RELAY2_PIN, HIGH);  // Turn on light 2
  } else {
    digitalWrite(RELAY2_PIN, LOW);   // Turn off light 2
    motionDetected2 = false;
  }

  if (motionDetected3 && millis() - lastMotionTime3 <= LIGHT_ON_TIME) {
    digitalWrite(LIGHT3_PIN, HIGH);  // Turn on light 3
  } else {
    digitalWrite(LIGHT3_PIN, LOW);   // Turn off light 3
    motionDetected3 = false;
  }
}

// Activate or deactivate all lights based on mode
void activateLights(bool state) {
  digitalWrite(RELAY1_PIN, state ? HIGH : LOW);
  digitalWrite(RELAY2_PIN, state ? HIGH : LOW);
  digitalWrite(LIGHT3_PIN, state ? HIGH : LOW);  // Set third light state
}

// Web server root - shows current mode
void handleRoot() {
  String html = "<h1>Light Control</h1>";
  html += "<p>Current Mode: " + getModeName() + "</p>";
  html += "<a href=\"/mode/auto\">Auto Mode</a><br>";
  html += "<a href=\"/mode/on\">Turn On</a><br>";
  html += "<a href=\"/mode/off\">Turn Off</a><br>";
  server.send(200, "text/html", html);
}

// Set mode to AUTO
void setAutoMode() {
  currentMode = AUTO;
  server.send(200, "text/plain", "Mode set to AUTO");
}

// Set mode to ON
void setOnMode() {
  currentMode = ON;
  server.send(200, "text/plain", "Mode set to ON");
}

// Set mode to OFF
void setOffMode() {
  currentMode = OFF;
  server.send(200, "text/plain", "Mode set to OFF");
}

// Helper to get the mode name
String getModeName() {
  switch (currentMode) {
    case AUTO: return "AUTO";
    case ON: return "ON";
    case OFF: return "OFF";
    default: return "UNKNOWN";
  }
}
