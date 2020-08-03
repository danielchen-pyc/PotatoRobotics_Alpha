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
    pinMode(this->arm_pin, OUTPUT);
    pinMode(this->claw_pin, OUTPUT);  
}

ClawSystem::~ClawSystem() {

}

void ClawSystem::init() {
    this->arm_servo.attach(this->arm_pin);
    this->claw_servo.attach(this->claw_pin);  
}

void ClawSystem::disconnect() {
    this->claw_servo.detach();
    this->arm_servo.detach();
}

void ClawSystem::disconnect_arm() {
    this->arm_servo.detach();
}



void ClawSystem::open_claw() {
    this->claw_servo.attach(this->claw_pin);  
    delay(200);
    for (int servoPos = 90; servoPos >= 59; servoPos--) {
        this->claw_servo.write(servoPos);
        delay(33);
    }
    for (int servoPos2 = 59; servoPos2 <= 90; servoPos2++) {
        this->claw_servo.write(servoPos2);
        delay(32);
    }
    this->currentPos = "open";
    this->claw_servo.detach();
}

void ClawSystem::grab() {
    this->claw_servo.attach(this->claw_pin);  
    delay(200);
    for (int clawPos = 90; clawPos <= 128; clawPos++) {
        this->claw_servo.write(clawPos);
        delay(31.5);
    }
    for (int clawPos = 128; clawPos >= 90; clawPos--) {
        this->claw_servo.write(clawPos);
        delay(31);
    }
    this->currentPos = "close";
    this->claw_servo.detach();
}

void ClawSystem::grab_can_sequence() {
    this->open_claw();
    delay(1000);
    this->lower_arm();
    delay(1000);
    this->grab();
    delay(1000);
    this->raise_arm();
    delay(1000);
}


void ClawSystem::dispose_can_sequence() {
    this->raise_arm();
    this->open_claw();
    this->grab();
    // delay(500);
    // this->disconnect_arm();
}

void ClawSystem::check_can_sequence(SonarSystem &sonarsystem) {
    delay(300);
    // this->arm_servo.detach();
    if (sonarsystem.getFrontDistance() < 10) {
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
    this->arm_servo.attach(arm_pin);
    delay(100); 
    for (int clawPos = 90; clawPos >= 79; clawPos--) {
        this->arm_servo.write(clawPos);
        delay(25);
    }
    for (int clawPos = 79; clawPos <= 90; clawPos++) {
        this->arm_servo.write(clawPos);
        delay(25);
    }
    this->arm_servo.detach();
}

void ClawSystem::raise_arm() {
    this->arm_servo.attach(arm_pin);
    delay(200); 
    for (int clawPos = 90; clawPos <= 108; clawPos++) {
        this->arm_servo.write(clawPos);
        delay(27);
    }
    for (int clawPos = 108; clawPos >= 90; clawPos--) {
        this->arm_servo.write(clawPos);
        delay(27);
    }
    this->arm_servo.detach();
}

void ClawSystem::raise_arm(int final_pos) {
    for (int armPos = ARM_INITPOS; armPos <= final_pos; armPos++) {
        this->arm_servo.write(armPos);
        delay(50);
    }
}
