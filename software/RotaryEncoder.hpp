#pragma once

#include "pico/stdlib.h"
#include <stdio.h>

#define ENCODER_A_PIN 13
#define ENCODER_B_PIN 14
#define ENCODER_SW_PIN 15

#define DEBOUNCE_MS 100

class RotaryEncoder;

static RotaryEncoder *g_encoder = nullptr; // Ugly trick to get (the only) RotaryEncoder object to run the interupt

enum class RotDirection
{
    LEFT,
    RIGHT,
    CLICK
};

class RotaryEncoder
{
private:
    absolute_time_t last_interupt = {0};

    volatile bool last_A = false;          // Last known state of encoder pin A
    volatile bool last_B = false;          // Last known state of encoder pin B
    volatile int32_t encoder_position = 0; // (Unused currently, can be used for absolute pos)
    volatile int8_t selectedDecimal;       // Index of currently selected digit
    int values[4];                         // Array holding 4 digits of the value

public:
    RotaryEncoder(int newValues[4], int newSelectedDecimal)
        : selectedDecimal(newSelectedDecimal)
    {
        for (int i = 0; i < 4; ++i)
        {
            values[i] = newValues[i];
        }
        g_encoder = this;

        // GPIO setup
        gpio_init(ENCODER_A_PIN);
        gpio_set_dir(ENCODER_A_PIN, GPIO_IN);
        gpio_pull_up(ENCODER_A_PIN);

        gpio_init(ENCODER_B_PIN);
        gpio_set_dir(ENCODER_B_PIN, GPIO_IN);
        gpio_pull_up(ENCODER_B_PIN);

        gpio_init(ENCODER_SW_PIN);
        gpio_set_dir(ENCODER_SW_PIN, GPIO_IN);
        gpio_pull_up(ENCODER_SW_PIN);

        // printf("Setting interrupt");

        irq_set_enabled(IO_IRQ_BANK0, true);
        gpio_set_irq_callback(RotaryEncoder::interruptTriggered);
        gpio_set_irq_enabled(ENCODER_A_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
        gpio_set_irq_enabled(ENCODER_B_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
        gpio_set_irq_enabled(ENCODER_SW_PIN, GPIO_IRQ_EDGE_FALL, true);
    }
    absolute_time_t getLastInterupt()
    {
        return last_interupt;
    }

    bool interuptIsDebounced()
    {
        absolute_time_t now = get_absolute_time();
        bool valid = absolute_time_diff_us(last_interupt, now) > DEBOUNCE_MS * 1000;
        last_interupt = now;
        return valid;
    }
    void expressYourself()
    {
        printf("rot pos: %d\n", encoder_position);
    }
    void handleRotation(RotDirection rotDirection)
    {
        if (!interuptIsDebounced())
        {
            return;
        }

        switch (rotDirection)
        {
        case RotDirection::LEFT:
            values[selectedDecimal]--;
            encoder_position--;
            printf("Left Rotation");
            break;

        case RotDirection::RIGHT:
            values[selectedDecimal]++;
            encoder_position++;
            printf("Right Rotation");
            break;
        case RotDirection::CLICK:
            selectedDecimal++;
            if (selectedDecimal >= 4)
                selectedDecimal = 0;
            break;
        default:
            break;
        }
    }

    void checkValues()
    {
        constexpr int maxValues[4] = {1, 9, 9, 9};
        constexpr int minValue = 0;

        // Clamp each digit between min and max values
        for (int i = 0; i < 4; ++i)
        {
            if (values[i] < minValue)
                values[i] = minValue;
            else if (values[i] > maxValues[i])
                values[i] = maxValues[i];
        }

        // Calculate total numeric value from digits
        float total = values[0] * 10.0 + values[1] + values[2] * 0.1 + values[3] * 0.01;

        // Clamp total max to 18.00
        if (total > 18.00)
        {
            values[0] = 1;
            values[1] = 8;
            values[2] = 0;
            values[3] = 0;
        }
    }

    // Static ISR trampoline to call instance method
    static void interruptTriggered(uint gpio, uint32_t events)
    {
        // printf("interrupt triggered, gpio:%d\n\n\n", gpio);

        if (g_encoder)
            g_encoder->handleInterrupt(gpio, events);
    }
    // Actual interrupt handler updating state and calling rotation logic
    void handleInterrupt(uint gpio, uint32_t events)
    {

        bool A = gpio_get(ENCODER_A_PIN);
        bool B = gpio_get(ENCODER_B_PIN);

        if ((gpio == ENCODER_A_PIN && A != last_A) || (gpio == ENCODER_B_PIN && B != last_B))
        {
            bool directionLeft = (gpio == ENCODER_A_PIN) ? (A == B) : (A != B);

            handleRotation(directionLeft ? RotDirection::LEFT : RotDirection::RIGHT);

            if (gpio == ENCODER_A_PIN)
                last_A = A;
            else
                last_B = B;
        }
        else if (gpio == ENCODER_SW_PIN)
        {
            handleRotation(RotDirection::CLICK);
        }
    }

    const int *getValues()
    {
        checkValues();
        return values;
    }

    int8_t getSelectedDecimal() const
    {
        return selectedDecimal;
    }
};