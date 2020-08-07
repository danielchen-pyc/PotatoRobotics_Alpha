#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <cmath>
#include <Servo.h>
#include "DriveSystem.h"
#include "SonarSystem.h"
#include "ClawSystem.h"

using namespace std;


// FUNCTION PROTOTYPE
void activate_hook();
void calibrate_hook();
void print_tape_state(int lightvolt_left, int lightvolt_right);
void follow_tape();
void entertainment();
// TEST FUNCTION PROTOTYPE
void speedTest(String side);
void rotateTest(String direction);

// SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// TAPE FOLLOWING
#define RIGHT_TAPE_SENSOR PA6
#define LEFT_TAPE_SENSOR PA4
#define RIGHT_THRESHOLD_LIGHTVOLT 950
#define LEFT_THRESHOLD_LIGHTVOLT 160
#define ONTAPE_RECORD_NUM 10
#define OFFTAPELOTMAX 20
bool left_list[ONTAPE_RECORD_NUM];
bool right_list[ONTAPE_RECORD_NUM];
bool found_tape;
bool left_ontape;
bool left_prev;
bool right_ontape;
bool right_prev;
int off_tape_turn_lot_num;

// SONAR
#define TRIGGER_PIN_LEFT PB4 // sonar uses digital pins
#define ECHO_PIN_LEFT PB5
#define TRIGGER_PIN_FRONT PA15 
#define ECHO_PIN_FRONT PB3
#define TRIGGER_PIN_RIGHT PA11
#define ECHO_PIN_RIGHT PA12
SonarSystem sonarsystem(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT,  TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT);
#define LEFT_CAN_THRESHOLD 40
#define RIGHT_CAN_THRESHOLD 40
#define FRONT_CAN_THRESHOLD 30
#define GRABBING_THRESHOLD 9

// DRIVE SYSTEM
#define PWM_FREQ 2000
#define MOTOR_LEFT_FORWARD PA_1
#define MOTOR_LEFT_REVERSE PA_0
#define MOTOR_RIGHT_FORWARD PA_3
#define MOTOR_RIGHT_REVERSE PA_7
DriveSystem drivesystem(MOTOR_LEFT_FORWARD, MOTOR_LEFT_REVERSE, MOTOR_RIGHT_FORWARD, MOTOR_RIGHT_REVERSE, PWM_FREQ);
#define ROTATELEFT90TIME 500
// #define ROTATELEFT90TIME 620
#define ROTATERIGHT90TIME 700

// CLAW SYSTEM
#define ARM_PIN PB8
#define CLAW_PIN PB9
ClawSystem clawsystem(ARM_PIN, CLAW_PIN);

// HOOK 
Servo hook;
#define HOOK_PIN PB1


// SYSTEM
#define DELAYUNITTIME 70

void setup() {
  // Claw
  // clawsystem.rest_arm();

  // Serial.begin(9600);

  // Set up Pins Here
  pinMode(RIGHT_TAPE_SENSOR, INPUT);
  pinMode(LEFT_TAPE_SENSOR, INPUT);

  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(HOOK_PIN, OUTPUT);
  hook.attach(HOOK_PIN);



  // Testing Area
  // drivesystem.init();
  

  // Set up Variables Here
  found_tape = true;
  left_prev = true;
  right_prev = true;
  off_tape_turn_lot_num = 0;

  // // Print SetUp Message
  // display.clearDisplay();
  // display.setTextSize(1.5);
  // display.setTextColor(SSD1306_WHITE);
  // display.setCursor(0, 0);
  // display.println("Setup Successful!");
  // display.display();
  // display.clearDisplay();
  delay(3000);
  activate_hook();
  // clawsystem.raise_arm();
  delay(4000);
  // clawsystem.grab();
  // clawsystem.disconnect_arm();
  // drivesystem.init();
  // delay(300);
  drivesystem.forward_slow();
  // calibrate_hook();
}

