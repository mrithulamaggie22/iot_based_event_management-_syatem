#include "ThingSpeak.h"
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include<SPI.h>
#include<MFRC522.h>
#include <Wire.h>
#include <time.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SDA_PIN = D4; 
MFRC522 rfid(SDA_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
unsigned long ch_no =1483352;
const char *ssid = "mathu";  
const char *password = "mathu@123";
long host = 1451372;
const char  apiKey[] = "OMR70O2JK75LUPSV"; 
int count = 0;
String card_no;
String name;
int a = 0, b = 0;
WiFiClient  client;
const char *str;
void setup() 
{
  Serial.begin(9600);
  configTime(11 * 1800, 0, "pool.ntp.org", "time.nist.gov");
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  rfid.PCD_Init();
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO   ");
  lcd.setCursor(0, 1);
  lcd.print("  AGNI COLLEGE  ");
  delay(1000);
  lcd.clear();

  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  //WiFi.mode(WIFI_AP_STA);   //Both hotspot and client are enabled
  //WiFi.mode(WIFI_AP);       //Only Access point
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  ThingSpeak.begin(client);
}

  

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("No. of Vis:" + String(count));
  time_t now = time(nullptr);
  str = ctime(&now);
  if(count < 100)
  {
    if ( ! rfid.PICC_IsNewCardPresent())
      return;
    if(rfid.PICC_ReadCardSerial()) {
      for (byte i = 0 ;i < 6; i++) {
        card_no += rfid.uid.uidByte[i];
        
      }
      Serial.println(card_no);
      
      if ((card_no == "21331814800") && (a==0))
       {
         lcd.setCursor(0,0);
         lcd.print("Welcome Praveen         ");
         name = "Praveen";
         a = 1;
         count++;
         delay(1000);
         lcd.clear();
      }
      else if ((card_no == "236951989100")  && (b==0))
      {
        lcd.setCursor(0,0);
        lcd.print("Welcome Mrithula        ");
        name = "Mrithula";
        b = 1;
        count++;
        delay(1000);
        
        lcd.clear();
      }
      else
       {
          lcd.setCursor(0, 1);
          lcd.print("Welcome Back!!!          ");
          delay(1000);
          lcd.clear();
       }
      card_no="";
      
      
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      ThingSpeak.writeField(host,1,name,apiKey);
      ThingSpeak.writeField(host,2,str,apiKey);
      name="";
      
      
    }
  }

}
