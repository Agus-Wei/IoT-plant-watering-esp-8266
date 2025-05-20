#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const char* mqtt_user = MQTT_USERNAME;
const char* mqtt_pass = MQTT_PASSWORD;

const int pumpPin = D3;

WiFiClient espClient;
PubSubClient client(espClient);

bool pumpState = false;

void setup() {
  Serial.begin(115200);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);

  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi...");
  }

  Serial.println("WiFi Connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  connectMqtt();
  Serial.println("MQTT Connected");
}

void loop() {
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

  if (String(topic) == "device/pump") {
    if (msg == "1") {
      turnPumpOn();
    } else {
      turnPumpOff();
    }
  }
}

void turnPumpOn() {
  digitalWrite(pumpPin, HIGH);
  pumpState = true;
  Serial.println("Pump On");
}

void turnPumpOff() {
  digitalWrite(pumpPin, LOW);
  if (pumpState) {
    pumpState = false;
    Serial.println("Pump Off");
  }
}

void connectMqtt() {
  const char* clientId = "ESP8266Client";

  while (!client.connected()) {
    Serial.println("Connecting MQTT...");

    if (client.connect(clientId, mqtt_user, mqtt_pass)) {
      client.subscribe("device/pump");
    } else {
      delay(1000);
    }
  }
}
