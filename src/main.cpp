#include <Arduino.h>

#include <scd41.h>

#include "config.h"
#include "built_in_led.h"

Scd41 sensor(SENSOR_TEMPERATURE_OFFSET, SENSOR_ALTITUDE);
BuiltInLed builtInLed(PIN_BUILT_IN_LED);

constexpr Indicator ROTATION[] = {Indicator::CO2, Indicator::Temperature, Indicator::Humidity};
constexpr uint8_t ROTATION_SIZE = sizeof(ROTATION) / sizeof(ROTATION[0]);
constexpr uint16_t PULSE_PERIOD = 800;
uint8_t rotationIdx = 0;

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

uint8_t pulsesFor(Indicator indicator, const Measurement &m)
{
  switch (indicator)
  {
  case Indicator::CO2:
    return round(m.co2 / 100.0f);
  case Indicator::Temperature:
    return round(max(0.0f, m.temperature) / 10.0f);
  case Indicator::Humidity:
    return round(max(0.0f, m.humidity) / 10.0f);
  default:
    return 0;
  }
}

void showNextMetric(const Measurement &m)
{
  if (builtInLed.isPulsate())
  {
    return; // busy, rotation does not advance
  }

  Indicator indicator = ROTATION[rotationIdx];
  rotationIdx = (rotationIdx + 1) % ROTATION_SIZE;
  builtInLed.startPulses(pulsesFor(indicator, m), PULSE_PERIOD, indicator);
}

void processSensorMeasurement()
{
  bool ready;
  uint16_t error = sensor.getDataReadyStatus(ready);
  if (error)
  {
    char errorMessage[256];
    Serial.print("Reading SCD41 sensor ready status failed: ");
    sensor.errorText(error, errorMessage, 256);
    Serial.println(errorMessage);
    return;
  }
  if (!ready)
  {
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
    builtInLed.resetPulses();
    builtInLed.startPulses(5, PULSE_PERIOD, Indicator::Error);
    rotationIdx = 0;
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

    showNextMetric(measurement);
  }
}

void loop()
{
  builtInLed.updatePulses();
  processSensorMeasurement();
}