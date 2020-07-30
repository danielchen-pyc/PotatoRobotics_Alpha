#include "DriveSystem.h"

#define FAST 100
#define MEDIUM 42
#define SLOW 39
#define A_BIT 4
#define MORE 6
#define A_LOT 6.8
#define ROTATE_SPEED 39
#define SLOW_FACTOR 0.2


// #define MEDIUM 26
// #define SLOW 24
// #define A_BIT 5
// #define MORE 10
// #define A_LOT 14

#define RIGHT_CALIBRATION 0
#define LEFT_CALIBRATION 0

DriveSystem::DriveSystem(PinName left_forward, PinName left_backward, PinName right_forward, PinName right_backward, int clock_freq)
: Right(right_forward, right_backward, clock_freq), Left(left_forward, left_backward, clock_freq) {
    this->speed = 0;
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::init() {
    this->Right.init();
    this->Left.init();
}

int DriveSystem::getLeftSpeed() {
    return this->Left.getSpeed();
}

int DriveSystem::getRightSpeed() {
    return this->Right.getSpeed();
}

void DriveSystem::update(int left_speed, int right_speed) {
    this->Left.update(left_speed + LEFT_CALIBRATION);
    this->Right.update(right_speed + RIGHT_CALIBRATION);
}

void DriveSystem::stop(int duration) {
    this->Right.init();
    this->Left.init();
    delay(duration);
}

void DriveSystem::stop() {
    this->init();
}

void DriveSystem::actuate() {
    this->Left.actuate();
    this->Right.actuate();
}


// Moving Actions 

void DriveSystem::forward_fast() {
    this->update(FAST, FAST);
}

void DriveSystem::forward_med() {
    this->update(MEDIUM, MEDIUM);
}

void DriveSystem::forward_slow() {
    this->update(SLOW, SLOW);
}

void DriveSystem::left_bit() {
    this->update(SLOW - SLOW_FACTOR * A_BIT, SLOW + A_BIT);
}

void DriveSystem::right_bit() {
    this->update(SLOW + A_BIT, SLOW - SLOW_FACTOR * A_BIT);
}

void DriveSystem::left_more() {
    this->update(SLOW - SLOW_FACTOR * MORE, SLOW + MORE);
}

void DriveSystem::right_more() {
    this->update(SLOW + MORE, SLOW - SLOW_FACTOR * MORE);
}

void DriveSystem::left_lot() {
    this->update(SLOW - SLOW_FACTOR * A_LOT, SLOW + A_LOT);
}

void DriveSystem::right_lot() {
    this->update(SLOW + A_LOT, SLOW - SLOW_FACTOR * A_LOT);
}

void DriveSystem::rotate_left() {
    this->update(0, ROTATE_SPEED);
}

void DriveSystem::rotate_right() {
    this->update(ROTATE_SPEED, 0);
}

void DriveSystem::reverse() {
    this->update(-MEDIUM, -MEDIUM);
}

void DriveSystem::escapeCorner() {
    // TODO: This method need to be tested a few times and modified
    this->reverse();
    delay(1000);
    this->update(SLOW, -SLOW);
    delay(300);
    this->init();
}