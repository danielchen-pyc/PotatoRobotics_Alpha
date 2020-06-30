//
// Created by Daniel Chen on 2020-06-29
// 

#ifndef MOTOR_H
#define MOTOR_H

#include "PwmActuator.hpp"

class Motor: public PwmActuator
{
private:
    PinName forward;
    PinName backward;

public:
    Motor(PinName forward, PinName backward, int clock_freq);
    virtual ~Motor();
    void init();
    void update(int speed);
    void stop(int duration);
};

#endif