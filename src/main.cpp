#include <Arduino.h>
#include <PS4Controller.h>
#include "motor.h"

//Pins
#define BLDC_PIN 33
#define MOTOR_L_PIN0 12
#define MOTOR_L_PIN1 14
#define MOTOR_R_PIN0 27
#define MOTOR_R_PIN1 26

//Frequencies
#define DC_FREQ 20000
#define BLDC_FREQ 50

//LEDC channels
#define BLDC_CHAN 1
#define MOTOR_L_CHAN0 2
#define MOTOR_L_CHAN1 4
#define MOTOR_R_CHAN0 6
#define MOTOR_R_CHAN1 8

//Motor structs setup
Motor motorWeapon {BLDC_PIN, 0, BLDC_CHAN, 10, BLDC_FREQ, MOTOR_TYPE_BLDC};
Motor motorRight {MOTOR_R_PIN0, MOTOR_R_PIN1, MOTOR_R_CHAN0, MOTOR_R_CHAN1, DC_FREQ, MOTOR_TYPE_DC};
Motor motorLeft {MOTOR_L_PIN0, MOTOR_L_PIN1, MOTOR_L_CHAN0, MOTOR_L_CHAN1, DC_FREQ, MOTOR_TYPE_DC};

void steering(Motor motorLeft, Motor motorRight, PS4Controller PS4) {
  if(PS4.isConnected()) {
    setMotorSpeed(PS4.LStickY(), motorLeft);
    setMotorSpeed(PS4.RStickY(), motorRight);
    setMotorSpeed(PS4.RStickX(), motorWeapon);
  }
  else {
    setMotorSpeed(0, motorLeft);
    setMotorSpeed(0, motorRight);
    setMotorSpeed(0, motorWeapon);
  }
}

void setup() {

  Serial.begin(9600);
  
  motorInit(motorWeapon);
  motorInit(motorLeft);
  motorInit(motorRight);

  Serial.printf("Test\n");
  PS4.begin("dc:e9:94:b5:b6:5e\n");
}

void loop() {
  steering(motorLeft, motorRight, PS4); //Change to right after test

  delay(1);
  /*
  if (PS4.isConnected()) {
    if (PS4.Square()) Serial.println("Square Button");
    if (PS4.Cross()) Serial.println("Cross Button");
    if (PS4.Circle()) Serial.println("Circle Button");
    if (PS4.Triangle()) Serial.println("Triangle Button");

    if (PS4.UpRight()) Serial.println("Up Right");
    if (PS4.DownRight()) Serial.println("Down Right");
    if (PS4.UpLeft()) Serial.println("Up Left");
    if (PS4.DownLeft()) Serial.println("Down Left");

    if (PS4.L1()) Serial.println("L1 Button");
    if (PS4.R1()) Serial.println("R1 Button");

    if (PS4.LStickX()) {
      Serial.printf("Left Stick x at %d\n", PS4.LStickX());
    }
    if (PS4.LStickY()) {
      Serial.printf("Left Stick y at %d\n", PS4.LStickY());
    }
    if (PS4.RStickX()) {
      Serial.printf("Right Stick x at %d\n", PS4.RStickX());
    }
    if (PS4.RStickY()) {
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
    }

    if (PS4.Charging()) Serial.println("The controller is charging");
    if (PS4.Audio()) Serial.println("The controller has headphones attached");
    if (PS4.Mic()) Serial.println("The controller has a mic attached");

    Serial.printf("Battery Level : %d\n", PS4.Battery());

  }else {
    Serial.printf("Controller not connected");
  }    
  Serial.printf("\n");
  */
}

