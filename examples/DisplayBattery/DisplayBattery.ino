/*********************************************************************

  Example: Display battery voltage

*********************************************************************/

#include <Adafruit_FeatherOLED.h>

#ifdef ARDUINO_SAMD_FEATHER_M0
  #define VBAT_PIN A7
  #define readBatteryVoltage() (analogRead(VBAT_PIN) * 2 * 3.3 / 1024)
#endif

#ifdef ARDUINO_STM32_FEATHER
  #define VBAT_PIN PA1
  #define readBatteryVoltage() (analogRead(VBAT_PIN) * 2 * 3.3 / 4096)
#endif

#ifdef ARDUINO_ESP8266_ESP12
  #define VBAT_PIN A0
  #define readBatteryVoltage() (analogRead(VBAT_PIN) * 2 * 3.3 / 1024)
#endif

#ifdef ARDUINO_FEATHER_ESP32
  #define VBAT_PIN A13
  #define readBatteryVoltage() (analogRead(VBAT_PIN) * 2 * 3.3 / 4096)
#endif

Adafruit_FeatherOLED display = Adafruit_FeatherOLED();

void setup() {
  display.init();
  display.clearDisplay();

  #ifdef ARDUINO_STM32_FEATHER
  pinMode(VBAT_PIN, INPUT_ANALOG);
  #endif
}

void loop() {
  updateDisplay();
}

void updateDisplay() {
  unsigned long now = millis();
  static unsigned long last = 0;

  if (now - last >= 1000) {
    display.clearDisplay();
    display.setBattery(readBatteryVoltage());
    display.renderBattery();
    display.display();

    last = now;
  }
}
