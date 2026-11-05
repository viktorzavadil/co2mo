#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "built_in_led.h"

BuiltInLed::BuiltInLed(int16_t pin) : pin(pin)
{
    pixels = Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ800);
}

void BuiltInLed::begin()
{
    pixels.begin();
    pixels.setBrightness(10);
}

void BuiltInLed::pulse(uint16_t period)
{
    uint8_t phase = (millis() % period) * 256 / period;
    uint8_t level = pixels.sine8(phase);
    pixels.setPixelColor(0, pixels.Color(0, 0, level));
    pixels.show();
}