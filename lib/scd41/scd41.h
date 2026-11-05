#pragma once
#include <Arduino.h>

#define I2C_ADDRES 0x62

struct Measurement {
    uint16_t co2;
    float temperature;
    float humidity;
};

class Scd41
{
public:
    explicit Scd41(float temperatureOffset, uint16_t altitude);
    uint16_t begin(int sda, int scl);
    uint64_t getSerialNumber();
    uint16_t readMeasurement(Measurement& measurement);
    uint16_t getDataReadyStatus(bool &ready);
    void errorText(uint16_t error, char *buf, size_t size);

private:
    float temperatureOffset = 0;
    uint16_t altitude = 0;
    uint64_t serialNumber;
};