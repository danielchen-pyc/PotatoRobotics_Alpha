#include "SonarSystem.h"

#define MAX_DISTANCE 200

SonarSystem::SonarSystem(int left_trigger, int left_echo, int front_trigger, int front_echo, int right_trigger, int right_echo) {
    SonarSystem::sonar_left = new NewPing(left_trigger, left_echo, MAX_DISTANCE);
    SonarSystem::sonar_front = new NewPing(front_trigger, front_echo, MAX_DISTANCE);
    SonarSystem::sonar_right = new NewPing(right_trigger, right_echo, MAX_DISTANCE);
}

SonarSystem::~SonarSystem() {

}

unsigned int SonarSystem::getFrontDistance() {
    return sonar_front->ping_cm();
}

unsigned int SonarSystem::getLeftDistance() {
    return sonar_left->ping_cm();
}

unsigned int SonarSystem::getRightDistance() {
    return sonar_right->ping_cm();
}


unsigned int SonarSystem::getFrontDistance_accurate() {
    return sonar_front->convert_cm(sonar_front->ping_median(3));
}

unsigned int SonarSystem::getLeftDistance_accurate() {
    return sonar_left->convert_cm(sonar_left->ping_median(3));
}

unsigned int SonarSystem::getRightDistance_accurate() {
    return sonar_right->convert_cm(sonar_left->ping_median(3));
}

