#ifndef TwoWheel_h
#define TwoWheel_h

#include <AccelStepper.h>

/*

TwoWheel

Motion management for a 2-wheel stepper robot

*/



#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

#define MAX_STEPS 2147483646L

class TwoWheel
{
  AccelStepper *leftStepper;
  AccelStepper *rightStepper;
  
  float stepsPerRev;
  float wheelDiameter;
  float wheelSpacing;
  long stepsPerTurn;
  
  long totalForwardSteps = 0;
  long totalTurnSteps = 0;
  
  // Heading and position by dead reckoning
  float heading = 0.0;  // clockwise from our starting direction pointing alog the x axis
  float xPos;
  float yPos;
  
  long targetSteps;
  boolean targetIsBigger;

  public:
  TwoWheel( AccelStepper *leftStepper, AccelStepper *rightStepper, float stepsPerRev, float wheelDiameter, float wheelSpacing );
  void setup();
  void loop();
  
  void enable( boolean on );
  
  void go( float distance );
  void turn( float degrees ); //  clockwise
  void turnToHeading( float targetHeading );
  
  boolean arrived();
  
  void goForever();
  void turnLeftForever();
  void turnRightForever();
  
  float stepsForDistance( float distance );
  float distanceForSteps( float steps );
  float stepsForTurn( float degrees ); //  clockwise
  
  float getHeading();

  
  
  
};

#endif 
