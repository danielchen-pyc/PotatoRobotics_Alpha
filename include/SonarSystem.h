//
// Created by Daniel Chen on 2020-07-20
// 

#ifndef SONARSYSTEM_H
#define SONARSYSTEM_H

#include "NewPing.h"

class SonarSystem
{
private:
    NewPing* sonar_left;
    NewPing* sonar_front;
    NewPing* sonar_right;
    
public:
    SonarSystem(int left_trigger, int left_echo, int front_trigger, int front_echo, int right_trigger, int right_echo);
    ~SonarSystem();
    int getFrontDistance();
    int getLeftDistance();
    int getRightDistance();
};

#endif