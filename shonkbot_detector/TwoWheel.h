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
  
  long totalForwardSteps = 0;
  long totalTurnSteps = 0;
  long targetSteps;
  boolean targetIsBigger;

  public:
  TwoWheel( AccelStepper *leftStepper, AccelStepper *rightStepper, float stepsPerRev, float wheelDiameter, float wheelSpacing );
  void setup();
  void loop();
  
  void go( float distance );
  void turn( float degrees ); //  clockwise
  boolean arrived();
  
  void goForever();
  void turnLeftForever();
  void turnRightForever();
  
  float stepsForDistance( float distance );
  float stepsForTurn( float degrees ); //  clockwise

  
  
  
};

#endif 
