
#include "Motor.hpp"
#include <Arduino.h>

Motor::Motor(PinName forward, PinName backward, int clock_freq): PwmActuator::PwmActuator(clock_freq) {
    this->forward = forward;
    this->backward = backward;
}

void Motor::init() {
    PwmActuator::init(this->forward); // we only have to init (make vibrate) one motor
    pinMode(this->forward, OUTPUT);
    pinMode(this->backward, OUTPUT);
}

void Motor::update(int speed) {
    PinName updatePin;

    if (speed > 0) {
        updatePin = this->forward;
    } else {
        updatePin = this->backward;
    }
    
    PwmActuator::write(updatePin, speed);
}

void Motor::stop(int duration) {
    PwmActuator::write(this->forward, 0);
    PwmActuator::write(this->backward, 0);
    delay(duration);
    PwmActuator::init(this->forward);
}