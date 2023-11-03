#include "motor.h"
//This is optmized for simplicty not performance. 
//If (more) performance is needed motor object should not be passed around as much
//On an ESP2 this is not av significant problem

//Excpects 16 bit resolution
#define MOTOR_LEDC_TIMER_16_BIT 10


/*
* Takes in speed values between 3277 and 6553 (1ms and 2ms on-time in PWM)
* Give max resolution on PWM, should only be used internally in motor module.
*/
static void motorBLDCSetSpeed(uint32_t speed, uint8_t motorChannel) {
  if((speed > 102) || (speed < 51)) {
    Serial.printf("Invalid motor speed\n");
    return;
  }
  else {
    ledcWrite(motorChannel, speed);
  }
}

void motorInit(Motor motor) {
  if(motor.motorType == MOTOR_TYPE_DC) { //DC uses 2 pins
    pinMode(motor.pin1, OUTPUT);
    ledcSetup(motor.channel1, motor.freq, MOTOR_LEDC_TIMER_16_BIT);
    ledcAttachPin(motor.pin1, motor.channel1);
  }
  pinMode(motor.pin0, OUTPUT);
  ledcSetup(motor.channel0, motor.freq, MOTOR_LEDC_TIMER_16_BIT);
  ledcAttachPin(motor.pin0, motor.channel0);
  Serial.printf("Set up pin, chan %i %i %i\n", motor.pin0, motor.channel0, motor.freq);
  if(motor.motorType == MOTOR_TYPE_BLDC) {
    setMotorSpeed(-127, motor);
    delay(100);
    setMotorSpeed(0, motor);
  }
}

void setMotorSpeed(int8_t speed, Motor motor) {
  if(motor.motorType == MOTOR_TYPE_BLDC) {
    //This is where we could take advantage of 16 bit resolution to ramp up more slowly
    uint32_t mapped_speed = map(speed, -127, 127, 51, 102); //3277, 6553);
    motorBLDCSetSpeed(mapped_speed, motor.channel0);
    int freq = ledcReadFreq(motor.channel0);
    Serial.printf("Set BLDC speed %i %i\n", mapped_speed, freq);
  }
  else if(motor.motorType == MOTOR_TYPE_DC) {
    if(speed > 0) {
      uint32_t mapped_speed = map(speed, 0, 127, 0, 1023);
      ledcWrite(motor.channel0, 0);
      ledcWrite(motor.channel1, mapped_speed);
      Serial.printf("Mapped over %i %i %i %i %i\n", mapped_speed, motor.channel0, motor.channel1, motor.pin0, motor.pin1);
    }
    else if(speed < 0) {
      uint32_t mapped_speed = map(speed, 0, -127, 0, 1023);
      ledcWrite(motor.channel1, 0);
      ledcWrite(motor.channel0, mapped_speed);
      Serial.printf("Mapped over %i %i %i %i %i\n", mapped_speed, motor.channel0, motor.channel1, motor.pin0, motor.pin1);
    }
    else if(speed == 0) {
      ledcWrite(motor.channel0, speed);
      ledcWrite(motor.channel1, speed);

    }
    Serial.printf("Speed %i\n", speed);
  }
  else {
    Serial.printf("Invalid motor type\n");
  }
}