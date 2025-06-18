#pragma once

#define PWM_pin 16
#define U_in_pin 26 // ADC0
#define I_in_pin 27 // ADC1

#define amperage_ADC 1
#define voltage_ADC 0

#define V_REF 3.3
#define ADC_MAX 4095.0

#define R1 51000.0
#define R2 10000.0
#define R_SHUNT 0.1

#define K_p 0.1
#define K_i 0.01

#define PWM_FREQUENCY 50000 // 50 kHz

class RegulatorManager
{
private:
    double targetVoltage = 0;
    double currentVoltage = 0;

    double currentAmperage = 0;
    double maxAmperage = 2;

    double integral = 0.0;
    double duty_cycle = 0.0;
    uint8_t slice_num;

public:
    RegulatorManager();

    double readVoltage();

    void pwm_init(uint32_t freq_hz);

    void pwm_set_duty_cycle(float duty_cycle);

    void setTargetVoltage(double newTarget);

    void update(double loopTime_ms);
    double getAmperage();
};