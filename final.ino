// Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

// Constant values
#define INTERVAL 30000
#define LCD_INTERVAL 5000

// Temperature sensor
OneWire oneWire(9);
DallasTemperature sensors(&oneWire);
EthernetClient client;

// Define LCD 
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);

// Ethernet variables
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x96, 0xB8 };
char server[] = "data.sparkfun.com";

// Pre-defined variables
unsigned long lastRun = 0;
unsigned long lastLcdUpdate = 0;

// setup() runs once on startup
void setup() 
{
  // Start hardware (LED lights)
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  // Start hardware (LCD screen)
  lcd.begin(16, 2);
  // Define temperature sensor
  sensors.begin();
  // Define output speed for console
  Serial.begin(115200); 
  while (!Serial) { ; }
  // Connect to internet
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    while(1){}
  }
  delay(1000);
  Serial.println("connecting...");
}

// loop() runs continuously as fast as possible
void loop() 
{
  // If internet connection works
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // If internet connection doesn't work
  if (!client.connected()) {
    client.stop();
  }

  // Make request once in a while
  if (lastRun + INTERVAL < millis())
  {
    makeRequest();
    lastRun = millis();
  }

  // Check temperature value
  if( lastLcdUpdate + LCD_INTERVAL < millis())
  {
    sensors.requestTemperatures();

    // Get Celcius value and display 1st pin (T < 13)
    if (sensors.getTempCByIndex(0) < 13) 
    {
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
    }
    // Get Celcius value and display 2nd pin (13 < T < 21)
    else if (sensors.getTempCByIndex(0) < 21)
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
    }
    // Get Celcius value and display 3rd pin (21 < T < 27)
    else if (sensors.getTempCByIndex(0) < 27)
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
    }
    // Get Celcius value and display 4th pin (T > 27)
    else 
    {
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
    }

    // Display values on LCD screen in Farenheit on 1st row
    lcd.setCursor(0, 0);
    lcd.print(sensors.getTempFByIndex(0));
    lcd.print(" F ");
    lcd.setCursor(0, 1);
    // Display values on LCD screen in Celcius on 2nd row
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C ");
    lastLcdUpdate = millis();
  }
}

void makeRequest() {
  // Make request to data.sparkfun.com
  //http://data.sparkfun.com/input/3JynOpWyqrHnrvjNdzrm?private_key=oZDb0rpDgeH6AbEXPqAB&temp_c=23.10&temp_f=1.11
  // If success
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
  // If fail
  else 
  {
    Serial.println("connection failed");
  }
}
