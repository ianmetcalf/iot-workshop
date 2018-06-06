/*********************************************************************

  Solution: Display Environmental Sensor Data

*********************************************************************/

#ifdef ARDUINO_SAMD_FEATHER_M0
  #include <WiFi101.h>
#endif

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>

  #define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>

  #define min(a,b) ((a)<(b)?(a):(b))
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

#include <Adafruit_FeatherOLED.h>

const char* ssid = "iot workshop";
const char* password = "burlingtoncode";

const char* broker = "broker.local.lan";
const char* clientId = "<insert name>";

#ifndef ARDUINO_ARCH_WICED
  WiFiClient wifiClient;
  PubSubClient mqtt(wifiClient);

  #define connectMQTT(id, args...) (mqtt.setServer(args), mqtt.connect(id))
  #define subscribeMQTT(topic, handler) (mqtt.setCallback(handler), mqtt.subscribe(topic))
#endif

#ifdef ARDUINO_ARCH_WICED
  AdafruitMQTT mqtt;

  #define connectMQTT(id, args...) (mqtt.clientID(id), mqtt.connect(args))
  #define subscribeMQTT(topic, handler) mqtt.subscribe(topic, MQTT_QOS_AT_MOST_ONCE, handler)
#endif

Adafruit_FeatherOLED display = Adafruit_FeatherOLED();

#define MAX_SIZE 50
char lastId[MAX_SIZE];
char lastMsg[MAX_SIZE];

void setup() {
  display.init();
  display.clearDisplay();

  #ifdef ARDUINO_SAMD_FEATHER_M0
  WiFi.setPins(8, 7, 4, 2);

  if (WiFi.status() == WL_NO_SHIELD) {
    display.println("WiFi not present");
    display.display();
    while (true);
  }
  #endif

  #ifdef ARDUINO_ARCH_ESP8266
  WiFi.mode(WIFI_STA);
  #endif

  display.clearMsgArea();
  display.print("Connecting to WiFi...");
  display.display();

  connectWiFi(ssid, password);

  do {
    delay(1000);
  } while (!isWiFiConnected());

  display.clearMsgArea();
  display.print("Connecting to MQTT...");
  display.display();

  connectMQTT(clientId, broker, 1883);

  do {
    delay(1000);
  } while (!mqtt.connected());

  subscribeMQTT("data/+/temperature", handler);

  display.clearMsgArea();
}

#ifndef ARDUINO_ARCH_WICED
void handler(char* topic, byte* payload, unsigned int len) {
  if (strncmp(topic, "data/", 5) == 0) {
    splitTopic(lastId, topic + 5, MAX_SIZE);
    copy(lastMsg, (char*)payload, min(len + 1, MAX_SIZE));
    displayLastMessage();
  }
}
#endif

#ifdef ARDUINO_ARCH_WICED
void handler(UTF8String topic, UTF8String message) {
  if (strncmp(topic.data, "data/", 5) == 0) {
    splitTopic(lastId, topic.data + 5, min(topic.len + 1, MAX_SIZE));
    copy(lastMsg, message.data, min(message.len + 1, MAX_SIZE));
    displayLastMessage();
  }
}
#endif

void splitTopic(char* next, const char* topic, unsigned int n) {
  char* ptr = strchr(topic, '/');
  copy(next, topic, ptr ? min(n, ptr - topic + 1) : n);
}

void copy(char* dest, const char* src, unsigned int n) {
  strncpy(dest, src, n - 1);
  dest[n - 1] = '\0';
}

void displayLastMessage() {
  display.fillRect(0, 0, 128, 32, BLACK);
  display.setCursor(0, 0);

  display.println(lastId);
  display.println(lastMsg);

  display.display();
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
