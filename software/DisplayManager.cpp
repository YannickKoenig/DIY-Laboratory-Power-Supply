#include "DisplayManager.hpp"
#define SEGMENT_DOT 0x80
#define TIMEOUT_US 2500000 // 2.5 seconds
#define BLINK_CYCLE_LENGTH 3

DisplayManager::DisplayManager() : voltageDisplay(CLK_Display1, DIO_Display1, 80, 4), amperageDisplay(CLK_Display2, DIO_Display2, 80, 4)
{
    voltageDisplay.displayBegin();
    voltageDisplay.setBrightness(5, true);

    amperageDisplay.displayBegin();
    amperageDisplay.setBrightness(5, true);

    gpio_set_function(CLK_Display1, GPIO_FUNC_SIO);
    gpio_set_function(DIO_Display1, GPIO_FUNC_SIO);
    gpio_set_dir(CLK_Display1, GPIO_OUT);
    gpio_set_dir(DIO_Display1, GPIO_OUT);
    gpio_set_function(CLK_Display2, GPIO_FUNC_SIO);
    gpio_set_function(DIO_Display2, GPIO_FUNC_SIO);
    gpio_set_dir(CLK_Display2, GPIO_OUT);
    gpio_set_dir(DIO_Display2, GPIO_OUT);
}

void DisplayManager::update(const int *newValues, int selectedPosition)
{
    for (int i = 0; i < 4; ++i)
    {
        values[i] = newValues[i];
    }
    blinkPosition = selectedPosition;
}

bool DisplayManager::isBlinkingPeriod()
{
    return absolute_time_diff_us(last_updated, get_absolute_time()) <= TIMEOUT_US;
}

void DisplayManager::show()
{
    for (int i = 0; i < 4; ++i)
    {
        data[i] = voltageDisplay.encodeCharacter('0' + values[i]);
    }

    data[1] = voltageDisplay.encodeCharacter('0' + values[1]) | SEGMENT_DOT; // Set point

    if (values[0] == 0) // Hide leading zero
        data[0] = EMPTY_SEGMENT;

    if (isBlinkingPeriod()) // //TODO: Add enums
    {
        if (cycle == 0)
        {
            data[blinkPosition] = EMPTY_SEGMENT; // make blinking position blank
        }
        cycle = (cycle + 1) % BLINK_CYCLE_LENGTH;
    }

    voltageDisplay.setSegments(data, 4, 0);
}
void DisplayManager::setLastEdited(absolute_time_t timestamp)
{
    last_updated = timestamp;
}

void DisplayManager::setAmperage(float amp)
{
    char buffer[8];
    uint8_t data[4] = {EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT};
    // Convert float to string
    snprintf(buffer, sizeof(buffer), "%.3f", amp); // %.3f formats to 3 decimal places

    int index = 1;
    int bufIndex = 0;
    while (bufIndex < 5)
    {
        if (buffer[bufIndex] == '.')
        {
            bufIndex++;
            continue;
        }
        data[index] = amperageDisplay.encodeCharacter(buffer[bufIndex]);
        index++;
        bufIndex++;
    }
    data[0] = amperageDisplay.encodeCharacter('0');
    data[1] = data[1] | SEGMENT_DOT; // Set point
    amperageDisplay.setSegments(data, 4, 0);
}