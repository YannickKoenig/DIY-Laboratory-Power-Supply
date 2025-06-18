#include "pico/stdlib.h"
#include "displaylib_LED_PICO/include/displaylib_LED_PICO/tm1637.hpp"
#include "DisplayManager.hpp"
#include "RotaryEncoder.hpp"
#include "RegulatorManager.hpp"

#define DISPLAY_REFRESH_RATE_MS 255

DisplayManager *displayManager;
RotaryEncoder *rotaryEncoder;
RegulatorManager *regulatorManager;
absolute_time_t lastDisplayRefresh = {0};

void setup(void);
void loop(void);

int main()
{
    setup();
    loop();
    return 0;
}

void setup(void)
{
    stdio_init_all(); // Initialize USB Serial

    int values[] = {0, 5, 0, 0}; // Default Voltage: 5.00 V

    displayManager = new DisplayManager();
    rotaryEncoder = new RotaryEncoder(values, 1);
    regulatorManager = new RegulatorManager();
    regulatorManager->setTargetVoltage(5.0);
}

double calculateTargetVoltage(const int *values)
{
    return values[0] * 10.0 + values[1] * 1.0 + values[2] * 0.1 + values[3] * 0.01;
}

bool displayRefreshDue()
{
    return absolute_time_diff_us(lastDisplayRefresh, get_absolute_time()) > DISPLAY_REFRESH_RATE_MS * 1000;
}

void loop()
{
    while (true)
    {
        const int *values = rotaryEncoder->getValues();
        if (displayRefreshDue())
        {
            displayManager->update(values, rotaryEncoder->getSelectedDecimal());
            displayManager->setLastEdited(rotaryEncoder->getLastInterupt());
            lastDisplayRefresh = get_absolute_time();
            displayManager->show();
            displayManager->setAmperage(regulatorManager->getAmperage());
        }

        regulatorManager->setTargetVoltage(calculateTargetVoltage(values));
        regulatorManager->update(5);

        sleep_us(5 * 1000);
    }
}