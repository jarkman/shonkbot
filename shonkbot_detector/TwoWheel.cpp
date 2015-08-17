#include "TwoWheel.h"



TwoWheel::TwoWheel( AccelStepper *_leftStepper, AccelStepper *_rightStepper, float _stepsPerRev, float _wheelDiameter, float _wheelSpacing, int _maxSpeed )
{
  leftStepper = _leftStepper;
  rightStepper = _rightStepper;
  stepsPerRev = _stepsPerRev;
  wheelDiameter = _wheelDiameter;
  wheelSpacing = _wheelSpacing;
  maxSpeed = _maxSpeed;
  stepsPerTurn = stepsForTurn( 360.0 );
  heading  = 0.0;
  xPos = 0.0;
  yPos = 0.0;
}

void TwoWheel::setup()
{}

void TwoWheel::loop()
{
  long lastLeftSteps = leftStepper->currentPosition();
  long lastRightSteps = rightStepper->currentPosition();
  
  leftStepper->run();
  rightStepper->run();
  
  long newLeftSteps = leftStepper->currentPosition();
  long newRightSteps = rightStepper->currentPosition();
  
  // Keep track of how far we've gone forwards and clockwise
  
  // Right stepper is positive for forwards, left stepper is negative for forwards
  long forwardSteps = (newRightSteps - lastRightSteps) - (newLeftSteps - lastLeftSteps);
  
  if( forwardSteps != 0 )
  {
      float distance = distanceForSteps( forwardSteps );
      float headingRadians = radians( heading );
      
      xPos += distance * cos( headingRadians );
      yPos += distance * sin( headingRadians );
  }
  
  totalForwardSteps += forwardSteps;
  
  // Both steppers are negative for a clockwise turn
  totalTurnSteps -=  (newRightSteps - lastRightSteps) + (newLeftSteps - lastLeftSteps);
  
  heading = (((float)totalTurnSteps) / ((float)stepsPerTurn)) * 360.0; // keep track of our overall heading in degrees
  heading = fmod( heading, 360 ); // keep in the range -360m to 360
}

void TwoWheel::enable( boolean on )
{
  if( on )
  {
    leftStepper->enableOutputs();
    rightStepper->enableOutputs();
  }
  else
  {
    leftStepper->disableOutputs();
    rightStepper->disableOutputs();
  }
}

void TwoWheel::go( float distance )
{
  go( distance, 0.0  );
}

void TwoWheel::go( float distance, float curvature )
{
  float steps = stepsForDistance( distance );
  
  targetIsBigger = steps < 0;
  
  /*
  Serial.println( "*****************************************************************************************************" );
   Serial.print( "go targetSteps " );
   Serial.print( targetSteps );
        
   Serial.print( " targetIsBigger " );
   Serial.println( targetIsBigger );
 */
 
 // avoid extreme curvatures that make the maths go wrong
  if( curvature < -0.8 )
    curvature = -0.8;
  if( curvature > 0.8 )
    curvature = 0.8;
    
  float leftRatio = 1.0 - curvature;
  float rightRatio = 1.0 + curvature;
  
  float leftSteps = -steps*leftRatio;
  
  targetSteps = leftStepper->currentPosition() + leftSteps;

  //TODO - could avoid overspeed with extreme curvature by scaling down both speeds here ?
  leftStepper->setMaxSpeed(maxSpeed*leftRatio);
  rightStepper->setMaxSpeed(maxSpeed*rightRatio);
 
  leftStepper->move(leftSteps);
  rightStepper->move(steps*rightRatio);
}

void TwoWheel::turnToHeading( float targetHeading ) // turn to the specified heading, where 0 means the way we were facing when we booted up
{

  float turnDegrees = (targetHeading - heading);
  turn( turnDegrees );
}

void TwoWheel::turn( float degrees ) //  turn by 'degrees' clockwise (pass a negative number to turn anticlockwise)
{
  float theta = radians(degrees);
  
  float distance = wheelSpacing * 0.5 * theta ;
  float steps = stepsForDistance( distance );
  
  targetSteps = leftStepper->currentPosition() - steps;
  targetIsBigger = steps < 0;

/*
    Serial.println( "*****************************************************************************************************" );
   Serial.print( "turn targetSteps " );
   Serial.print( targetSteps );
        
   Serial.print( " targetIsBigger " );
   Serial.println( targetIsBigger );
   */
     
  leftStepper->setMaxSpeed(maxSpeed);
  rightStepper->setMaxSpeed(maxSpeed);
     
  leftStepper->move(-steps);
  rightStepper->move(-steps);
}
  
float TwoWheel::getHeading()
{
  return heading;
}

boolean TwoWheel::arrived()
{
  long pos = leftStepper->currentPosition();
  
  if( targetIsBigger )
    return pos >= targetSteps-1;
  else
    return pos <= targetSteps+1;
}
  
void TwoWheel::goForever()
{
  //leftStepper->move(-1000);
  //rightStepper->move(1000);
  
  targetSteps = -MAX_STEPS;
  targetIsBigger = false;
  
  #ifdef DEBUG
     Serial.println( "*****************************************************************************************************" );
   Serial.println( "goForever " );
 #endif
 
  leftStepper->setMaxSpeed(maxSpeed);
  rightStepper->setMaxSpeed(maxSpeed);
  
  leftStepper->move(-maxSpeed);
  rightStepper->move(maxSpeed);
}
  
void TwoWheel::turnLeftForever()
{
  
#ifdef DEBUG
      Serial.println( "*****************************************************************************************************" );
   Serial.println( "goForever " );
#endif

    targetSteps = MAX_STEPS;
  targetIsBigger = true;

  leftStepper->setMaxSpeed(maxSpeed);
  rightStepper->setMaxSpeed(maxSpeed);
  
  leftStepper->move(MAX_STEPS);
  rightStepper->move(MAX_STEPS);
}

void TwoWheel::turnRightForever()
{
      
#ifdef DEBUG
   Serial.println( "*****************************************************************************************************" );
   Serial.println( "goForever " );
#endif

    targetSteps = -MAX_STEPS;
    targetIsBigger = false;

  leftStepper->setMaxSpeed(maxSpeed);
  rightStepper->setMaxSpeed(maxSpeed);
  
  leftStepper->move(-MAX_STEPS);
  rightStepper->move(-MAX_STEPS);

}
  
  

float TwoWheel::stepsForDistance( float distance )
{
  return       distance * 
          ( 2 *                         // because we're half-stepping the motors with HALF4WIRE
          stepsPerRev ) / 
          (3.1415 * wheelDiameter);  // circumference
}


float TwoWheel::distanceForSteps( float steps )
{
  return steps * (3.1415 * wheelDiameter) / (2 * stepsPerRev );                       
          
         
}

float TwoWheel::stepsForTurn( float degrees ) //  clockwise
{
  float theta = radians(degrees);
  
  float distance = wheelSpacing * 0.5 * theta ;
  float steps = stepsForDistance( distance );
  
  return steps;
}  


