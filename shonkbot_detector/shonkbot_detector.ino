
#include <AccelStepper.h>
#include "IRDetector.h"


#define  DO_LOGGING



// Arduino pins wired to stepper drivers - driver bosrd labels pinsas IN1, IN2 etc
#define LEFT_IN1 2
#define LEFT_IN2 3
#define LEFT_IN3 4
#define LEFT_IN4 5

#define RIGHT_IN1 6
#define RIGHT_IN2 7
#define RIGHT_IN3 8
#define RIGHT_IN4 9


// Define a stepper and the pins it will use
// We use HALF4WIRE because it results in lower overall current consumption (150mA per motor as opposed to 200mA)
AccelStepper leftStepper(AccelStepper::HALF4WIRE, LEFT_IN1,LEFT_IN3,LEFT_IN2,LEFT_IN4); // note middle two pins are swapped to work with 28BYJ-48 
                                                                                                                      
AccelStepper rightStepper(AccelStepper::HALF4WIRE, RIGHT_IN1,RIGHT_IN3,RIGHT_IN2,RIGHT_IN4); // note middle two pins are swapped!

// Wire IR LED to pin 13, IR phototransistor to A0, use 100 hz for collision detection
#define PIEZO_PIN 12
#define COLLISION_LED_PIN 13
#define COLLISION_PHOTOTRANSISTOR_PIN A0
#define COLLISION_FREQUENCY 100

IRDetector collisionDetector(COLLISION_LED_PIN, COLLISION_PHOTOTRANSISTOR_PIN, PIEZO_PIN, COLLISION_FREQUENCY);

// using 28BYj-48 motors from http://www.ebay.co.uk/itm/131410728499

#define STEPS_PER_REV (4096.0/2.0) // these motore come in two different gear rations, 
                              // see http://42bots.com/tutorials/28byj-48-stepper-motor-with-uln2003-driver-and-arduino-uno/ 
                              // and http://forum.arduino.cc/index.php?topic=71964.15

#define MAX_SPEED 350 // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
#define MAX_ACCELERATION 400 // 1600 on 5v, 400 on 3v

#define WHEEL_DIAMETER 48.0 // radius of your wheel in mm
#define WHEEL_SPACING 150.0   // distance from one wheel to the other in mm


void setup()
{  
  #ifdef DO_LOGGING
  Serial.begin (57600); // for debugging

  Serial.print ("setup\n");
  #endif
  
  collisionDetector.setup();

  leftStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  leftStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  rightStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  rightStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  
  //setupScript();
  setupWander();
}


void loop()
{
  collisionDetector.loop();
  
  
  
  //loopScript();
  loopWander();
  
  leftStepper.run();
  rightStepper.run();
   
}


float stepsForDistance( float distance )
{
  return distance * STEPS_PER_REV / (3.1415 * WHEEL_DIAMETER);
}

float stepsForTurn( float degrees ) //  clockwise
{
  float theta = radians(degrees);
  
  float distance = WHEEL_SPACING * 0.5 * theta ;
  float steps = stepsForDistance( distance );
  
  return steps;
}


