/*********************************************************************

  Example: Read from a BMP180 Sensor

*********************************************************************/

#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
  Serial.begin(57600);

  while (!Serial) {
    delay(1);
  }

  if (!bmp.begin()) {
    Serial.println("BMP180 not present");
    while (true);
  }
}

void loop() {
  float pressure;
  bmp.getPressure(&pressure);

  Serial.print("Pressure: ");
  Serial.print(pressure / 1000);
  Serial.println(" kPa");

  float temperature;
  bmp.getTemperature(&temperature);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  delay(1000);
}
