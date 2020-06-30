#ifndef DRIVESYSTEM_H
#define DRIVESYSTEM_H

#include "Motor.hpp"

class DriveSystem
{
private:
    Motor Left;
    Motor Right;

public:
    DriveSystem(PinName right_forward, PinName right_backward, PinName left_forward, PinName left_backward);
    virtual ~DriveSystem();
    void init();
    void update();
};

#endif