//
// Created by Daniel Chen on 2020-06-30
// 

#ifndef PWMACTUATOR_H
#define PWMACTUATOR_H

#include <Arduino.h>

class PwmActuator 
{
protected:
    const int clock_freq;
    PwmActuator(int clock_freq);
    virtual ~PwmActuator();
    void init(PinName pin);
    void write(PinName pin, int speed);
    void calibrate(int speed);
};

#endif