#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class BuiltInLed
{
public:
    explicit BuiltInLed(int16_t pin);
    void begin();
    void pulse(uint16_t period);

private:
    int16_t pin;
    Adafruit_NeoPixel pixels;
};