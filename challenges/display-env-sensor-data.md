# Display Environmental Sensor Data

## Challenge

Write a program to [display](/activities/oled-display.md) measurements being published to the
MQTT broker.

## Details

- Measurements will be published to the topic `data/<id>/<measurement>`
  - `<id>` is a unique identifier for the device that took the measurement
  - `<measurement>` is the type of value (i.e. "temperature", "humidity", "barometric pressure", etc)
- Messages will be in format `<value> <units>`
- Display the id, value and units for **only** one type of measurement

## Bonus

In addition to displaying readings, the program should publish control messages

- When the "A" button is pressed, an `"on"` message should be published
- When the "C" button is pressed, an `"off"` message should be published
- Messages should be published to the `control/<id>/led` topic
  - Use the `<id>` of the last reading received
