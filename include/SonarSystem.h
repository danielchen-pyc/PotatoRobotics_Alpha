//
// Created by Daniel Chen on 2020-07-20
// 

#ifndef SONARSYSTEM_H
#define SONARSYSTEM_H

#include "NewPing.h"

class SonarSystem
{
private:
    NewPing sonar_left;
    NewPing sonar_front;
    NewPing sonar_right;
public:
    SonarSystem(int TRIGGER_PIN_LEFT, int ECHO_PIN_LEFT, int TRIGGER_PIN_FRONT, int ECHO_PIN_FRONT, int TRIGGER_PIN_RIGHT, int ECHO_PIN_RIGHT);
    virtual ~SonarSystem();
    int getFrontDistance();
    int getLeftDistance();
    int getRightDistance();
    
};

#endif