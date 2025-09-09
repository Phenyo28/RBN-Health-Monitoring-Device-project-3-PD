#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//I2C address = 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
;
  Wire.begin();
  Serial.begin(115200);
  lcd.init();       // initialize the lcd
  lcd.backlight();  // turn on backlight

  lcd.setCursor(0, 0);  
  lcd.print("Welcome to IoT Healthcare");
  
  lcd.setCursor(0, 1);   
  lcd.print("Let's get started!");
}


void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Pulse: 75 bpm ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: 36.5 C ");
  delay(2500);   

  lcd.clear();   // Clear the screen

  lcd.setCursor(0, 0);
  lcd.print("Health Monitor");
  lcd.setCursor(0, 1);
  lcd.print("IoT Device");
  delay(3000);  
}

