#include "LiquidCrystal.h"
LiquidCrystal lcd(8,7,6,5,4,3);
int sensorPin = 0;
 
void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
}
 
void loop()
{
 
 int reading = analogRead(sensorPin);

 // measure the 5v with a meter for an accurate value

 float voltage = reading * 4.68;
 voltage /= 1024.0;
 
 // now print out the temperature

 float temperatureC = (voltage - 0.5) * 100;
 Serial.print(temperatureC);
 Serial.println(" degrees C");

   lcd.setCursor(0,0);
   lcd.print("Temperature ");
   lcd.setCursor(0,1);
   lcd.print("degrees C");
   lcd.setCursor(11,1);
   lcd.print(temperatureC);
 
 delay(100);
}