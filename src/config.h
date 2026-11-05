#pragma once

// SCD41
#define PIN_SDA 10
#define PIN_SCL 8

constexpr float SENSOR_TEMPERATURE_OFFSET = 4.0f;
constexpr uint16_t SENSOR_ALTITUDE = 306;

constexpr uint16_t CO2_ALERT_THRESHOLD = 1000;

// WLED
#define PIN_LED_DATA    5
#define PIN_LED_CLK     4

// Built-in LED
#define PIN_BUILT_IN_LED     9
