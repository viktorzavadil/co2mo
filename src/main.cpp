#include <Arduino.h>

#include <scd41.h>

#include "config.h"
#include "built_in_led.h"

Scd41 sensor(SENSOR_TEMPERATURE_OFFSET, SENSOR_ALTITUDE);
BuiltInLed builtInLed(PIN_BUILT_IN_LED);

void printSerialNumber(uint64_t value)
{
  Serial.print("Serial: 0x");
  Serial.print(value, HEX);
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && millis() - start < 3000)
  {
    delay(10);
  }

  Serial.println(F("[SETUP] Initialization..."));

  builtInLed.begin();

  uint16_t error = sensor.begin(PIN_SDA, PIN_SCL);
  char errorMessage[256];

  if (error)
  {
    Serial.print("Inicialization SCD41 sensor failed: ");
    sensor.errorText(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
}

void printSensorMeasurement() {
    bool ready;
  uint16_t error = sensor.getDataReadyStatus(ready);
  if (error) {
    char errorMessage[256];
    Serial.print("Reading SCD41 sensor ready status failed: ");
    sensor.errorText(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  if (!ready) {
    return;
  }

  Measurement measurement;
  error = sensor.readMeasurement(measurement);
  if (error)
  {
    char errorMessage[256];
    Serial.print("Reading SCD41 sensor measurement failed: ");
    sensor.errorText(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else
  {
    Serial.print("Sensor 0x");
    Serial.print(sensor.getSerialNumber(), HEX);
    Serial.print(" data: ");
    Serial.print("CO2=");
    Serial.print(measurement.co2);
    Serial.print(", ");
    Serial.print("temp=");
    Serial.print(measurement.temperature);
    Serial.print(", ");
    Serial.print("hum=");
    Serial.println(measurement.humidity);
  }
}

void loop()
{
  builtInLed.pulse(2000);
  printSensorMeasurement();
}