void loop() {
  // // DriveSystem Test Section
  // drivesystem.rotate_left();
  // delay(ROTATE90TIME);
  // drivesystem.init();
  // delay(2000);
  // Sonar Test Section

  // Claw Test


  // Actual Code
  // display.clearDisplay();

  int left_distance = sonarsystem.getLeftDistance();
  int front_distance = sonarsystem.getFrontDistance();
  int right_distance = sonarsystem.getRightDistance();

  int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
  int lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);
  print_tape_state(lightvolt_left, lightvolt_right);
  if (lightvolt_left > 100 && lightvolt_left < 400) {
    drivesystem.stop(50);
    int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
    if (lightvolt_left > 200 && lightvolt_left < 400) {
      delay(100);
      drivesystem.rotate_right();
      delay(ROTATERIGHT90TIME*2);
      drivesystem.stop(DELAYUNITTIME);
      found_tape = false;
      drivesystem.forward_slow();
    }
  } 

  if (!found_tape && ((left_ontape && !right_ontape) || (!left_ontape && right_ontape))) {
    // Turn with an angle
    found_tape = true;
    drivesystem.stop(500);
    drivesystem.forward_slow();
    follow_tape();
  } 
  else if (!found_tape && (left_ontape && right_ontape) && !(left_prev || right_prev)) {
    // To prevent sharp turns 
    found_tape = true;
    drivesystem.stop(1000);
    int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
    int lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);
    print_tape_state(lightvolt_left, lightvolt_right);
    int count = 0;
    while (left_ontape && right_ontape && count < 10) {
      int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
      int lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);  
      print_tape_state(lightvolt_left, lightvolt_right);
      drivesystem.forward_slow();
      delay(70);
      drivesystem.stop(50);
      drivesystem.rotate_left();
      delay(70);
      drivesystem.stop(50);
      count++;
    }
    drivesystem.forward_slow();
    follow_tape();
  }

  // display.setCursor(0, 0);
  // display.print(left_distance);
  // display.print("   ");
  // display.print(front_distance);
  // display.print("   ");
  // display.print(right_distance);
  // display.setCursor(0, 10);
  // display.print(lightvolt_left);
  // display.print("    ");
  // display.print(lightvolt_right);
  // display.display();

  if (left_distance <= LEFT_CAN_THRESHOLD && left_distance != 0) {
    drivesystem.stop();

    // Starting to rotate robot to aim at the can
    while (front_distance > (LEFT_CAN_THRESHOLD - 4) || front_distance == 0) {
      front_distance = sonarsystem.getFrontDistance_accurate();
      drivesystem.rotate_left();
      delay(130);
      drivesystem.stop(DELAYUNITTIME);
    }
    drivesystem.rotate_left();
    delay(100);
    drivesystem.stop(DELAYUNITTIME); // to make sure it's centered 
    front_distance = sonarsystem.getFrontDistance_accurate();


    // Engage
    if (front_distance <= 9 && front_distance != 0) {
      drivesystem.stop(DELAYUNITTIME);
    } else {
      front_distance = sonarsystem.getFrontDistance_accurate();
      while (front_distance > GRABBING_THRESHOLD || front_distance == 0) {
        front_distance = sonarsystem.getFrontDistance_accurate();
        drivesystem.forward_slow();
        delay(120);
        drivesystem.stop(50);
        // if off-tracked
        if (front_distance > LEFT_CAN_THRESHOLD) {
          while (front_distance > LEFT_CAN_THRESHOLD) {
            front_distance = sonarsystem.getFrontDistance_accurate();
            drivesystem.rotate_left();
            delay(90);
            drivesystem.stop(50);
          }
        }
      }
      drivesystem.stop(DELAYUNITTIME);
    }
    
    // Grab, check, dispose
    clawsystem.grab_can_sequence();
    // clawsystem.check_can_sequence(sonarsystem);
    clawsystem.dispose_can_sequence();
    clawsystem.disconnect();
    delay(DELAYUNITTIME);
    calibrate_hook();

    // Go back
    found_tape = false;
    drivesystem.rotate_right();
    delay(ROTATERIGHT90TIME);
    drivesystem.forward_slow();
  } 
  else if (right_distance < RIGHT_CAN_THRESHOLD && right_distance != 0) {
    drivesystem.stop();

    // Starting to rotate robot to aim at the can
    while (front_distance > (RIGHT_CAN_THRESHOLD - 4) || front_distance == 0) {
      front_distance = sonarsystem.getFrontDistance_accurate();
      drivesystem.rotate_right();
      delay(120);
      drivesystem.stop(DELAYUNITTIME);
    }
    drivesystem.rotate_right();
    delay(100);
    drivesystem.stop(DELAYUNITTIME); // to make sure it's centered 
    front_distance = sonarsystem.getFrontDistance_accurate();

    // Engage
    if (front_distance <= 9 && front_distance != 0) {
      drivesystem.stop(DELAYUNITTIME);
    } else {
      front_distance = sonarsystem.getFrontDistance_accurate();
      while (front_distance > GRABBING_THRESHOLD || front_distance == 0) {
        front_distance = sonarsystem.getFrontDistance_accurate();
        drivesystem.forward_slow();
        delay(120);
        drivesystem.stop(50);
        // if off-tracked
        if (front_distance > RIGHT_CAN_THRESHOLD) {
          while (front_distance > RIGHT_CAN_THRESHOLD) {
            front_distance = sonarsystem.getFrontDistance_accurate();
            drivesystem.rotate_right();
            delay(90);
            drivesystem.stop(50);
          }
        }
      }
      drivesystem.stop(DELAYUNITTIME);
    }
    
    // Grab, check, dispose
    clawsystem.grab_can_sequence();
    // clawsystem.check_can_sequence(sonarsystem);
    clawsystem.dispose_can_sequence();
    clawsystem.disconnect();
    delay(DELAYUNITTIME);
    calibrate_hook();

    // Go back
    found_tape = false;
    drivesystem.rotate_left();
    delay(ROTATELEFT90TIME);
    drivesystem.forward_slow();
  } 
  else if (front_distance < FRONT_CAN_THRESHOLD && front_distance != 0) {
    // display.clearDisplay();
    // display.setCursor(0, 0);
    // display.print("CAN ON FRONT");
    drivesystem.stop();
    front_distance = sonarsystem.getFrontDistance_accurate();
    
    // Engage
    if (front_distance <= 9 && front_distance != 0) {
      drivesystem.stop(DELAYUNITTIME);
    } else {
      front_distance = sonarsystem.getFrontDistance_accurate();
      while (front_distance > GRABBING_THRESHOLD || front_distance == 0) {
        front_distance = sonarsystem.getFrontDistance_accurate();
        drivesystem.forward_slow();
        delay(120);
        drivesystem.stop(60);
        // if off-tracked
        if (front_distance > RIGHT_CAN_THRESHOLD) {
          while (front_distance > RIGHT_CAN_THRESHOLD) {
            front_distance = sonarsystem.getFrontDistance_accurate();
            drivesystem.rotate_right();
            delay(120);
            drivesystem.stop(50);
          }
        }
      }
      drivesystem.stop(DELAYUNITTIME);
    }
    
    // Grab, check, dispose
    clawsystem.grab_can_sequence();
    // clawsystem.check_can_sequence(sonarsystem);
    clawsystem.dispose_can_sequence();
    clawsystem.disconnect();
    delay(DELAYUNITTIME);
    calibrate_hook();

    drivesystem.forward_slow();
  } 
  else if (!found_tape) {
    drivesystem.forward_slow();
  } else {
    follow_tape();
  }

  left_prev = left_ontape;
  right_prev = right_ontape;
  // display.display();
}



