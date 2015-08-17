
/*

Shonkbot with IR object detection and a bump-and-turn action for obstacle avoidance

See 
http://jarkman.co.uk/catalog/robots/shonkbot.htm for more details and 
https://github.com/jarkman/shonkbot/tree/master/shonkbot_detector for the current source.

You'll need the AccelStepper library, get it from 
http://www.airspayce.com/mikem/arduino/AccelStepper/index.html

*/


#include <AccelStepper.h>
#include "IRDetector.h"
#include "TwoWheel.h"


#define  DO_LOGGING



// Arduino pins wired to stepper drivers - driver board labels pins as IN1, IN2 etc
#define LEFT_IN1 2
#define LEFT_IN2 3
#define LEFT_IN3 4
#define LEFT_IN4 5

#define RIGHT_IN1 6
#define RIGHT_IN2 7
#define RIGHT_IN3 8
#define RIGHT_IN4 9


// Define a stepper and the pins it will use
// We use HALF4WIRE not FULL4WIRE because it results in lower overall current consumption (150mA per motor as opposed to 200mA)
AccelStepper leftStepper(AccelStepper::HALF4WIRE, LEFT_IN1,LEFT_IN3,LEFT_IN2,LEFT_IN4); // note middle two pins are swapped to work with 28BYJ-48 
                                                                                                                      
AccelStepper rightStepper(AccelStepper::HALF4WIRE, RIGHT_IN1,RIGHT_IN3,RIGHT_IN2,RIGHT_IN4); // note middle two pins are swapped!



#define PIEZO_PIN 12                        // Wire a piezo sounder from pin 12 to ground
#define COLLISION_LED_PIN 13                // Wire IR LED, long leg to pin 13, short leg to ground
#define SWARM_LED_PIN 11                    // Wire IR LED, long leg to pin 11, short leg to ground
#define COLLISION_PHOTOTRANSISTOR_PIN A0    // Wire IR phototransistor collector (short leg) to A0 , emitter (long leg) to ground, with a 10k pullup from A0 to +5

#define COLLISION_FREQUENCY 80              // Use 75 hz for collision detection
#define SWARM_FREQUENCY 30  // 

IRDetector collisionDetector(COLLISION_LED_PIN, COLLISION_PHOTOTRANSISTOR_PIN, PIEZO_PIN, COLLISION_FREQUENCY);
IRDetector swarmDetector(SWARM_LED_PIN, COLLISION_PHOTOTRANSISTOR_PIN, -1, SWARM_FREQUENCY);

// using 28BYj-48 motors from http://www.ebay.co.uk/itm/131410728499

#define STEPS_PER_REV (4096.0/2.0) // these motors come in two different gear rations, 
                              // see http://42bots.com/tutorials/28byj-48-stepper-motor-with-uln2003-driver-and-arduino-uno/ 
                              // and http://forum.arduino.cc/index.php?topic=71964.15

#define MAX_SPEED 800 // 1000 is a sensible limit on 3 x 1.5v battery, it's probably as much as we cna expect to get from AccelStepper anyhow
#define MAX_ACCELERATION 1000 // 1600 on 5v, 400 on 3v

#define WHEEL_DIAMETER 40.0   // radius of your wheel in mm - 40.00 is right for a blue wheel with a rubber band on
#define WHEEL_SPACING 150.0   // distance from one wheel to the other in mm

TwoWheel twoWheel( &leftStepper, &rightStepper, STEPS_PER_REV, WHEEL_DIAMETER, WHEEL_SPACING, MAX_SPEED );


void setup()
{  
  #ifdef DO_LOGGING
  Serial.begin (57600); // for debugging

  Serial.print ("setup\n");
  #endif
  
  randomSeed(analogRead(COLLISION_PHOTOTRANSISTOR_PIN));  // use ambient IR as a source of randomness
  
  collisionDetector.setup();
  swarmDetector.setup();
  twoWheel.setup();

  leftStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  leftStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  rightStepper.setMaxSpeed(MAX_SPEED); // 800 is a sensible limit on 5v motor supply, 300 is a sensible limit on 3v.
  rightStepper.setAcceleration(MAX_ACCELERATION); // 1600 on 5v
  
  setupWander();
}


void loop()
{
  collisionDetector.loop();
  swarmDetector.loop();
  
  loopWander();
  
  twoWheel.loop();
  
  
}

void buildPattern()
{
   //buildOneSquare();
   //buildPoly( random( 45, 170 ) );
   buildCurvahedron( random( 45, 170 ) );
   
  //buildStraightLine(); // use this for exploring behaviour
  // buildName();
  //buildSquares();
 
}

void buildPoly(int angle)
{
  int squareSide = 120;
  int side = (angle / 90.0) * squareSide;
  move( side );
  turnLeft( angle );
}

void buildCurvahedron(int angle)
{
  int squareSide = 120;
  int side = (angle / 90.0) * squareSide;
  
  float curvature = (random(100)/100.0) - 0.5; // set curvature to a random number between -0.5 and 0.5
  
  curve( side, curvature );
  turnLeft( angle );
}

void buildOneSquare()
{
  int squareSide = 120;
  
  move( squareSide );
  turnLeft( 90 );
  move( squareSide );
  turnLeft( 90 );
  move( squareSide );
  turnLeft( 90 );
  move( squareSide );
  turnLeft( 90 );
  
}


void buildStraightLine()
{
  addMovement( 0, -1 );
}



void buildSquares()
{
  int squareSide = 120;
  
  addMovement( -180, squareSide );
  addMovement( -90, squareSide );
  addMovement( -90, squareSide );
  addMovement( -90, squareSide );
  
  addMovement( 180, squareSide );
  addMovement( 90, squareSide );
  addMovement( 90, squareSide );
  addMovement( 90, squareSide );
}

void buildName()
{
  // S
  addMovement( 90, 40 );
  addMovement( -90, 40 );
  addMovement( -90, 40 );
  addMovement( 90, 40 );
  addMovement( 90, 40 );
  
  addMovement( 0, 10 );

  //h
  addMovement( 90, 80 );
  addMovement( 180, 40 );
  addMovement( 90, 40 );
  addMovement( 90, 40 );
  
  addMovement( -90, 20 );
  
  //o
  addMovement( 0, 40 );
  addMovement( -90, 40 );
  addMovement( -90, 40 );
  addMovement( -90, 40 );
  addMovement( -90, 40 );
  
  addMovement( 0, 20 );

  //n
  addMovement( -90, 50 );
  addMovement( 180, 10 );
  addMovement( -90, 40 );
  addMovement( 90, 40 );
  
  addMovement( -90, 20 );
  
  //k
  addMovement( -90, 80 );
  addMovement( 180, 40 );
  addMovement( -135, 40 );
  addMovement( 180, 40 );
  addMovement( -90, 40 * 1.414 );
  
  addMovement( -45, 20 );
  
  //y
  addMovement( -90, 40 );
  addMovement( 180, 40 );
  addMovement( -90, 40 );
  addMovement( -90, 40 );
  addMovement( 180, 80 );
  addMovement( 90, 20 );

  
}





