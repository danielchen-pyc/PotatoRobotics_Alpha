
#include "Motor.h"
#include <Arduino.h>

Motor::Motor(PinName forward, PinName backward, int clock_freq): PwmActuator::PwmActuator(clock_freq) {
    this->forward = forward;
    this->backward = backward;
}

Motor::~Motor() {

}

int Motor::getSpeed() {
    return this->speed;
}

void Motor::init() {
    this->speed = 20;
    this->isForward = true;
    PwmActuator::init(this->forward); // we only have to init (make vibrate) one motor
    pinMode(this->forward, OUTPUT);
    pinMode(this->backward, OUTPUT);
}

void Motor::update(int speed) {
    if (speed >= 0) {
        isForward = true;
        PwmActuator::write(this->forward, speed);
    } else {
        isForward = false;
        PwmActuator::write(this->backward, -speed);
    }

    this->speed = speed;
}

void Motor::stop(int duration) {
    PwmActuator::write(this->forward, 0);
    PwmActuator::write(this->backward, 0);
    delay(duration);
    PwmActuator::init(this->forward);
}

void Motor::actuate() {
    if (isForward) {
        PwmActuator::write(forward, this->speed);
        PwmActuator::write(backward, 0);
    } else {
        PwmActuator::write(forward, 0);
        PwmActuator::write(backward, this->speed);
    }
}