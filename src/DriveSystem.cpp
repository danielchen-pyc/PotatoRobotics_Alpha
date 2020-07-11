#include "DriveSystem.h"

#define STD_SPEED 80

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

void DriveSystem::update(int left_speed, int right_speed) {
    this->Left.update(left_speed);
    this->Right.update(right_speed);
}

void DriveSystem::stop(int duration) {
    this->Left.stop(duration);
    this->Right.stop(duration);
}

void DriveSystem::forward() {
    this->update(STD_SPEED, STD_SPEED);
}

void DriveSystem::reverse() {
    this->update(-STD_SPEED, -STD_SPEED);
}

void DriveSystem::escapeCorner() {
    // TODO: This method need to be tested a few times and modified
    this->reverse();
    delay(1000);
    this->update(STD_SPEED, -STD_SPEED);
    delay(300);
    this->init();
}

void DriveSystem::actuate() {
    this->Left.actuate();
    this->Right.actuate();
}