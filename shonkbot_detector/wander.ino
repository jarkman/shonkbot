
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_CRUISING 0
#define STATE_BACKING 1
#define STATE_TURNING 2

//#define DO_WANDER_LOGGING

int state = STATE_CRUISING;
int turningLeft = 0;

long clearSteps = -1;
int reversingPiezoPin = PIEZO_PIN;

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
   
     reversingBeep();
        
    // did we back far enough ?    
    if( twoWheel.arrived() )
    {
       startTurning();
       noTone(reversingPiezoPin); // finish any left-over reversing beep
    }
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
       keepTurning(); // still an obstacle, so we want to keep turning
     }
     break;
    
  }
  
  
}

void reversingBeep()
{
  // reversing beeps
  // beep for the first 0.5s of each 1.5s period
      long t = millis() % 1000;
      if( t < 200 && t > 0 )
        tone(reversingPiezoPin, 5000);
      else
        noTone(reversingPiezoPin);  
}

void startCruising()
{
  #ifdef DO_WANDER_LOGGING
   Serial.println( "*****************************************************************************************************" );
   Serial.println( "startCruising " );
#endif

  state = STATE_CRUISING;
  twoWheel.goForever();
}

void startBacking()
{
#ifdef DO_WANDER_LOGGING
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "startBacking " );
#endif

  state = STATE_BACKING;
  twoWheel.go( -25.0 ); // go back a bit to get away from the obstacle
}

void startTurning()
{
#ifdef DO_WANDER_LOGGING
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "startTurning " );
#endif

  state = STATE_TURNING;  
  turningLeft = random( 0, 2 ); // 0 or 1
  
  keepTurning();

}

void keepTurning()
{
#ifdef DO_WANDER_LOGGING
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "keepTurning " );
#endif

  // Called at the start of a turn and when we find we still have an obstacle during a turn
  int angle = random(10, 90);
  if( turningLeft )
    angle = -angle;
    
  twoWheel.turn( angle );  // turn by a random amount
}

