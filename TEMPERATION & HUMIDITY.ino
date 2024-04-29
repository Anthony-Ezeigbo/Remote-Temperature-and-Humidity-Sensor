#include <WiFiNINA.h>
#include "arduino_secrets.h"
#include "DHT.h" //include the DHT11 Library for interfacing with the sensor
#include "LiquidCrystal.h"
#include "ThingSpeak.h" // always include thingspeak header file after otherheader files
LiquidCrystal lcd(8,7,6,5,4,3);

char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASSWORD; // your network password
WiFiClient client;

unsigned long myChannelNumber =  2331344; // Write Channel Number generated from thingspeak website
const char * myWriteAPIKey = SECRET_API_KEY;  // Write API KEY generated from thingspeak website
int number = 0;

// create an instance of the DHT11 class
// - for Arduino connect the sensor to digital I/0 Pin 2
#define DHTPIN 2 

#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Initialize serial communication to allow debugging and data readout 
// Using a baud rate of 9600 bps
void setup() {
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
    }String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {Serial.println("Please upgrade the firmware");
    }
    
    ThingSpeak.begin(client); // Initialize ThingSpeak

  Serial.println(F("DHTxx test!"));
  lcd.begin(16,2);

  dht.begin();
}

void loop() {
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID");
    while(WiFi.status() != WL_CONNECTED){
  WiFi.begin(ssid, pass);
  Serial.print(".");
  delay(5000);
  
  }
  Serial.println("\nConnected.");
  }
// pieces of information in a channel. Here, we write to field 1 and 2


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  int x = ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);
  int y = ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
  lcd.setCursor(0,0);
  lcd.print("Temp  ");
  lcd.print(String(t));
  lcd.setCursor(0,1);
  lcd.print("Humidity  ");
  lcd.print(String(h));

}
