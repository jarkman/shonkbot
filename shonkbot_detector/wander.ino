
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_CRUISING 0
#define STATE_TURNING 1

int state = STATE_CRUISING;

long clearSteps = -1;

void setupWander()
{
}


void loopWander()
{

 int range = collisionDetector.getRangeInCm();

  switch( state )
  {
    case STATE_CRUISING:
      if( range != 0 )
      {
       
       state = STATE_TURNING;
       clearSteps = -1;
      }
      else
      {
        go();
      }
      
      break;
      
   case STATE_TURNING:
     if (range == 0)
     {
       if( leftStepper.currentPosition() > clearSteps )
       {  
         // turn is finished
         state = STATE_CRUISING;
       }
       else
       {
         turnLeft();
       }
     }
     else
     {
       clearSteps = leftStepper.currentPosition() + stepsForTurn( 90 ); // so we go 90 degrees after we last see an obstacle
       turnLeft();
     }
    break;
  }
  
}

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


