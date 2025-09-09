#include <WiFi.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>
#include <DFRobot_MLX90614.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "VodafoneMobileWiFi-CECE77";
const char* password = "4830970720";

//ThingSpeak settings
const char* thingSpeakApiKey = "QM6FSNLXACU35YQE";
const unsigned long thingSpeakPostDelay = 15000; // 15 sec
unsigned long lastThingSpeakPost = 0;

// Pulse Sensor Setup
const int PulseWire = 34;       // PulseSensor connected to pin 34
//const int LED = 17;             // LED blinks on beat
int Threshold = 2000;           // Threshold for detection

PulseSensorPlayground pulseSensor;
DFRobot_MLX90614_I2C mlxSensor;

//LCD Setup 
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Web server
WiFiServer server(80);

// Variables
int myBPM = 0;
unsigned long lastBeatTime = 0;
bool beatDetected = false;
int signalStrength = 0;
float ambientTemp = 0;
float objectTemp = 0;
int rawPulseValue = 0;

// SETUP
void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP32 Health Monitor...");

  // Init I2C + LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IoT Health Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  // Configure Pulse Sensor
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.setSerial(Serial);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor initialized!");
  } else {
    Serial.println("PulseSensor failed!");
  }

  // Init MLX90614 Temp Sensor
  if (NO_ERR != mlxSensor.begin()) {
    Serial.println("MLX90614 failed, check wiring.");
  } else {
    Serial.println("MLX90614 initialized!");
  }

  // Wi-Fi connect
  connectToWiFi();

  // Start web server
  server.begin();
  Serial.println("Web server started");
  lcd.clear();
}

// MAIN LOOP
void loop() {
  // Reconnect WiFi if dropped
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Read raw pulse
  rawPulseValue = analogRead(PulseWire);
  signalStrength = rawPulseValue;

  if (pulseSensor.sawStartOfBeat()) {
    myBPM = pulseSensor.getBeatsPerMinute();
    lastBeatTime = millis();
    beatDetected = true;
    Serial.println("♥ HeartBeat detected!");
  }

  // Read temperature
  readTemperature();

  // Print to Serial
  displaySensorData();

  // Update LCD every 1s
  updateLCD();

  // Send to ThingSpeak
  if (millis() - lastThingSpeakPost > thingSpeakPostDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      sendToThingSpeak();
    }
    lastThingSpeakPost = millis();
  }

  // Web client
  WiFiClient client = server.available();
  if (client) {
    handleClientRequest(client);
  }

  delay(20);
}

// FUNCTIONS
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected, IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi connect failed.");
  }
}

void readTemperature() {
  static unsigned long lastTempRead = 0;
  const unsigned long interval = 1000;
  if (millis() - lastTempRead >= interval) {
    ambientTemp = mlxSensor.getAmbientTempCelsius();
    objectTemp = mlxSensor.getObjectTempCelsius();
    lastTempRead = millis();
  }
}

void displaySensorData() {
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay >= 1000) {
    Serial.println("=== SENSOR READINGS ===");
    Serial.print("BPM: "); Serial.println(myBPM);
    Serial.print("Signal: "); Serial.println(signalStrength);
    Serial.print("Object Temp: "); Serial.print(objectTemp); Serial.println("°C");
    Serial.print("Ambient Temp: "); Serial.print(ambientTemp); Serial.println("°C");
    lastDisplay = millis();
  }
}

void updateLCD() {
  static unsigned long lastLCD = 0;
  if (millis() - lastLCD >= 1000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (beatDetected && (millis() - lastBeatTime < 5000)) {
      lcd.print("Pulse: " + String(myBPM) + " bpm");
    } else {
      lcd.print("Pulse: ---");
    }
    lcd.setCursor(0, 1);
    lcd.print("Temp: " + String(objectTemp, 1) + " C");
    lastLCD = millis();
  }
}

void sendToThingSpeak() {
  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + String(thingSpeakApiKey) +
               "&field1=" + String(myBPM) +
               "&field2=" + String(signalStrength) +
               "&field3=" + String(objectTemp) +
               "&field4=" + String(ambientTemp);
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Data sent to ThingSpeak.");
  } else {
    Serial.println("ThingSpeak send failed.");
  }
  http.end();
}

void handleClientRequest(WiFiClient client) {
  String currentLine = "";
  unsigned long timeout = millis() + 250;
  while (client.connected() && millis() < timeout) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          // Send HTTP response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html><html><body>");
          client.println("<h1>ESP32 Health Monitor</h1>");
          client.println("<p>BPM: " + String(myBPM) + "</p>");
          client.println("<p>Object Temp: " + String(objectTemp) + " C</p>");
          client.println("<p>Ambient Temp: " + String(ambientTemp) + " C</p>");
          client.println("</body></html>");
          break;
        } else {
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }
  client.stop();
}

