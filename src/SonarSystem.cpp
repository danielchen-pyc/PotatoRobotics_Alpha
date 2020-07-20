#include "SonarSystem.h"

#define MAX_DISTANCE 200

SonarSystem::SonarSystem(int TRIGGER_PIN_LEFT, int ECHO_PIN_LEFT, int TRIGGER_PIN_FRONT, int ECHO_PIN_FRONT, int TRIGGER_PIN_RIGHT, int ECHO_PIN_RIGHT) {
    sonar_left = NewPing(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
    sonar_front = NewPing(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE);
    sonar_right = NewPing(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);
}

SonarSystem::~SonarSystem() {

}

int SonarSystem::getFrontDistance() {
    return sonar_front.ping_cm();
}

int SonarSystem::getLeftDistance() {
    return sonar_left.ping_cm();
}

int SonarSystem::getRightDistance() {
    return sonar_right.ping_cm();
}

