#pragma once

#define PWM_pin 16
#define U_in_pin 26 // ADC0
#define I_in_pin 27 // ADC1

#define amperage_ADC 1
#define voltage_ADC 0

#define V_REF 3.3
#define ADC_MAX 4096.0

#define R1 51000.0
#define R2 10000.0
#define R_SHUNT 0.1

#define K_p 0.3
#define K_i 0.15

#define PWM_FREQUENCY 62 // 62 kHz

class RegulatorManager
{
private:
    float targetVoltage = 0;
    float currentVoltage = 0;

    float currentAmperage = 0;
    float maxAmperage = 2;

    float integral = 0.0;
    float duty_cycle = 0.0;
    uint8_t slice_num;

public:
    RegulatorManager();

    float readVoltage();

    void pwm_init(uint32_t freq_hz);

    void pwm_set_duty_cycle(float duty_cycle);

    void setTargetVoltage(float newTarget);

    void update(float loopTime_ms);
    float getAmperage();
};