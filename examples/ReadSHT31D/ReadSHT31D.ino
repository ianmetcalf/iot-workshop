/*********************************************************************

  Example: Read from an SHT31-D Sensor

*********************************************************************/

#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

const byte addr = 0x44;

void setup() {
  Serial.begin(57600);

  while (!Serial) {
    delay(1);
  }

  if (!sht31.begin(addr)) {
    Serial.println("SHT31-D not present");
    while (true);
  }
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(sht31.readTemperature());
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(sht31.readHumidity());
  Serial.println(" %");

  delay(1000);
}
