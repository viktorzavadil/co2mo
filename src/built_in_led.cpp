#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "built_in_led.h"

BuiltInLed::BuiltInLed(int16_t pin) : pin(pin)
{
    pixels = Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ800);
}

void BuiltInLed::begin(uint8_t brightness)
{
    pixels.begin();
    pixels.setBrightness(brightness);
}

void BuiltInLed::pulse(uint16_t period, uint8_t mode)
{
    uint8_t phase = (millis() % period) * 256 / period;
    uint32_t color;
    switch (mode)
    {
    case 0:
        color = pixels.Color(pixels.sine8(phase), 0, 0);
        break;

    case 1:
        color = pixels.Color(0, pixels.sine8(phase), 0);
        break;

    default:
        color = pixels.Color(0, 0, pixels.sine8(phase));
        break;
    }

    uint8_t level = pixels.sine8(phase);
    pixels.setPixelColor(0, color);
    pixels.show();
}

void BuiltInLed::switchOff()
{
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
}