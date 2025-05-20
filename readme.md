add a secrets.h on the same directory with your .ino file

secrets.h
```
#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID       "YourWiFiSSID"
#define WIFI_PASSWORD   "YourWiFiPassword"

#define MQTT_SERVER     "192.168.1.100"
#define MQTT_PORT       1883
#define MQTT_USERNAME   "your_mqtt_user"
#define MQTT_PASSWORD   "your_mqtt_password"

#endif
```