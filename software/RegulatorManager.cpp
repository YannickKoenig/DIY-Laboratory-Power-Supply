#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "RegulatorManager.hpp"
#include <stdio.h>
RegulatorManager::RegulatorManager()
{

    // Get Slice number
    slice_num = pwm_gpio_to_slice_num(PWM_pin);

    // Initialize ADCs
    adc_init();
    adc_gpio_init(I_in_pin);
    adc_gpio_init(U_in_pin);

    // Initialize PWM
    pwm_init(PWM_FREQUENCY);
}

float RegulatorManager::readVoltage()
{
    adc_select_input(voltage_ADC);
    float voltage = ((float)adc_read() / ADC_MAX) * V_REF; // You could use an external regulated reference voltage for increased precision
    float real_voltage = voltage * ((R1 + R2) / R2);
    return real_voltage;
}

void RegulatorManager::pwm_init(uint32_t freq_hz)
{
    gpio_set_function(PWM_pin, GPIO_FUNC_PWM);
    uint32_t clock = 125000000; // Standard clock RP2040
    uint32_t wrap = clock / freq_hz - 1;
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
}

void RegulatorManager::pwm_set_duty_cycle(float duty_cycle)
{
    uint32_t wrap = pwm_hw->slice[slice_num].top; // Read wrap (TOP) value from hardware register
    uint32_t level = (uint32_t)((wrap + 1) * duty_cycle);
    pwm_set_gpio_level(PWM_pin, level);
}

void RegulatorManager::setTargetVoltage(float newTarget)
{
    targetVoltage = newTarget;
}

void RegulatorManager::update(float loopTime_us)
{

    currentVoltage = readVoltage();
    currentAmperage = getAmperage();
    float error = targetVoltage - currentVoltage;
    integral += error * loopTime_us * 0.001 * 0.001;
    duty_cycle = K_p * error + K_i * integral;
    if (duty_cycle > 1.0)
        duty_cycle = 1.0;
    if (duty_cycle < 0.0 || targetVoltage == 0)
        duty_cycle = 0.0;

    // duty_cycle = 1 - duty_cycle; // Inverting MOSFET driver

    pwm_set_duty_cycle(duty_cycle);
    /* printf("Current: %.3f\n", currentAmperage);
     printf("Voltage: %.3f\n", currentVoltage);
     printf("Error: %.3f\n", error);
     printf("Integral: %.3f\n", integral);
     printf("duty_cycle: %.3f\n%", duty_cycle);*/
    printf("%.3f,", currentVoltage);
    printf("%.3f\n", currentAmperage);
}

float RegulatorManager::getAmperage()
{
    adc_select_input(amperage_ADC);
    double amperage = (((double)adc_read() / ADC_MAX) * V_REF) / R_SHUNT;
    return amperage;
}
