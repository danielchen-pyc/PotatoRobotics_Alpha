#include "PwmActuator.h"

PwmActuator::PwmActuator(int clock_freq): clock_freq(clock_freq) {

}

void PwmActuator::init(PinName pin) {
    pwm_start(pin, clock_freq, 20, PERCENT_COMPARE_FORMAT); // The motor vibrates but not spin at speed=20%
    
}

void PwmActuator::write(PinName pin, int speed) {
    pwm_start(pin, clock_freq, speed, PERCENT_COMPARE_FORMAT);
}
