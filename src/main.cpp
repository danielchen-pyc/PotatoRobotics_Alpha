#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <cmath>
#include "DriveSystem.h"

using namespace std;

void set_tape_state(int lightvolt_left, int lightvolt_right);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PWM_FREQ 2000

#define MOTOR_LEFT_FORWARD PA_0
#define MOTOR_LEFT_REVERSE PA_3
#define MOTOR_RIGHT_FORWARD PA_1
#define MOTOR_RIGHT_REVERSE PA_2

#define RIGHT_TAPE_SENSOR PB0
#define LEFT_TAPE_SENSOR PB1
#define RIGHT_THRESHOLD_LIGHTVOLT 37
#define LEFT_THRESHOLD_LIGHTVOLT 34.5

bool found_tape;
bool left_ontape;
bool left_prev;
bool right_ontape;
bool right_prev;

#define ONTAPE_RECORD_NUM 10
bool left_list[ONTAPE_RECORD_NUM];
bool right_list[ONTAPE_RECORD_NUM];


DriveSystem drivesystem = DriveSystem(MOTOR_LEFT_FORWARD, MOTOR_LEFT_REVERSE, MOTOR_RIGHT_FORWARD, MOTOR_LEFT_REVERSE, PWM_FREQ);


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
  pinMode(PA3, OUTPUT);

  pwm_stop(MOTOR_LEFT_FORWARD);
  pwm_stop(MOTOR_LEFT_REVERSE);
  pwm_stop(MOTOR_RIGHT_FORWARD);
  pwm_stop(MOTOR_RIGHT_REVERSE);

  // drivesystem.forward();


  // Set up Variables Here
  found_tape = false;

  // Print SetUp Message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Setup Successful!");
  display.display();
  delay(100);
}

void loop() {
  double lightvolt_left = analogRead(LEFT_TAPE_SENSOR);
  double lightvolt_right = analogRead(RIGHT_TAPE_SENSOR);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(lightvolt_left);
  display.print("    ");
  display.print(lightvolt_right);

  set_tape_state(lightvolt_left, lightvolt_right);
  
  if (!found_tape && (left_ontape || right_ontape)) {
    found_tape = true;
  }
  
  if (!found_tape) {
    drivesystem.forward_med();
  } else {
    for (int k = ONTAPE_RECORD_NUM - 1; k >= 1; k--) {
      left_list[k] = left_list[k-1];
      right_list[k] = right_list[k-1];
    }
    left_list[0] = left_ontape;
    right_list[0] = right_ontape;
    if (left_ontape && right_ontape) {
      // slowly go forward
      drivesystem.forward_slow();
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
        for (int i = 0; i < ONTAPE_RECORD_NUM; i++) {
          if (left_list[i] && !right_list[i]) {
            // turn left a lot
            drivesystem.left_lot();
          }
          if (right_list[i] && !left_list[i]) {
            // turn right a lot
            drivesystem.right_lot();
          }
        }
      }
    }
    
  }

  display.display();

  left_prev = left_ontape;
  right_prev = right_ontape;
}


void set_tape_state(int lightvolt_left, int lightvolt_right) {
  if (lightvolt_left > LEFT_THRESHOLD_LIGHTVOLT) {
    left_ontape = true;
    display.setCursor(0, 10);
    display.print("Left ON");
  } else {
    left_ontape = false;
    display.setCursor(0, 10);
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
