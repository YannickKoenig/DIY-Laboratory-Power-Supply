#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "RegulatorManager.hpp"

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

double RegulatorManager::readVoltage()
{
    adc_select_input(voltage_ADC);
    double voltage = ((double)adc_read() / ADC_MAX) * V_REF; // You could use an external regulated reference voltage for increased precision
    double real_voltage = voltage * ((R1 + R2) / R2);
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

void RegulatorManager::setTargetVoltage(double newTarget)
{
    targetVoltage = newTarget;
}

void RegulatorManager::update(double loopTime_ms)
{
    currentVoltage = readVoltage();
    currentAmperage = getAmperage();
    double error = targetVoltage - currentVoltage;
    integral += error * loopTime_ms * 0.001;
    duty_cycle = K_p * error + K_i * integral;
    if (duty_cycle > 1.0)
        duty_cycle = 1.0;
    if (duty_cycle < 0.0 || targetVoltage == 0)
        duty_cycle = 0.0;

    pwm_set_duty_cycle(duty_cycle);
}

double RegulatorManager::getAmperage()
{
    adc_select_input(amperage_ADC);
    double amperage = ((double)adc_read() / ADC_MAX) / R_SHUNT;
    return amperage;
}
