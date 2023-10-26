#include "motor.h"
//This is optmized for simplicty not performance. 
//If (more) performance is needed motor object should not be passed around as much
//On an ESP2 this is not av significant problem

//Excpects 16 bit resolution
#define MOTOR_LEDC_TIMER_16_BIT 16


/*
* Takes in speed values between 3277 and 6553 (1ms and 2ms on-time in PWM)
* Give max resolution on PWM, should only be used internally in motor module.
*/
static void motorBLDCSetSpeed(uint32_t speed, uint8_t motorChannel) {
  if((speed > 6553) || (speed < 3277)) {
    Serial.printf("Invalid motor speed\n");
    return;
  }
  else {
    ledcWrite(motorChannel, speed);
  }
}

void motorInit(Motor motor) {
  pinMode(motor.pin, OUTPUT);
  ledcSetup(motor.channel, motor.freq, MOTOR_LEDC_TIMER_16_BIT);
  ledcAttachPin(motor.pin, motor.channel);
  if(motor.motorType == MOTOR_TYPE_BLDC) {
    setMotorSpeed(-127, motor);
    setMotorSpeed(0, motor);
  }
}

void setMotorSpeed(int8_t speed, Motor motor) {
  if(motor.motorType == MOTOR_TYPE_BLDC) {
    //This is where we could take advantage of 16 bit resolution to ramp up more slowly
    uint32_t mapped_speed = map(speed, -127, 127, 3277, 6553);
    motorBLDCSetSpeed(mapped_speed, motor.channel);
  }
  else if(motor.motorType == MOTOR_TYPE_DC) {
    uint32_t mapped_speed = map(speed, -127, 127, 3277, 6553);
    ledcWrite(motor.channel, mapped_speed);
  }
  else {
    Serial.printf("Invalid motor type\n");
  }
}