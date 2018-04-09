# Publish to an MQTT Broker

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
#endif

#ifdef ARDUINO_ARCH_WICED
  AdafruitMQTT mqtt;

  #define connectMQTT(id, args...) (mqtt.clientID(id), mqtt.connect(args))
#endif
```

* Finally define the setup function

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

  Serial.print("Publishing hello world...");
  mqtt.publish("world", "Hello from <insert name>!");

  Serial.println("done");
}
```

## Try It

* Open the MQTT [browser client](http://www.hivemq.com/demos/websocket-client/)

* Connect to the broker and subscribe to the `world` topic

* Load the program onto the microcontroller using the Arduino IDE

* You should see your hello message appear
