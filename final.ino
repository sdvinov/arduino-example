#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#define INTERVAL 30000
#define LCD_INTERVAL 5000
OneWire oneWire(9);
DallasTemperature sensors(&oneWire);
EthernetClient client;
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x96, 0xB8 };
char server[] = "data.sparkfun.com";
unsigned long lastRun = 0;
unsigned long lastLcdUpdate = 0;

void setup() 
{
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  lcd.begin(16, 2);
  sensors.begin();
  Serial.begin(115200); 
  while (!Serial) { ; }
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1){}
  }
  delay(1000);
  Serial.println("connecting...");
}

void loop() 
{
  
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected()) {
    client.stop();
  }
  
  if (lastRun + INTERVAL < millis())
  {
    makeRequest();
    lastRun = millis();
  }
  
  if( lastLcdUpdate + LCD_INTERVAL < millis())
  {
    sensors.requestTemperatures();
    if (sensors.getTempCByIndex(0) < 13) 
    {
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
    }
    else if (sensors.getTempCByIndex(0) < 21)
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
    }
    else if (sensors.getTempCByIndex(0) < 27)
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
    }
    else 
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
    }
    lcd.setCursor(0, 0);
    lcd.print(sensors.getTempFByIndex(0));
    lcd.print(" F ");
    lcd.setCursor(0, 1);
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C ");
    lastLcdUpdate = millis();
  }
}

void makeRequest() {
  //http://data.sparkfun.com/input/3JynOpWyqrHnrvjNdzrm?private_key=oZDb0rpDgeH6AbEXPqAB&temp_c=23.10&temp_f=1.11
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.print("GET /input/3JynOpWyqrHnrvjNdzrm?private_key=oZDb0rpDgeH6AbEXPqAB&temp_c=");
    client.print(sensors.getTempCByIndex(0));
    client.print("&temp_f=");
    client.print(sensors.getTempFByIndex(0));
    client.println(" HTTP/1.1");
    client.println("Host: data.sparkfun.com");
    client.println("Connection: close");
    client.println();
  } 
  else 
  {
    Serial.println("connection failed");
  }
}
