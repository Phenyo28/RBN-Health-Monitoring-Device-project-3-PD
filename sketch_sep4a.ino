int pulsePin = 34;   // Pulse Sensor connected to GPIO34 (ADC1 channel 6)
int signal;          // stores the incoming raw signal

void setup() {
  Serial.begin(115200);   // start serial monitor
}

void loop() {
  signal = analogRead(pulsePin);   // read analog value (0–4095 on ESP32)
  Serial.println(signal);          // print the raw signal
  delay(10);                       // small delay for readability
}

