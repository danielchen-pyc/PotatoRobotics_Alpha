#include "ClawSystem.h"
#include "SonarSystem.h"

#define OPENING_POS 180
#define CLOSING_POS 85
#define ARM_INITPOS 0
#define ARM_RESTPOS 90
#define ARM_FINALPOS 120
#define CHECK_ARM_POS 45

ClawSystem::ClawSystem(int arm_pin, int claw_pin): arm_pin(arm_pin), claw_pin(claw_pin) {
    this->arm_servo.attach(arm_pin);
    this->claw_servo.attach(claw_pin);
    this->rest_arm();
    this->close_claw();
}

ClawSystem::~ClawSystem() {

}

void ClawSystem::open_claw() {
    this->claw_servo.write(OPENING_POS);
}

void ClawSystem::grab() {
    for (int clawPos = OPENING_POS; clawPos <= CLOSING_POS; clawPos--) {
        this->claw_servo.write(clawPos);
        delay(50);
    }
}

void ClawSystem::close_claw() {
    this->claw_servo.write(CLOSING_POS);
}

void ClawSystem::dispose_can_sequence() {
    this->raise_arm();
    this->open_claw();
}

void ClawSystem::check_can_sequence() {
    this->raise_arm(CHECK_ARM_POS);
    this->arm_servo.detach();
    if (SonarSystem::getFrontDistance() > 15) {
        // didn't successfully grabbed can -> regrab it
        this->open_claw();
        this->lower_arm();
        this->grab();
        this->check_can_sequence();
    }
}

void ClawSystem::rest_arm() {
    for (int armPos = ARM_INITPOS; armPos <= ARM_RESTPOS; armPos++) {
        this->arm_servo.write(armPos);
        delay(50);
    }
}

void ClawSystem::lower_arm() {
    for (int armPos = ARM_FINALPOS; armPos <= ARM_INITPOS; armPos++) {
        this->arm_servo.write(armPos);
        delay(50);
    }
}

void ClawSystem::raise_arm() {
    for (int armPos = ARM_INITPOS; armPos <= ARM_FINALPOS; armPos++) {
        this->arm_servo.write(armPos);
        delay(50);
    }
}
