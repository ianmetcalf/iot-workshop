/*********************************************************************

  Example: Read from an HTU21D-F Sensor

*********************************************************************/

#include <Adafruit_HTU21DF.h>

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void setup() {
  Serial.begin(57600);

  while (!Serial) {
    delay(1);
  }

  if (!htu.begin()) {
    Serial.println("HTU21D-F not present");
    while (true);
  }
}

void loop() {
  Serial.print("Temperature: ");
  Serial.print(htu.readTemperature());
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(htu.readHumidity());
  Serial.println(" %");

  delay(1000);
}
