#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

constexpr long PULSES_PAUSE = 5000;

constexpr uint8_t MIN_PULSE_BRIGHTNESS = 10;
constexpr uint8_t MAX_PULSE_BRIGHTNESS = 50;

enum class Indicator
{
    Error = 0,          // red
    Temperature = 5500, // orange
    Humidity = 43690,   // blue
    CO2 = 21845         // green
};

class BuiltInLed
{
public:
    explicit BuiltInLed(int16_t pin);
    void begin();
    void pulse(uint16_t period, Indicator indicator);
    void startPulses(uint8_t count, uint16_t period, Indicator indicator);
    void updatePulses();
    void resetPulses();
    bool isPulsate();
    void switchOff();

private:
    int16_t pin;
    Adafruit_NeoPixel pixels;

    uint8_t pulsesLeft = 0;
    uint32_t pulseStart = 0;
    uint16_t pulsePeriod = 0;
    uint8_t pulseLastPhase = 0;
    Indicator pulseIndicator;

    uint32_t colorOf(Indicator indicator, uint8_t saturation, uint8_t brightness);
    uint8_t pulseLevel(uint8_t phase);
};