// Helper Functions
void activate_hook() {
  for (int hookPos = 130; hookPos >= -20; hookPos--) {
    hook.write(hookPos);
    delay(6);
  }
  hook.detach();
}


void calibrate_hook() {
  hook.attach(HOOK_PIN);
  hook.write(-20);
  hook.detach();
}



void print_tape_state(int lightvolt_left, int lightvolt_right) {
  // display.setCursor(0, 20);
  if (lightvolt_left > LEFT_THRESHOLD_LIGHTVOLT) {
    left_ontape = true;
    // display.print("Left ON");
  } else {
    left_ontape = false;
    // display.print("Left OFF");
  }
  display.print("   ");
  if (lightvolt_right > RIGHT_THRESHOLD_LIGHTVOLT) {
    right_ontape = true;
    // display.print("Right ON");
  } else {
    right_ontape = false;
    // display.print("Right OFF");
  }
}

void follow_tape() {
  for (int k = ONTAPE_RECORD_NUM - 1; k >= 1; k--) {
      left_list[k] = left_list[k-1];
      right_list[k] = right_list[k-1];
  }
  left_list[0] = left_ontape;
  right_list[0] = right_ontape;

  if ((left_ontape && right_ontape) || off_tape_turn_lot_num >= OFFTAPELOTMAX) {
    // slowly go forward
    off_tape_turn_lot_num = 0;
    drivesystem.forward_med();
  } else if (left_ontape && !right_ontape) {
    // turn left a bit
    drivesystem.left_bit();
  } else if (!left_ontape && right_ontape) {
    // turn right a bit
    drivesystem.right_bit();
  } else {
    if (left_prev && !right_prev) {
      // turn left more
      drivesystem.left_more();
    }
    else if (!left_prev && right_prev) {
      // turn right more
      drivesystem.right_more();
    }
    else {
      bool turned = false;
      off_tape_turn_lot_num++;
      for (int i = 0; i < ONTAPE_RECORD_NUM && !turned; i++) {
        if (left_list[i] && !right_list[i]) {
          // turn left a lot
          drivesystem.left_lot();
          turned = true;
        }
        if (right_list[i] && !left_list[i]) {
          // turn right a lot
          drivesystem.right_lot();
          turned = true;
        }
      }
    }
  }
}


void entertainment() {
  drivesystem.rotate_left_fast();
  delay(2000);
  drivesystem.stop(100);
  drivesystem.rotate_right_fast();
  delay(2000);
  drivesystem.stop(100);
  drivesystem.forward_fast();
  delay(500);
  drivesystem.stop();
  clawsystem.raise_arm();
  delay(100);
  clawsystem.grab();
}



//// TEST FUNCTIONS

void speedTest(String side) {
  if (side == "l") {
    drivesystem.update(0, 37);
    delay(2000);
    drivesystem.update(0, 39);
    delay(2000);
    drivesystem.update(0, 41);
    delay(2000);
    drivesystem.update(0, 43);
  } else {
    drivesystem.update(37, 0);
    delay(2000);
    drivesystem.update(39, 0);
    delay(2000);
    drivesystem.update(41, 0);
    delay(2000);
    drivesystem.update(43, 0);
  }
  
}

void rotateTest(String direction) {
  if (direction == "l") {
    drivesystem.rotate_left();
    delay(ROTATELEFT90TIME);
  } else {
    drivesystem.rotate_right();
    delay(ROTATERIGHT90TIME);
  }
  drivesystem.stop(5000);
}