/*********************************************************************

  Example: Read from a BMP280 Sensor

*********************************************************************/

#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

void setup() {
  Serial.begin(57600);

  while (!Serial) {
    delay(1);
  }

  if (!bmp.begin()) {
    Serial.println("BMP280 not present");
    while (true);
  }
}

void loop() {
  Serial.print("Pressure: ");
  Serial.print(bmp.readPressure() / 1000);
  Serial.println(" kPa");

  Serial.print("Temperature: ");
  Serial.print(bmp.readTemperature());
  Serial.println(" C");

  delay(1000);
}
