#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h> 
#include <LiquidCrystal_I2C.h> 
#include "ThingSpeak.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int AirVal         = 620;
const int WaterVal       = 310;
int soilMoistureValueA   = 0;
int soilMoistureValueB   = 0;
String readingA          = ""; 
String readingB          = ""; 
int soilMoustureSensorPinA = A0;
int soilMoustureSensorPinB = D2;
//------- WI-FI details:
char ssid[] = "winternetIsComing"; //SSID here
char pass[] = "old_Mcd0nald"; // Passowrd here

//------ Channel details:
unsigned long Channel_ID = 1668758; // Your Channel ID
const char * myWriteAPIKey = "2LV2DR4I1H8C6JUP"; //Your write API key

const int PlantA = 1; 
const int PlantB = 2; 

WiFiClient client;
void setup() 
{
  Serial.begin(115200);
  pinMode(soilMoustureSensorPinA, INPUT);
  pinMode(soilMoustureSensorPinB, INPUT);
  lcd.begin(16, 2); // LCD screen
  lcd.init();         // initialize the LCD
  lcd.backlight();    // Turn on the blacklight
  lcd.clear();

  lcd.setCursor(1, 0);  
  lcd.print("   SMART");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print("  AGRICULTURE"); 
  WiFi.begin(ssid, pass);
 
  lcd.clear();
  lcd.setCursor(1, 0);  
  lcd.print("Connecting...");

  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(1, 0);  
  lcd.print("Connection is");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print("Established!"); 
  delay(1000);
} 
void loop() 
{
  soilMoistureValueA = analogRead(soilMoustureSensorPinA);
  
  soilMoistureValueB = analogRead(soilMoustureSensorPinB);
  Serial.print("Sensor A: ");
  Serial.println(soilMoistureValueA);
  Serial.print("Sensor B: ");
  Serial.println(soilMoistureValueB);
 
  lcd.clear();
  readingA = checkMoisture(soilMoistureValueA);  
  writeToLCD(lcd, 0, "A:", soilMoistureValueA, readingA);
  readingB = checkMoisture(soilMoistureValueB);
  writeToLCD(lcd, 1, "B:", soilMoistureValueB, readingB);
  ThingSpeak.writeField(Channel_ID, PlantA, soilMoistureValueA, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(Channel_ID, PlantB, soilMoistureValueB, myWriteAPIKey);
  delay(15000);
}
String checkMoisture(int soilMoistureValue)
{
  String reading = "";
  if(soilMoistureValue <= 500)
  {
     reading= "Normal LvL";
     Serial.println("Enough moisture.");
  }
  else 
  {
     reading= "Low LvL";
     Serial.println("Need water.");
  }   
  return reading;
}
void writeToLCD(LiquidCrystal_I2C lcd, int row, String sensor, int val1, String val2)
{
  lcd.setCursor(0, row);  
  lcd.print(sensor);
  lcd.setCursor(3, row);  
  lcd.print(val1);
  lcd.setCursor(8, row);  
  lcd.print(val2);  
}
