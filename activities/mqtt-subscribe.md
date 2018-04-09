# Subscribe to an MQTT Broker

## Getting Setup

*NOTE: This step is not needed if you are using the WICED Wifi Feather board*

* Download the [PubSubClient](https://pubsubclient.knolleary.net) library from [here](https://github.com/knolleary/pubsubclient/archive/master.zip)

* Uncompress the folder and rename it `pubsubclient`

* Place the folder in your `arduinosketchfolder/libraries/` folder

## Code

* Open the Arduino IDE and start a new sketch

* At the top define the following includes and macros

```c
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
```

* Then define the following constants

```c
const char* ssid = "iot workshop";
const char* password = "burlingtoncode";

const char* broker = "broker.local.lan";
const char* clientId = "<insert name>";
```

* And initialize the mqtt client

```c
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
```

* Then define the setup function

```c
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

  Serial.print("Subscribing to control topic...");
  subscribeMQTT("control/<insert name>", handler);

  Serial.println("done");
}
```

* And a handler for the mqtt subscription

```c
#ifndef ARDUINO_ARCH_WICED
void handler(char* topic, byte* payload, unsigned int length) {
  Serial.print("Control says: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
#endif

#ifdef ARDUINO_ARCH_WICED
void handler(UTF8String topic, UTF8String message) {
  Serial.print("Control says: ");
  Serial.println(message);
}
#endif
```

* Finally define the loop function

```c
void loop() {
  #ifndef ARDUINO_ARCH_WICED
  mqtt.loop();
  #endif
}
```

## Try It

* Load the program onto the microcontroller using the Arduino IDE

* Open the MQTT [browser client](http://www.hivemq.com/demos/websocket-client/)

* Connect to the broker and publish to your control topic

* You should see your message in the serial monitor
