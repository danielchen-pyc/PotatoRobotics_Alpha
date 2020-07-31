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
void print_tape_state(int lightvolt_left, int lightvolt_right);
void follow_tape();
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
#define RIGHT_THRESHOLD_LIGHTVOLT 160
#define LEFT_THRESHOLD_LIGHTVOLT 860
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
#define LEFT_CAN_THRESHOLD 35
#define RIGHT_CAN_THRESHOLD 35
#define FRONT_CAN_THRESHOLD 15
#define GRABBING_THRESHOLD 1

// DRIVE SYSTEM
#define PWM_FREQ 2000
#define MOTOR_LEFT_FORWARD PA_1
#define MOTOR_LEFT_REVERSE PA_0
#define MOTOR_RIGHT_FORWARD PA_3
#define MOTOR_RIGHT_REVERSE PA_7
DriveSystem drivesystem(MOTOR_LEFT_FORWARD, MOTOR_LEFT_REVERSE, MOTOR_RIGHT_FORWARD, MOTOR_RIGHT_REVERSE, PWM_FREQ);
#define ROTATE90TIME 620

// CLAW SYSTEM
#define ARM_PIN PB8
#define CLAW_PIN PB9
ClawSystem clawsystem(ARM_PIN, CLAW_PIN);

// Temporary Claw Section
// Servo claw;

void setup() {
  // Claw
  // clawsystem.rest_arm();
  // claw.attach(CLAW_PIN);
  clawsystem.init();

  // Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);

  // Set up Pins Here
  pinMode(RIGHT_TAPE_SENSOR, INPUT);
  pinMode(LEFT_TAPE_SENSOR, INPUT);

  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA7, OUTPUT);


  // Testing Area
  // drivesystem.init();
  // drivesystem.forward_slow();
  delay(5000);
  rotateTest("l");
  
  

  // Set up Variables Here
  found_tape = false;
  off_tape_turn_lot_num = 0;

  // Print SetUp Message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Setup Successful!");
  display.display();
  display.clearDisplay();
  delay(3000);
  // clawsystem.raise_arm();
  // delay(5000);
  // clawsystem.grab();
  // delay(3000);
  // clawsystem.disconnect_arm();
}

