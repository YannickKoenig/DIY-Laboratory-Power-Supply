#pragma once

#include "pico/stdlib.h"
#include "displaylib_LED_PICO/include/displaylib_LED_PICO/tm1637.hpp"

#define CLK_Display1 9
#define DIO_Display1 6

#define CLK_Display2 3
#define DIO_Display2 4

#define EMPTY_SEGMENT (0xff)

class DisplayManager
{
private:
    // Voltage Display
    TM1637plus_model4 voltageDisplay; // CLK_Display1, DIO_Display1, 80, 4
    int blinkPosition = 1;

    absolute_time_t last_updated;

    int values[4];                                                                  // Decimal Array
    uint8_t data[4] = {EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT}; // Empty Display
    int cycle = 0;                                                                  // TODO: Refactor

    // Amperage Display
    TM1637plus_model4 amperageDisplay;

public:
    DisplayManager();
    void update(const int *newValues, int selectedPosition);
    void setAmperage(double amp);
    void show();
    void setLastEdited(absolute_time_t timestamp);
    bool isBlinkingPeriod();
};
