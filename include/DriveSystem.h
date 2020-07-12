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
    DriveSystem(PinName left_forward, PinName left_backward, PinName right_forward, PinName right_backward, int clock_freq);
    virtual ~DriveSystem();
    void init();
    int getLeftSpeed();
    int getRightSpeed();
    void update(int left_speed, int right_speed);
    void stop(int duration);
    void forward_fast();
    void forward_med();
    void forward_slow();
    void reverse();
    void escapeCorner();
    void actuate();
};

#endif