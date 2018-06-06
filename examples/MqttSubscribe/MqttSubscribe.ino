/*********************************************************************

  Example: Subscribe to an Mqtt Broker

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

  #define LED_BUILTIN BOARD_LED_PIN

  #define connectWiFi(args...) Feather.connect(args)
  #define isWiFiConnected() Feather.connected()
#endif

#ifdef ARDUINO_ARCH_ESP8266
  #define toggleLED(on) digitalWrite(LED_BUILTIN, (on) ? LOW : HIGH);
#else
  #define toggleLED(on) digitalWrite(LED_BUILTIN, (on) ? HIGH : LOW);
#endif

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

void setup() {
  Serial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
  toggleLED(false);

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

  Serial.print("Subscribing to control topic...");
  subscribeMQTT("control/<insert name>", handler);

  Serial.println("done");
}

#ifndef ARDUINO_ARCH_WICED
void handler(char* topic, byte* payload, unsigned int length) {
  Serial.print("Control says: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strncmp((char*)payload, "on", length) == 0) {
    toggleLED(true);
  }

  if (strncmp((char*)payload, "off", length) == 0) {
    toggleLED(false);
  }
}
#endif

#ifdef ARDUINO_ARCH_WICED
void handler(UTF8String topic, UTF8String message) {
  Serial.print("Control says: ");
  Serial.println(message);

  if (message == "on") {
    toggleLED(true);
  }

  if (message == "off") {
    toggleLED(false);
  }
}
#endif

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
