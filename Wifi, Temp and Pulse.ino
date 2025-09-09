#include <WiFi.h>
#include <HTTPClient.h>
#include <PulseSensorPlayground.h>
#include <DFRobot_MLX90614.h>
#include <Wire.h>

// WiFi credentials
const char* ssid = "VodafoneMobileWiFi-CECE77";
const char* password = "4830970720";

// ThingSpeak settings
const char* thingSpeakApiKey = "QM6FSNLXACU35YQE";  // Replace with your ThingSpeak API Key
const unsigned long thingSpeakPostDelay = 400; // Post to ThingSpeak every 15 seconds
unsigned long lastThingSpeakPost = 0;

// Pulse Sensor Setup
const int PulseWire = 34;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = 17;             // The on-board Arduino LED, close to PIN 13.
int Threshold = 2000;           // Increased threshold for better detection

PulseSensorPlayground pulseSensor;  // Pulse sensor object
DFRobot_MLX90614_I2C mlxSensor;     // Temperature sensor object

// Web server on port 80
WiFiServer server(80);

// Variables for sensor data
int myBPM = 0;
unsigned long lastBeatTime = 0;
bool beatDetected = false;
int signalStrength = 0;
float ambientTemp = 0;
float objectTemp = 0;
int rawPulseValue = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing ESP32 Health Monitor...");

  // Initialize I2C communication for temperature sensor
  Wire.begin();
  
  // Configure the PulseSensor object
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);
  pulseSensor.setThreshold(Threshold);   

  // Initialize pulse sensor with more settings
  pulseSensor.setSerial(Serial);
  
  // Initialize pulse sensor
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created successfully!");
  } else {
    Serial.println("PulseSensor object creation failed!");
  }

  // Initialize temperature sensor
  Serial.println("Initializing MLX90614 temperature sensor...");
  if (NO_ERR != mlxSensor.begin()) {
    Serial.println("Communication with MLX90614 failed, please check connection");
    // Don't halt, continue without temperature sensor
  } else {
    Serial.println("MLX90614 initialized successfully!");
  }

  // Connect to Wi-Fi
  connectToWiFi();

  // Start web server
  server.begin();
  Serial.println("Web server started");
  Serial.println("==========================================");
}

void loop() {
  // Check WiFi connection and reconnect if needed
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    connectToWiFi();
  }
  
  // Read raw pulse sensor value
  rawPulseValue = analogRead(PulseWire);
  signalStrength = rawPulseValue;
  
  // Handle pulse sensor data - using a simpler approach
  if (pulseSensor.sawStartOfBeat()) {
    myBPM = pulseSensor.getBeatsPerMinute();
    lastBeatTime = millis();
    beatDetected = true;
    
    Serial.println("♥ A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(myBPM);
    Serial.print("Signal: ");
    Serial.println(signalStrength);
  }

  // Read temperature data
  readTemperature();

  // Display all sensor data on Serial Monitor
  displaySensorData();

  // Send data to ThingSpeak periodically
  if (millis() - lastThingSpeakPost > thingSpeakPostDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      sendToThingSpeak();
    }
    lastThingSpeakPost = millis();
  }

  // Handle client requests
  WiFiClient client = server.available();
  if (client) {
    handleClientRequest(client);
  }

  delay(20); // Small delay for stability
}

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
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi. Please check credentials.");
  }
}

void readTemperature() {
  static unsigned long lastTempRead = 0;
  const unsigned long tempReadInterval = 1000; // Read temperature every second
  
  if (millis() - lastTempRead >= tempReadInterval) {
    ambientTemp = mlxSensor.getAmbientTempCelsius();
    objectTemp = mlxSensor.getObjectTempCelsius();
    lastTempRead = millis();
  }
}

void displaySensorData() {
  static unsigned long lastDisplay = 0;
  const unsigned long displayInterval = 1000; // Update display every second
  
  if (millis() - lastDisplay >= displayInterval) {
    Serial.println("=== SENSOR READINGS ===");
    Serial.print("Pulse Signal: ");
    Serial.println(signalStrength);
    
    if (beatDetected) {
      Serial.print("Heart Rate: ");
      Serial.print(myBPM);
      Serial.println(" BPM");
    } else {
      Serial.println("Heart Rate: Waiting for pulse...");
    }
    
    Serial.print("Object Temp: ");
    Serial.print(objectTemp);
    Serial.println(" °C");
    
    Serial.print("Ambient Temp: ");
    Serial.print(ambientTemp);
    Serial.println(" °C");
    
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    Serial.println("=======================");
    
    lastDisplay = millis();
  }
}

