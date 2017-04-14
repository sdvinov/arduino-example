# Arduino Example

## Hardware
* DS18B20 Temperature Sensor
* LED lights (blue, green, yellow, red)
* LCD screen 2x16
* Ethernet wire

## What it does
This example was created by me for **Arduino Genuino Uno** platform. 

Gets temperature value from thermometer:

```arduino
sensors.requestTemperatures();
``` 

Compare value:

```arduino
// Get Celcius value and display blue pin (T < 13)
if (sensors.getTempCByIndex(0) < 13) 
{
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}
// Get Celcius value and display green pin (13 < T < 21)
else if (sensors.getTempCByIndex(0) < 21)
{
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}
// Get Celcius value and display yellow pin (21 < T < 27)
else if (sensors.getTempCByIndex(0) < 27)
{
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
}
// Get Celcius value and display red pin (T > 27)
else 
{
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
}
```

Display it on LCD screen:

```arduino
// Display values on LCD screen in Farenheit on 1st row
lcd.setCursor(0, 0);
lcd.print(sensors.getTempFByIndex(0));
lcd.print(" F ");
lcd.setCursor(0, 1);
// Display values on LCD screen in Celcius on 2nd row
lcd.print(sensors.getTempCByIndex(0));
lcd.print(" C ");
```

Send data to 3rd party database:

```arduino
Serial.println("connected");
client.print("GET /input/_YOUR_ID_?private_key=_YOUR_KEY_&temp_c=");
client.print(sensors.getTempCByIndex(0));
client.print("&temp_f=");
client.print(sensors.getTempFByIndex(0));
client.println(" HTTP/1.1");
client.println("Host: data.sparkfun.com");
client.println("Connection: close");
client.println();
```

Parse it with javascript on a [simple site example](https://github.com/sdvinov/arduino-example/site).

