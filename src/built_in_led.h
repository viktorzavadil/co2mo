#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class BuiltInLed
{
public:
    explicit BuiltInLed(int16_t pin);
    void begin(uint8_t brightness);
    void pulse(uint16_t period, uint8_t mode);
    void switchOff();

private:
    int16_t pin;
    Adafruit_NeoPixel pixels;
};