void sendToThingSpeak() {
  HTTPClient http;
  
  // Create URL for ThingSpeak API with all sensor data
  String url = "http://api.thingspeak.com/update?api_key=" + String(thingSpeakApiKey) +
               "&field1=" + String(myBPM) +
               "&field2=" + String(signalStrength) +
               "&field3=" + String(objectTemp) +
               "&field4=" + String(ambientTemp);
  
  Serial.println("Sending to ThingSpeak: " + url);
  
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    Serial.println("Data sent to ThingSpeak successfully!");
    Serial.print("BPM: "); Serial.println(myBPM);
    Serial.print("Signal: "); Serial.println(signalStrength);
    Serial.print("Object Temp: "); Serial.print(objectTemp); Serial.println("°C");
    Serial.print("Ambient Temp: "); Serial.print(ambientTemp); Serial.println("°C");
  } else {
    Serial.println("Error sending data to ThingSpeak. HTTP Code: " + String(httpCode));
  }
  
  http.end();
}

void handleClientRequest(WiFiClient client) {
  Serial.println("New Client.");
  String currentLine = "";
  unsigned long timeout = millis() + 250; // Client timeout
  
  while (client.connected() && millis() < timeout) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      
      if (c == '\n') {
        if (currentLine.length() == 0) {
          // Send HTTP response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          
          // Web page content
          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">"); // Auto-refresh every 5 seconds
          client.println("<link rel=\"icon\" href=\"data:,\">");
          client.println("<style>");
          client.println("body { text-align: center; font-family: Arial, sans-serif; background: linear-gradient(to bottom, #1a1a2e, #16213e); color: white; margin: 0; padding: 20px; }");
          client.println(".container { max-width: 600px; margin: 0 auto; background: rgba(255, 255, 255, 0.1); border-radius: 15px; padding: 20px; box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1); }");
          client.println(".heart { color: #e94560; font-size: 3em; animation: pulse 1.5s infinite; }");
          client.println("@keyframes pulse { 0% { transform: scale(1); } 50% { transform: scale(1.1); } 100% { transform: scale(1); } }");
          client.println(".data { font-size: 1.8em; margin: 20px; padding: 15px; background: rgba(255, 255, 255, 0.15); border-radius: 10px; }");
          client.println(".temp-data { font-size: 1.4em; margin: 15px; padding: 10px; background: rgba(255, 255, 255, 0.1); border-radius: 8px; }");
          client.println(".signal { height: 20px; background: #4cc9f0; border-radius: 10px; margin: 10px 0; }");
          client.println("h1 { color: #4cc9f0; }");
          client.println(".status { margin-top: 20px; font-size: 0.9em; color: #a5b1c2; }");
          client.println("</style></head>");
          client.println("<body><div class='container'>");
          client.println("<h1>ESP32 Health Monitor</h1>");
          
          if (beatDetected && (millis() - lastBeatTime < 5000)) {
            client.println("<div class=\"heart\">♥</div>");
            client.println("<div class=\"data\">BPM: " + String(myBPM) + "</div>");
            client.println("<div>Signal Strength: " + String(signalStrength) + "</div>");
            client.println("<div class=\"signal\" style=\"width: " + String(map(signalStrength, 0, 4095, 0, 100)) + "%;\"></div>");
            client.println("<div>Last beat: " + String((millis() - lastBeatTime)/1000) + " seconds ago</div>");
          } else {
            client.println("<div class=\"data\">Place your finger on the sensor</div>");
            client.println("<div>Signal Strength: " + String(signalStrength) + "</div>");
            client.println("<div class=\"signal\" style=\"width: " + String(map(signalStrength, 0, 4095, 0, 100)) + "%;\"></div>");
            beatDetected = false;
          }
          
          // Temperature data
          client.println("<div class=\"temp-data\">");
          client.println("Object Temperature: " + String(objectTemp) + "°C<br>");
          client.println("Ambient Temperature: " + String(ambientTemp) + "°C");
          client.println("</div>");
          
          client.println("<div class=\"status\">");
          client.println("ThingSpeak: " + String((WiFi.status() == WL_CONNECTED) ? "Connected" : "Disconnected"));
          client.println("<br>Next update in: " + String((thingSpeakPostDelay - (millis() - lastThingSpeakPost)) / 1000) + "s");
          client.println("</div>");
          
          client.println("</div></body></html>");
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
  Serial.println("Client disconnected.");
}