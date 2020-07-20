#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <cmath>
#include "NewPing.h"
#include "DriveSystem.h"

using namespace std;


// FUNCTION PROTOTYPE
void print_tape_state(int lightvolt_left, int lightvolt_right);
void follow_tape();

// SCREEN
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MOTOR 
#define PWM_FREQ 2000
#define MOTOR_LEFT_FORWARD PA_0
#define MOTOR_LEFT_REVERSE PA_1
#define MOTOR_RIGHT_FORWARD PA_2
#define MOTOR_RIGHT_REVERSE PA_7

// TAPE FOLLOWING
#define RIGHT_TAPE_SENSOR PB0
#define LEFT_TAPE_SENSOR PB1
#define RIGHT_THRESHOLD_LIGHTVOLT 100
#define LEFT_THRESHOLD_LIGHTVOLT 100
#define ONTAPE_RECORD_NUM 10
bool left_list[ONTAPE_RECORD_NUM];
bool right_list[ONTAPE_RECORD_NUM];

// SONAR
#define TRIGGER_PIN_LEFT PA11 // sonar uses digital pins
#define ECHO_PIN_LEFT PA12
#define TRIGGER_PIN_RIGHT PA15 
#define ECHO_PIN_RIGHT PB3
#define TRIGGER_PIN_FRONT PB4
#define ECHO_PIN_FRONT PB5
#define MAX_DISTANCE 200
NewPing sonar_left(TRIGGER_PIN_LEFT, ECHO_PIN_LEFT, MAX_DISTANCE);
NewPing sonar_right(TRIGGER_PIN_RIGHT, ECHO_PIN_RIGHT, MAX_DISTANCE);
NewPing sonar_front(TRIGGER_PIN_FRONT, ECHO_PIN_FRONT, MAX_DISTANCE);
#define LEFT_CAN_THRESHOLD 45
#define RIGHT_CAN_THRESHOLD 45
#define GRABBING_THRESHOLD 8

// 
bool found_tape;
bool left_ontape;
bool left_prev;
bool right_ontape;
bool right_prev;



DriveSystem drivesystem = DriveSystem(MOTOR_LEFT_FORWARD, MOTOR_LEFT_REVERSE, MOTOR_RIGHT_FORWARD, MOTOR_RIGHT_REVERSE, PWM_FREQ);

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);

  // Set up Pins Here
  pinMode(RIGHT_TAPE_SENSOR, INPUT);
  pinMode(LEFT_TAPE_SENSOR, INPUT);

  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA7, OUTPUT);

  pwm_stop(MOTOR_LEFT_FORWARD);
  pwm_stop(MOTOR_LEFT_REVERSE);
  pwm_stop(MOTOR_RIGHT_FORWARD);
  pwm_stop(MOTOR_RIGHT_REVERSE);

  // Testing Area
  // drivesystem.init();
  // drivesystem.forward_med();
  

  // Set up Variables Here
  found_tape = false;

  // Print SetUp Message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Setup Successful!");
  display.display();
  delay(3000);
}

void loop() {
  // Sonar Test Section
  // display.clearDisplay();
  // display.setCursor(0, 10);
  // unsigned int distance = sonar_left.ping_cm();
  // display.print(distance);

  // Claw Servo Test Section
  
  if (!found_tape && (left_ontape || right_ontape)) {
    found_tape = true;
  }

  display.setCursor(0, 20);
  unsigned int left_distance = sonar_left.ping_cm();
  unsigned int front_distance = sonar_front.ping_cm();
  unsigned int right_distance = sonar_right.ping_cm();

  display.print(left_distance);
  display.print("   ");
  display.print(front_distance);
  display.print("   ");
  display.print(right_distance);

  if (!found_tape) {
    drivesystem.forward_med();
  } else if (left_distance < LEFT_CAN_THRESHOLD) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("CAN ON LEFT");
    bool aimed_object_left = false;
    drivesystem.stop(500);

    // Starting to rotate robot to aim at the can
    while (front_distance > LEFT_CAN_THRESHOLD) {
      front_distance = sonar_front.ping_cm();
      drivesystem.rotate_left();
      display.setCursor(0, 10);
      display.print("Aiming.....");
    }
    delay(200); // to make sure it's centered 
    aimed_object_left = true;
    display.setCursor(0, 20);
    display.print("Aimed");
    drivesystem.stop(500);

    // Engage
    while (front_distance > GRABBING_THRESHOLD) {
      front_distance = sonar_front.ping_cm();
      drivesystem.forward_slow();
      // if off-tracked
      if (front_distance > LEFT_CAN_THRESHOLD) {
        while (front_distance > LEFT_CAN_THRESHOLD) {
          drivesystem.rotate_left();
        }
      }
    }
    
    // Grab 
    
  }
  else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("TAPE FOLLOWING");
    follow_tape();
  }


  left_prev = left_ontape;
  right_prev = right_ontape;
  display.display();
  delay(50);
}

void print_tape_state(int lightvolt_left, int lightvolt_right) {
  display.setCursor(0, 20);
  if (lightvolt_left > LEFT_THRESHOLD_LIGHTVOLT) {
    left_ontape = true;
    display.print("Left ON");
  } else {
    left_ontape = false;
    display.print("Left OFF");
  }
  display.print("   ");
  if (lightvolt_right > RIGHT_THRESHOLD_LIGHTVOLT) {
    right_ontape = true;
    display.print("Right ON");
  } else {
    right_ontape = false;
    display.print("Right OFF");
  }
}

void follow_tape() {
  int lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
  int lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);
  display.setCursor(0, 10);
  display.print(lightvolt_left);
  display.print("    ");
  display.print(lightvolt_right);

  print_tape_state(lightvolt_left, lightvolt_right);
  delay(100);

  for (int k = ONTAPE_RECORD_NUM - 1; k >= 1; k--) {
      left_list[k] = left_list[k-1];
      right_list[k] = right_list[k-1];
  }
  left_list[0] = left_ontape;
  right_list[0] = right_ontape;

  // display.setCursor(0, 30);
  // for (int l = 0; l < ONTAPE_RECORD_NUM; l++) {
  //   display.print(left_list[l]);
  //   display.print(" ");
  // }
  // display.setCursor(0, 40);
  // for (int r = 0; r < ONTAPE_RECORD_NUM; r++) {
  //   display.print(right_list[r]);
  //   display.print(" ");
  // }
  
  display.setCursor(0, 30);
  if (left_ontape && right_ontape) {
    // slowly go forward
    drivesystem.forward_slow();
    display.print("slowly go forward");
  } else if (left_ontape && !right_ontape) {
    // turn left a bit
    drivesystem.left_bit();
    display.print("turn left a bit");
  } else if (!left_ontape && right_ontape) {
    // turn right a bit
    drivesystem.right_bit();
    display.print("turn right a bit");
  } else {
    if (left_prev && !right_prev) {
      // turn left more
      drivesystem.left_more();
      display.print("turn left more");
    }
    else if (!left_prev && right_prev) {
      // turn right more
      drivesystem.right_more();
      display.print("turn right more");
    }
    else {
      bool turned = false;
      for (int i = 0; i < ONTAPE_RECORD_NUM && !turned; i++) {
        if (left_list[i] && !right_list[i]) {
          // turn left a lot
          drivesystem.left_lot();
          display.print("turn left a lot");
          turned = true;
        }
        if (right_list[i] && !left_list[i]) {
          // turn right a lot
          drivesystem.right_lot();
          display.print("turn right a lot");
          turned = true;
        }
      }
    }
  }
}