#include <stdio.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <cmath>

using namespace std;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Hello");
}