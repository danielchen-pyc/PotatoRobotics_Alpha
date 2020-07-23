#include "ClawSystem.h"
#include "SonarSystem.h"

#define OPENING_POS 180
#define CLOSING_POS 85
#define ARM_INITPOS 0
#define ARM_RESTPOS 90
#define ARM_FINALPOS 120
#define CHECK_ARM_POS 45

ClawSystem::ClawSystem(int arm_pin, int claw_pin): arm_pin(arm_pin), claw_pin(claw_pin) {
    // this->rest_arm();
    // delay(1000);
    // this->close_claw();
    // delay(1000);
}

ClawSystem::~ClawSystem() {

}

void ClawSystem::init() {
    pinMode(this->arm_pin, OUTPUT);
    pinMode(this->claw_pin, OUTPUT);  
    this->arm_servo.attach(this->arm_pin);
    this->claw_servo.attach(this->claw_pin);  
}

void ClawSystem::open_claw() {
    for (int servoPos = 90; servoPos >= 59; servoPos--) {
        claw_servo.write(servoPos);
        delay(30);
    }
    for (int servoPos2 = 59; servoPos2 <= 90; servoPos2++) {
        claw_servo.write(servoPos2);
        delay(30);
    }
    this->currentPos = "open";
}

void ClawSystem::grab() {
    for (int clawPos = 90; clawPos <= 128; clawPos++) {
        this->claw_servo.write(clawPos);
        delay(30);
    }
    for (int clawPos = 128; clawPos >= 90; clawPos--) {
        this->claw_servo.write(clawPos);
        delay(30);
    }
    this->currentPos = "close";
}

void ClawSystem::grab_can_sequence() {
    this->open_claw();
    delay(1000);
    this->lower_arm();
    delay(1000);
    this->grab();
    delay(1000);
}

void ClawSystem::close_claw() {
    this->claw_servo.write(CLOSING_POS);
}

void ClawSystem::dispose_can_sequence() {
    this->raise_arm();
    this->open_claw();
}

void ClawSystem::check_can_sequence(SonarSystem &sonarsystem) {
    this->raise_arm(CHECK_ARM_POS);
    // this->arm_servo.detach();
    if (sonarsystem.getFrontDistance() < 15) {
        // didn't successfully grabbed can -> regrab it
        this->open_claw();
        delay(1000);
        this->lower_arm();
        delay(1000);
        this->grab();
        delay(1000);
        // repeat checking sequence
        // this->check_can_sequence(sonarsystem);
    }
}

// TODO: Implement these methods
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

void ClawSystem::raise_arm(int final_pos) {
    for (int armPos = ARM_INITPOS; armPos <= final_pos; armPos++) {
        this->arm_servo.write(armPos);
        delay(50);
    }
}
