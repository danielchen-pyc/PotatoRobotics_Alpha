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
    void actuate();


    // Actions
    void stop(int duration);
    void stop();
    void forward_fast();
    void forward_med();
    void forward_slow();

    void left_bit();
    void right_bit();
    void left_more();
    void right_more();
    void left_lot();
    void right_lot();

    void rotate_left();
    void rotate_right();
    void rotate_left_fast();
    void rotate_right_fast();

    void reverse();
    void escapeCorner();
};

#endif