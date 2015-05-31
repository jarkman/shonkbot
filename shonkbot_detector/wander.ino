
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_CRUISING 0
#define STATE_BACKING 1
#define STATE_TURNING 2

int state = STATE_CRUISING;

long clearSteps = -1;

void setupWander()
{
  startCruising();
}


void loopWander()
{

 int range = collisionDetector.getRangeInCm();

  switch( state )
  {
    case STATE_CRUISING:
      if( range != 0 )
      {
       // we see a thing! 
       startBacking();
      }
     
      
      break;
   
   case STATE_BACKING:
    if( twoWheel.arrived() && range == 0)
       startTurning();
    break;
    
   case STATE_TURNING:
     if (range == 0)    // nothing there there
     {
       if( twoWheel.arrived()) // turned enough
         startCruising();      // stop turning
       else
         ; // do nothing, still turning to do  
     }
     else
     {
       startTurning(); // still an obstacle, so we want to keep turning at least 90 more
     }
     break;
    
  }
  
  
}

void startCruising()
{
   Serial.println( "*****************************************************************************************************" );
   Serial.println( "startCruising " );

  state = STATE_CRUISING;
  twoWheel.goForever();
}

void startBacking()
{
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "startBacking " );

  state = STATE_BACKING;
  twoWheel.go( -25.0 ); // go back a bit to get away from the obstacle
}

void startTurning()
{
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "startTurning " );

  state = STATE_TURNING;  
  twoWheel.turn( -90.0 );  // turn left 90 degrees
}

/*
void turnLeft()
{
  leftStepper.moveTo( leftStepper.currentPosition() + stepsForDistance( 50 )); // distance is a bit irrelevant, we will calculate a new destination next time round the loop
  rightStepper.moveTo( rightStepper.currentPosition() + stepsForDistance( 50 ));
}

void go()
{
  leftStepper.moveTo( leftStepper.currentPosition()  - stepsForDistance( 50 ));
  rightStepper.moveTo( rightStepper.currentPosition() + stepsForDistance( 50 ));
}
*/

