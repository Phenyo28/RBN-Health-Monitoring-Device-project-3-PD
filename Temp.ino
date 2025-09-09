#include <WiFi.h>
#include <Wire.h>
#include <DFRobot_MLX90614.h>

// ===== WiFi credentials =====
const char* ssid = "E5783_9CEA";
const char* password = "aDeTi3n8NFqBdRi3";

// ===== Web server =====
WiFiServer server(80);
String header;

// ===== Temperature Sensor =====
DFRobot_MLX90614_I2C sensor; // MLX90614 object

void setup() {
  Serial.begin(115200);

  // ===== Initialize MLX90614 sensor =====
  while (NO_ERR != sensor.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Sensor ready!");

  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);

  // ===== Connect to WiFi =====
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    Serial.println("New Client connected.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            // ===== Read sensor data =====
            float ambientTemp = sensor.getAmbientTempCelsius();
            float objectTemp = sensor.getObjectTempCelsius();

            // ===== Send webpage =====
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>ESP32 Temperature Monitor</title></head>");
            client.println("<body style='font-family: Arial; text-align: center;'>");
            client.println("<h2>MLX90614 Temperature Sensor</h2>");
            client.println("<p>Ambient Temperature: " + String(ambientTemp) + " °C</p>");
            client.println("<p>Object Temperature: " + String(objectTemp) + " °C</p>");
            client.println("</body></html>");

            // End response
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}
