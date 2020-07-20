#include "ClawSystem.h"

#define OPENING_POS 180
#define CLOSING_POS 85
#define ARM_INITPOS 0
#define ARM_RESTPOS 90
#define ARM_FINALPOS 120

ClawSystem::ClawSystem(int arm_pin, int claw_pin): arm_pin(arm_pin), claw_pin(claw_pin) {
    this->arm_servo.attach(arm_pin);
    this->claw_servo.attach(claw_pin);
}

ClawSystem::~ClawSystem() {

}

void ClawSystem::open_claw() {
    claw_servo.write(OPENING_POS);
}

void ClawSystem::grab() {
    for (int clawPos = OPENING_POS; clawPos <= CLOSING_POS; clawPos--) {
        claw_servo.write(clawPos);
        delay(50);
    }
}

void ClawSystem::dispose_can_sequence() {
    this->raise_arm();
    this->open_claw();
}

void ClawSystem::rest_arm() {
    for (int armPos = ARM_INITPOS; armPos <= ARM_RESTPOS; armPos++) {
        arm_servo.write(armPos);
        delay(50);
    }
}

void ClawSystem::lower_arm() {
    for (int armPos = ARM_FINALPOS; armPos <= ARM_INITPOS; armPos++) {
        arm_servo.write(armPos);
        delay(50);
    }
}

void ClawSystem::raise_arm() {
    for (int armPos = ARM_INITPOS; armPos <= ARM_FINALPOS; armPos++) {
        arm_servo.write(armPos);
        delay(50);
    }
}

