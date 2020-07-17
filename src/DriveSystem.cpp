#include "DriveSystem.h"

#define FAST 100
#define MEDIUM 75
#define SLOW 64
#define A_BIT 6
#define MORE 10
#define A_LOT 13
#define RIGHT_CALIBRATION 7

DriveSystem::DriveSystem(PinName left_forward, PinName left_backward, PinName right_forward, PinName right_backward, int clock_freq)
: Right(right_forward, right_backward, clock_freq), Left(left_forward, left_backward, clock_freq) {
    this->speed = 0;
}

DriveSystem::~DriveSystem() {

}

void DriveSystem::init() {
    this->Left.init();
    this->Right.init();
}

int DriveSystem::getLeftSpeed() {
    return this->Left.getSpeed();
}

int DriveSystem::getRightSpeed() {
    return this->Right.getSpeed();
}

void DriveSystem::update(int left_speed, int right_speed) {
    this->Left.update(left_speed);
    this->Right.update(right_speed + RIGHT_CALIBRATION);
}

void DriveSystem::stop(int duration) {
    this->Left.stop(duration);
    this->Right.stop(duration);
}


// Actions 

void DriveSystem::forward_fast() {
    this->update(FAST, FAST);
}

void DriveSystem::forward_med() {
    this->update(MEDIUM, MEDIUM);
}

void DriveSystem::forward_slow() {
    this->update(SLOW, SLOW);
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

void DriveSystem::actuate() {
    this->Left.actuate();
    this->Right.actuate();
}

void DriveSystem::left_bit() {
    this->update(SLOW - A_BIT, SLOW + A_BIT);
}

void DriveSystem::right_bit() {
    this->update(SLOW + A_BIT, SLOW - A_BIT);
}

void DriveSystem::left_more() {
    this->update(SLOW - MORE, SLOW + MORE);
}

void DriveSystem::right_more() {
    this->update(SLOW + MORE, SLOW - MORE);
}

void DriveSystem::left_lot() {
    this->update(SLOW - A_LOT, SLOW + A_LOT);
}

void DriveSystem::right_lot() {
    this->update(SLOW + A_LOT, SLOW - A_LOT);
}