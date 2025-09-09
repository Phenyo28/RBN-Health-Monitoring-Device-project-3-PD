void setup() {
  Serial.begin(115200);   // start serial monitor
}

void loop() {
  Serial.println(analogRead(34));   // read analog value (0–4095 on ESP32)
  delay(100);                       // small delay for readability
}

