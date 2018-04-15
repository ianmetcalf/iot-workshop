/*********************************************************************

  Example: Display battery voltage

*********************************************************************/

#include <Adafruit_FeatherOLED.h>

Adafruit_FeatherOLED display = Adafruit_FeatherOLED();

void setup() {
  display.init();
  display.clearDisplay();
}

void loop() {
  updateDisplay();
}

void updateDisplay() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 1000) {
    display.clearDisplay();
    display.setBattery(readBatteryVolts());
    display.renderBattery();
    display.display();

    last = now;
  }
}

float readBatteryVolts() {
  float voltage = 0.0;

  #ifdef ARDUINO_SAMD_FEATHER_M0
  voltage = analogRead(A7) * 2 * 3.3 / 1024;
  #endif

  #ifdef ARDUINO_STM32_FEATHER
  voltage = analogRead(PA1) * 2 * 3.3 / 4096;
  #endif

  #ifdef ARDUINO_ESP8266_ESP12
  voltage = analogRead(A0) * 2 * 3.3 / 1024;
  #endif

  #ifdef ARDUINO_FEATHER_ESP32
  voltage = analogRead(A13) * 2 * 3.3 / 4096;
  #endif

  return voltage;
}
