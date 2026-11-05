#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "built_in_led.h"

BuiltInLed::BuiltInLed(int16_t pin) : pin(pin), pixels(1, pin, NEO_GRB + NEO_KHZ800) {}

void BuiltInLed::begin()
{
    pixels.begin();
    // pixels.setBrightness(brightness);
}

void BuiltInLed::pulse(uint16_t period, Indicator indicator)
{
    resetPulses();
    if (period == 0)
    {
        return;
    }

    uint8_t phase = (millis() % period) * 256 / period;
    uint8_t level = pulseLevel(phase);

    pixels.setPixelColor(0, colorOf(indicator, 255, level));
    pixels.show();
}

void BuiltInLed::startPulses(uint8_t count, uint16_t period, Indicator indicator)
{
    if (count == 0 || period == 0)
    {
        return;
    }

    pulseStart = millis();
    pulsePeriod = period;
    pulsesLeft = count;
    pulseLastPhase = 0;
    pulseIndicator = indicator;
}

void BuiltInLed::updatePulses()
{
    if (pulsesLeft == 0)
        return;

    uint8_t phase = ((millis() - pulseStart) % pulsePeriod) * 256 / pulsePeriod;

    if (phase < pulseLastPhase && --pulsesLeft == 0)
    {
        pixels.clear();
        pixels.show();
        return;
    }
    pulseLastPhase = phase;

    uint8_t level = pulseLevel(phase);

    pixels.setPixelColor(0, colorOf(pulseIndicator, 255, level));
    pixels.show();
}

bool BuiltInLed::isPulsate() {
    return pulsesLeft != 0;
}

void BuiltInLed::resetPulses()
{
    pulseStart = 0;
    pulsePeriod = 0;
    pulsesLeft = 0;
    pulseLastPhase = 0;
}

void BuiltInLed::switchOff()
{
    resetPulses();
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
}

uint32_t BuiltInLed::colorOf(Indicator indicator, uint8_t saturation, uint8_t brightness)
{
    return Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(static_cast<uint16_t>(indicator), saturation, brightness));
}

uint8_t BuiltInLed::pulseLevel(uint8_t phase)
{
    uint8_t wave = Adafruit_NeoPixel::sine8(phase + 192); // starts in dark
    return MIN_PULSE_BRIGHTNESS + (wave * (MAX_PULSE_BRIGHTNESS - MIN_PULSE_BRIGHTNESS)) / 255;
}