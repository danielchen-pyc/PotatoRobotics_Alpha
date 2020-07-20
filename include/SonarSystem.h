//
// Created by Daniel Chen on 2020-07-20
// 

#ifndef SONARSYSTEM_H
#define SONARSYSTEM_H

#include "NewPing.h"

class SonarSystem
{
private:
    static NewPing* sonar_left;
    static NewPing* sonar_front;
    static NewPing* sonar_right;
public:
    SonarSystem(int TRIGGER_PIN_LEFT, int ECHO_PIN_LEFT, int TRIGGER_PIN_FRONT, int ECHO_PIN_FRONT, int TRIGGER_PIN_RIGHT, int ECHO_PIN_RIGHT);
    virtual ~SonarSystem();
    static int getFrontDistance();
    static int getLeftDistance();
    static int getRightDistance();
    
};

#endif