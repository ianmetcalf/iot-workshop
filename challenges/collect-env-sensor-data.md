# Collect Environmental Sensor Data

## Challenge

Write a program to read measurements from one of the [environmental sensors](/activities/env-sensors.md)
and publish the readings to the MQTT broker.

## Details

- Measurements should be taken once per minute
- Each measurment should be published to the `data/<id>/<measurement>` topic
  - `<id>` is a unique identifier for the device, you can hard code a constant for this in the program
  - `<measurement>` is the type of value (i.e. "temperature", "humidity", "barometric pressure", etc)
- Messages should use the format `<value> <units>`

## Bonus

In addition to publishing readings, the program should handle control messages

- The program should subscribe to the `control/<id>/led` topic
- When a message containing `"on"` is received the builtin LED should be turned on
- When a message containing `"off"` is received the builtin LED should be turned off
