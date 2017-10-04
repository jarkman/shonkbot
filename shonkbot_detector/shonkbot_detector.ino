
/*

Shonkbot with IR object detection and a bump-and-turn action for obstacle avoidance

See http://jarkman.co.uk/catalog/robots/shonkbot.htm for more details

-- 

This is 25e5c44 (the newest one I could find that does wander ok, I think?) but with Newping instread of IR detection
Newping doesn't work with tone :-(
-- Libby

*/

#include <NewPing.h>

#include <AccelStepper.h>
#include "TwoWheel.h"

#define DO_LOGGING
#define DEBUG


// Arduino pins wired to stepper drivers - driver board labels pins as IN1, IN2 etc
#define LEFT_IN1 2
#define LEFT_IN2 3
#define LEFT_IN3 4
#define LEFT_IN4 5

#define RIGHT_IN1 6
#define RIGHT_IN2 7
#define RIGHT_IN3 8
#define RIGHT_IN4 9

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 10

// Define a stepper and the pins it will use
// We use HALF4WIRE not FULL4WIRE because it results in lower overall current consumption (150mA per motor as opposed to 200mA)
AccelStepper leftStepper(AccelStepper::HALF4WIRE, LEFT_IN1,LEFT_IN3,LEFT_IN2,LEFT_IN4); // note middle two pins are swapped to work with 28BYJ-48 
                                                                                                                      
AccelStepper rightStepper(AccelStepper::HALF4WIRE, RIGHT_IN1,RIGHT_IN3,RIGHT_IN2,RIGHT_IN4); // note middle two pins are swapped!

#define COLLISION_PHOTOTRANSISTOR_PIN A0    // Wire IR phototransistor from A0 to ground, with a 10k pullup

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// using 28BYj-48 motors from http://www.ebay.co.uk/itm/131410728499

#define STEPS_PER_REV (4096.0/2.0) // these motors come in two different gear rations, 
                              // see http://42bots.com/tutorials/28byj-48-stepper-motor-with-uln2003-driver-and-arduino-uno/ 
                              // and http://forum.arduino.cc/index.php?topic=71964.15

#define MAX_SPEED 1200 // 1000 is a sensible limit on 3 x 1.5v battery, it's probably as much as we cna expect to get from AccelStepper anyhow
#define MAX_ACCELERATION 1600 // 1600 on 5v, 400 on 3v

#define WHEEL_DIAMETER 40.0   // radius of your wheel in mm - 40.00 is right for a blue wheel with a rubber band on
#define WHEEL_SPACING 200.0   // distance from one wheel to the other in mm

TwoWheel twoWheel( &leftStepper, &rightStepper, STEPS_PER_REV, WHEEL_DIAMETER, WHEEL_SPACING );


void setup()
{  
  #ifdef DO_LOGGING
  Serial.begin (57600); // for debugging

  Serial.print ("setup\n");
  #endif
  
  twoWheel.setup();

  leftStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  leftStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  rightStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  rightStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  
  setupWander();
}


void loop()
{
  
  loopWander();
  twoWheel.loop();
  
}




