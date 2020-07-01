#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "Motor.h"

class DriveSystem
{
private:
    Motor Left;
    Motor Right;
    int speed;

public:
    DriveSystem(PinName right_forward, PinName right_backward, PinName left_forward, PinName left_backward, int clock_freq);
    virtual ~DriveSystem();
    void init();
    void update(int left_speed, int right_speed);
    void stop(int duration);
    void forward();
    void reverse();
    void escapeCorner();
    void actuate();
};

#endif