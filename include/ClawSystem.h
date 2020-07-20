//
// Created by Daniel Chen on 2020-07-19
// 

#ifndef CLAWSYSTEM_H
#define CLAWSYSTEM_H

#include <Arduino.h>
#include <Servo.h>

class ClawSystem
{
private:
    int arm_pin;
    int claw_pin;
    Servo arm_servo;
    Servo claw_servo;

public:
    ClawSystem(int armpin, int claw_pin);
    virtual ~ClawSystem();
    void open_claw();
    void grab();
    void dispose_can_sequence();
    void rest_arm();
    void lower_arm();
    void raise_arm();
};

#endif