/*********************************************************************

  Example: Publish to an Mqtt Broker

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

void setup() {
  Serial.begin(57600);

  while (!Serial) {
    delay(1);
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

  Serial.print("Publishing hello world...");
  mqtt.publish("world", "Hello from <insert name>!");

  Serial.println("done");
}

void loop() {
  checkConnection();

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
