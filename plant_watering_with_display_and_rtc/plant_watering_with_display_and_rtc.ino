#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid = "<your-SSID>";
const char* password = "<your-PASSWORD>";

const char* mqtt_server = "<your-MQTT-SERVER>";
const char* mqtt_user = "<your-MQTT-USER>";
const char* mqtt_pass = "<your-MQTT-PASSWORD>";

const int pumpPin = D3;

RTC_DS3231 rtc;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

WiFiClient espClient;
PubSubClient client(espClient);

bool pumpState = false;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 8 * 3600, 60000);

unsigned long lastDisplayUpdate = 0;
String lastMessage = "";

void showMessage(String msg, int line = 3) {
  lastMessage = msg;
  Serial.println(msg);

  int y = (line - 1) * 8;  

  if (line < 1 || line > 8) return;

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.fillRect(0, y, 128, 8, SSD1306_BLACK);  
  display.setCursor(0, y);
  display.println(lastMessage);
  display.display();
}

void initializeDS3231() {
  rtc.begin();

  timeClient.begin();
  showMessage("Syncing NTP...");

  int retryCount = 0;
  const int maxRetries = 10;

  while (!timeClient.update() && retryCount < maxRetries) {
    showMessage("Waiting NTP... Attempt " + String(retryCount + 1));
    timeClient.forceUpdate();
    delay(2000);
    retryCount++;
  }

  if (timeClient.isTimeSet()) {
    unsigned long epochTime = timeClient.getEpochTime();
    DateTime now = DateTime(epochTime);
    rtc.adjust(now);
    showMessage("RTC Synced to NTP");
  } else {
    showMessage("NTP Sync Failed");
  }  
}

void setup() {
  Serial.begin(115200);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
  display.clearDisplay();
  display.display();

  showMessage("Connecting WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    showMessage("WiFi...");
  }
  showMessage("WiFi Connected");

  initializeDS3231();

  display.clearDisplay();
  display.display();
  updateClockLine();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  connectMqtt();
  showMessage("MQTT Connected");
}

void loop() {
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();

  DateTime now = rtc.now();
  unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = currentMillis;
    updateClockLine();
  }
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
  showMessage("Pump On", 5);
}

void turnPumpOff() {
  digitalWrite(pumpPin, LOW);
  if (pumpState) {
    pumpState = false;
    showMessage("Pump Off", 5);
  }
}

void connectMqtt() {
  const char* clientId = "ESP8266Client";

  while (!client.connected()) {
    showMessage("Connecting MQTT...");

    if (client.connect(clientId, mqtt_user, mqtt_pass)) {
      client.subscribe("device/pump");
    } else {
      delay(1000);
    }
  }
}

int getWifiStrengthPercent() {
  int rssi = WiFi.RSSI();
  int percent = map(rssi, -100, -30, 0, 100);
  percent = constrain(percent, 0, 100);
  return percent;
}

void updateClockLine() {
  DateTime now = rtc.now();
  String timeStr = String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute()) + ":" + (now.second() < 10 ? "0" : "") + String(now.second());

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.fillRect(0, 0, 88, 8, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.print(timeStr);

  int strength = getWifiStrengthPercent();
  int barWidth = map(strength, 0, 100, 0, 40);
  display.fillRect(88, 0, 40, 8, SSD1306_BLACK);
  display.fillRect(88, 0, barWidth, 8, SSD1306_WHITE);

  display.display();
}
