/*********************************************************************

  Example: Display wifi information

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
  #define connectWiFi(args...) WiFi.begin(args)
  #define isWiFiConnected() (WiFi.status() == WL_CONNECTED)
  #define getRSSI() WiFi.RSSI()
  #define getLocalIP() WiFi.localIP()
#endif

#ifdef ARDUINO_ARCH_WICED
  #include <adafruit_feather.h>

  #define connectWiFi(args...) Feather.connect(args)
  #define isWiFiConnected() Feather.connected()
  #define getRSSI() Feather.RSSI()
  #define getLocalIP() reverse(Feather.localIP())
#endif

#include <Adafruit_FeatherOLED_WiFi.h>

const char* ssid = "iot workshop";
const char* password = "burlingtoncode";

Adafruit_FeatherOLED_WiFi display = Adafruit_FeatherOLED_WiFi();

void setup() {
  display.init();
  display.setBatteryVisible(false);
  display.setRSSIIcon(true);
  display.setRSSIAsPercentage(true);
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
}

void loop() {
  checkConnection();
  updateDisplay();
}

void checkConnection() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 10000) {
    if (!isWiFiConnected()) {
      connectWiFi(ssid, password);
    }

    last = now;
  }
}

void updateDisplay() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 1000) {
    if (isWiFiConnected()) {
      display.setConnected(true);
      display.setRSSI(getRSSI());
      display.setIPAddress(getLocalIP());
    } else {
      display.setConnected(false);
      display.setRSSI(0);
      display.setIPAddress(0);
    }

    display.refreshIcons();
    last = now;
  }
}

#ifdef ARDUINO_ARCH_WICED
uint32_t reverse(uint32_t x) {
  return ((x & 0xFF) << 24) |
    ((x & 0xFF00) << 8) |
    ((x & 0xFF0000) >> 8) |
    ((x & 0xFF000000) >> 24);
}
#endif
