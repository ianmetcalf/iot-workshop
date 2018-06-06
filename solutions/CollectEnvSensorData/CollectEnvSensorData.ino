/*********************************************************************

  Solution: Collect Environmental Sensor Data

*********************************************************************/

#ifdef ARDUINO_SAMD_FEATHER_M0
  #include <WiFi101.h>
#endif

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif

#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#ifndef ARDUINO_ARCH_WICED
  #include <PubSubClient.h>

  #define connectWiFi(args...) WiFi.begin(args)
  #define isWiFiConnected() (WiFi.status() == WL_CONNECTED)
#endif

#ifdef ARDUINO_ARCH_WICED
  #include <adafruit_feather.h>
  #include <adafruit_mqtt.h>

  #define connectWiFi(args...) Feather.connect(args)
  #define isWiFiConnected() Feather.connected()
#endif

#include <Adafruit_SHT31.h>

const char* ssid = "iot workshop";
const char* password = "burlingtoncode";

const char* broker = "broker.local.lan";
const char* clientId = "<insert name>";

#ifndef ARDUINO_ARCH_WICED
  WiFiClient wifiClient;
  PubSubClient mqtt(wifiClient);

  #define connectMQTT(id, args...) (mqtt.setServer(args), mqtt.connect(id))
#endif

#ifdef ARDUINO_ARCH_WICED
  AdafruitMQTT mqtt;

  #define connectMQTT(id, args...) (mqtt.clientID(id), mqtt.connect(args))
#endif

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(57600);

  if (!sht31.begin(0x44)) {
    Serial.println("Sensor not present");
    while (true);
  }

  #ifdef ARDUINO_SAMD_FEATHER_M0
  WiFi.setPins(8, 7, 4, 2);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi not present");
    while (true);
  }
  #endif

  #ifdef ARDUINO_ARCH_ESP8266
  WiFi.mode(WIFI_STA);
  #endif

  Serial.print("Connecting to WiFi...");
  connectWiFi(ssid, password);

  while (!isWiFiConnected()) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println("done");

  Serial.print("Connecting to MQTT...");
  connectMQTT(clientId, broker, 1883);

  while (!mqtt.connected()) {
    Serial.print('.');
    delay(1000);
  }

  Serial.println("done");
}

void loop() {
  checkConnection();
  readSensor();

  #ifndef ARDUINO_ARCH_WICED
  mqtt.loop();
  #endif
}

void checkConnection() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 10000) {
    if (!isWiFiConnected()) {
      connectWiFi(ssid, password);
    }

    if (isWiFiConnected() && !mqtt.connected()) {
      connectMQTT(clientId, broker, 1883);
    }

    last = now;
  }
}

void readSensor() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 60000) {
    char topic[50];
    char msg[25];

    sprintf(topic, "data/%s/temperature", clientId);
    sprintf(msg, "%f F", (sht31.readTemperature() * 1.8) + 32);

    Serial.print("Publishing temperature...");
    mqtt.publish(topic, msg);
    Serial.println("done");

    sprintf(topic, "data/%s/humidity", clientId);
    sprintf(msg, "%f %%", sht31.readHumidity());

    Serial.print("Publishing humidity...");
    mqtt.publish(topic, msg);
    Serial.println("done");

    last = now;
  }
}
