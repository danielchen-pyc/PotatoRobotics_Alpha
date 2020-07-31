//
// Created by Daniel Chen on 2020-07-19
// 

#ifndef CLAWSYSTEM_H
#define CLAWSYSTEM_H

#include <Arduino.h>
#include <Servo.h>
#include "SonarSystem.h"

class ClawSystem
{
private:
    int arm_pin;
    int claw_pin;
    Servo arm_servo;
    Servo claw_servo;
    String currentPos;

public:
    ClawSystem(int arm_pin, int claw_pin);
    virtual ~ClawSystem();
    void init();
    void disconnect();
    void disconnect_arm();
    void open_claw();
    void close_claw();
    void grab();
    void grab_can_sequence();
    void dispose_can_sequence();
    void check_can_sequence(SonarSystem &sonarsystem);
    void rest_arm();
    void lower_arm();
    void raise_arm();
    void raise_arm(int final_pos);
};

#endif