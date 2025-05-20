## Software Requirements

Before uploading the code to your ESP8266 board, make sure you have installed the following software:

- **Arduino IDE**  
    - [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
- **CH340 USB-Serial Driver** (required if your board uses the CH340 chip):  
    - [https://sparks.gogo.co.nz/ch340.html](https://sparks.gogo.co.nz/ch340.html)

> **Note:**  
> Install both the Arduino IDE and the necessary drivers before connecting and programming your board.

## Hardware Components

Below is a list of hardware components used in this project, along with links to purchase them:
- ESP8266 NodeMCU board with base plate ([Link to buy](https://s.shopee.co.id/2qI9jK2PeT))
- Relay module ([Link to buy](https://s.shopee.co.id/4AnXKVecMh))
- Water pump ([Link to buy](https://s.shopee.co.id/2LLt8TXhko))
- Jumper wires 
    - Male to male ([Link to buy](https://s.shopee.co.id/10qVXry0Sg))
    - Male to female ([Link to buy](https://s.shopee.co.id/4L6xVyYWOZ))
    - Female to female ([Link to buy](https://s.shopee.co.id/3LEQKAXObQ))
- Breadboard ([Link to buy](https://s.shopee.co.id/8AJg5q3vu9))
- 12V DC adaptor ([Link to buy](https://s.shopee.co.id/5Ag4VYCske))
- OLED 128x64 display ([Link to buy](https://s.shopee.co.id/3frGio9jUv))
- RTC DS3231 ([Link to buy](https://s.shopee.co.id/6fUsJ3AvsA))
- DC to DC step down converter ([Link to buy](https://s.shopee.co.id/2LLt8MwfVA))
- Terminal ([Link to buy](https://s.shopee.co.id/8Kd6HPHH3X))
- DC Barrel male jack ([Link to buy](https://s.shopee.co.id/9pRu4Ay1sl))
- DC Barrel female jack ([Link to buy](https://s.shopee.co.id/AKOAexQdni))

## Secrets
add a secrets.h on the same directory with your .ino file

secrets.h
```
#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID       "YourWiFiSSID"
#define WIFI_PASSWORD   "YourWiFiPassword"

#define MQTT_SERVER     "MQTTHost"
#define MQTT_PORT       1883
#define MQTT_USERNAME   "your_mqtt_user"
#define MQTT_PASSWORD   "your_mqtt_password"

#endif
```