void loop() {
  // // DriveSystem Test Section
  // drivesystem.rotate_left();
  // delay(ROTATE90TIME);
  // drivesystem.stop(3000);

  // Sonar Test Section

  // // Claw Test
  // clawsystem.grab();
  // delay(1000);
  // clawsystem.raise_arm();
  // delay(1000);
  // // clawsystem.grab_can_sequence();
  // // clawsystem.check_can_sequence(sonarsystem);
  // clawsystem.open_claw();
  // delay(3000);
  // clawsystem.lower_arm();
  // delay(5000);





  // Actual Code
  display.clearDisplay();
  
  if (!found_tape && (left_ontape || right_ontape)) {
    found_tape = true;
  }

  display.setCursor(0, 0);
  unsigned int left_distance = sonarsystem.getLeftDistance();
  unsigned int front_distance = sonarsystem.getFrontDistance();
  unsigned int right_distance = sonarsystem.getRightDistance();

  display.print(left_distance);
  display.print("   ");
  display.print(front_distance);
  display.print("   ");
  display.print(right_distance);

  int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
  int lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);
  print_tape_state(lightvolt_left, lightvolt_right);
  // display.setCursor(0, 10);
  // display.print(lightvolt_left);
  // display.print("    ");
  // display.print(lightvolt_right);
  // display.display();

  // if (left_distance < LEFT_CAN_THRESHOLD && left_distance != 0) {
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.println("CAN ON LEFT");    
  //   display.display();

  //   drivesystem.stop(1000);
  //   display.clearDisplay();
  //   display.setCursor(0, 0);

  //   // Starting to rotate robot to aim at the can
  //   while (front_distance > (LEFT_CAN_THRESHOLD - 4)) {
  //     front_distance = sonarsystem.getFrontDistance();
  //     drivesystem.rotate_left();
  //     display.println("Aiming.....");
  //     display.display();
  //   }
  //   // delay(50); // to make sure it's centered 
  //   display.println("Aimed. Stop for a sec.");
  //   display.display();
  //   drivesystem.stop(3000);

  //   // Engage
  //   do {
  //     front_distance = sonarsystem.getFrontDistance();
  //     display.clearDisplay();
  //     display.setCursor(0, 0);
  //     display.print("Engage.");
  //     display.display();
  //     drivesystem.forward_slow();
  //     // if off-tracked
  //     // if (front_distance > LEFT_CAN_THRESHOLD) {
  //     //   while (front_distance > LEFT_CAN_THRESHOLD) {
  //     //     drivesystem.rotate_left();
  //     //   }
  //     // }
  //   } while (front_distance > GRABBING_THRESHOLD);
  //   drivesystem.stop(3000);
    
  //   // Grab, check, dispose
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.println("Grab can");
  //   display.display();
  //   clawsystem.grab_can_sequence();
  //   display.println("check can");
  //   display.display();
  //   // clawsystem.check_can_sequence(sonarsystem);
  //   display.println("dispose can");
  //   display.display();
  //   clawsystem.dispose_can_sequence();

  //   // Go back
  //   found_tape = false;
  //   drivesystem.rotate_left();
  //   delay(ROTATE90TIME*2);
  //   drivesystem.forward_slow();

  // } 
  // else if (right_distance < RIGHT_CAN_THRESHOLD && right_distance != 0) {
  //   display.clearDisplay();
  //   display.setCursor(0, 0);
  //   display.println("CAN ON RIGHT");    
  //   display.display();

  //   drivesystem.stop(1000);
  //   display.clearDisplay();
  //   display.setCursor(0, 0);

  //   // Starting to rotate robot to aim at the can
  //   while (front_distance > (RIGHT_CAN_THRESHOLD - 4)) {
  //     front_distance = sonarsystem.getFrontDistance();
  //     drivesystem.rotate_right();
  //     display.println("Aiming.....");
  //     display.display();
  //   }
  //   // delay(50); // to make sure it's centered 
  //   display.println("Aimed. Stop for a sec.");
  //   display.display();
  //   drivesystem.stop(3000);

  //   // Engage
  //   do {
  //     front_distance = sonarsystem.getFrontDistance();
  //     // display.clearDisplay();
  //     // display.setCursor(0, 0);
  //     // display.print("Engage.");
  //     // display.display();
  //     drivesystem.forward_slow();
  //     // if off-tracked
  //     // if (front_distance > LEFT_CAN_THRESHOLD) {
  //     //   while (front_distance > LEFT_CAN_THRESHOLD) {
  //     //     drivesystem.rotate_left();
  //     //   }
  //     // }
  //   } while (front_distance > GRABBING_THRESHOLD);
  //   drivesystem.stop(3000);
    
  //   // Grab, check, dispose
  //   display.println("Grab can");
  //   display.display();  
  //   clawsystem.grab_can_sequence();
  //   display.println("check can");
  //   display.display(); 
  //   clawsystem.check_can_sequence(sonarsystem);
  //   display.println("dispose can");
  //   display.display(); 
  //   clawsystem.dispose_can_sequence();

  //   // Go back
  //   found_tape = false;
  //   drivesystem.rotate_right();
  //   delay(ROTATE90TIME*2);
  //   drivesystem.forward_slow();
  // } 
  // // else if (front_distance < FRONT_CAN_THRESHOLD && front_distance != 0) {
  // //   display.clearDisplay();
  // //   display.setCursor(0, 0);
  // //   display.print("CAN ON FRONT");
  // //   drivesystem.forward_slow();
    
  // //   // Engage
  // //   while (front_distance > GRABBING_THRESHOLD) {
  // //     front_distance = sonarsystem.getFrontDistance();
  // //     drivesystem.forward_slow();
  // //     // if off-tracked
  // //     if (front_distance > FRONT_CAN_THRESHOLD) {
  // //       while (front_distance > FRONT_CAN_THRESHOLD) {
  // //         // drivesystem.rotate_left();
  // //       }
  // //     }
  // //   }
    
  // //   // Grab, check, dispose
  // //   clawsystem.grab_can_sequence();
  // //   clawsystem.check_can_sequence(sonarsystem);
  // //   clawsystem.dispose_can_sequence();
  // // }
  // if (!found_tape) {
  //   display.setCursor(0, 0);
  //   display.print("NOT FOUND TAPE");
  //   drivesystem.forward_slow();
  // } else {
  //   display.setCursor(0, 0);
  //   display.print("TAPE FOLLOWING");
  //   follow_tape();
  // }

  // left_prev = left_ontape;
  // right_prev = right_ontape;
  display.display();
  // delay(50);
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



//// TEST FUNCTIONS

void speedTest(String side) {
  if (side == "l") {
    drivesystem.update(0, 37);
    delay(ROTATE90TIME);
    drivesystem.update(0, 39);
    delay(ROTATE90TIME);
    drivesystem.update(0, 41);
    delay(ROTATE90TIME);
    drivesystem.update(0, 43);
  } else {
    drivesystem.update(37, 0);
    delay(ROTATE90TIME);
    drivesystem.update(39, 0);
    delay(ROTATE90TIME);
    drivesystem.update(41, 0);
    delay(ROTATE90TIME);
    drivesystem.update(43, 0);
  }
  
}

void rotateTest(String direction) {
  if (direction == "l") {
    drivesystem.rotate_left();
  } else {
    drivesystem.rotate_right();
  }
  delay(ROTATE90TIME);
  drivesystem.stop(5000);
}