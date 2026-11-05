#include <Wire.h>
#include <SensirionI2cScd4x.h>

#include "scd41.h"

SensirionI2cScd4x scd4x;

Scd41::Scd41(float temperatureOffset, uint16_t altitude) : temperatureOffset(temperatureOffset), altitude(altitude) {}

uint16_t Scd41::begin(int sda, int scl)
{
  Wire.begin(sda, scl);

  scd4x.begin(Wire, I2C_ADDRES);

  scd4x.stopPeriodicMeasurement();

  scd4x.setTemperatureOffset(temperatureOffset);
  scd4x.setSensorAltitude(altitude);

  uint16_t error;
  error = scd4x.getSerialNumber(serialNumber);
  if (error)
  {
    return error;
  }

  return scd4x.startPeriodicMeasurement();
}

uint64_t Scd41::getSerialNumber()
{
  return serialNumber;
}

uint16_t Scd41::readMeasurement(Measurement &measurement)
{
  return scd4x.readMeasurement(measurement.co2, measurement.temperature, measurement.humidity);
}

uint16_t Scd41::getDataReadyStatus(bool &ready) {
  return scd4x.getDataReadyStatus(ready);
}

void Scd41::errorText(uint16_t error, char *buf, size_t size)
{
  errorToString(error, buf, size);
}
