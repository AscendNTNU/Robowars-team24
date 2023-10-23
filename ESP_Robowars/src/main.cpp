#include <Arduino.h>
#include <Servo.h>

Servo myservo1 = Servo();
Servo myservo2 = Servo();

const int potPin1 = 35, servoPin1 = 13;
const int potPin2 = 34, servoPin2 = 12;

void setup() {
  myservo1.setFrequency(servoPin1, 10000);
  myservo2.setFrequency(servoPin2, 5000);
}

void loop() {

  int val1 = analogRead(potPin1);       // read the pot value (0-4095)
  val1 = map(val1, 0, 4095, 35, 142);  // align pot pointer to servo arm
  // myservo1.write(servoPin1, 1);      // set the servo position (degrees)
  myservo1.writePwm(servoPin1, val1, 20000);
  int val2 = analogRead(potPin2);       // read the pot value (0-4095)
  val2 = map(val2, 0, 4095, 35, 142);  // align pot pointer to servo arm
  // myservo2.write(servoPin2, 2);      // set the servo position (degrees)
  //myservo2.writePwm(servoPin2, 20, 20000);
  delay(10);
  myservo1.printDebug();
  myservo2.printDebug();
}