
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_SELFTEST 3
#define STATE_CRUISING 0
#define STATE_BACKING 1
#define STATE_TURNING 2

#define DO_WANDER_LOGGING

int selftestPhase = 0;
int state = STATE_SELFTEST;
int turningLeft = 0;

long clearSteps = -1;

void setupWander()
{
  state = STATE_SELFTEST;
  selftestPhase = 0;
  loopWander();
//  startCruising();
}


void loopWander()
{

  int range = sonar.ping_cm();
//  Serial.println(state);
  //Serial.println(range);
  //Serial.println();
  switch( state )
  {

    case STATE_SELFTEST:
      doSelftest();
      break;
      
    case STATE_CRUISING:
      if( range > 0 && range < MAX_DISTANCE )
      {
       // we see a thing! 
       Serial.println("BACKING !!! ");
       //Serial.println(range);
       startBacking();
      }
     
      
      break;
   
   case STATE_BACKING:
   
        
    // did we back far enough ?    
    if( twoWheel.arrived() )
    {
       startTurning();
    }
    break;
    
   case STATE_TURNING:
     if (range == 0)    // nothing there there
     {
       if( twoWheel.arrived()){ // turned enough
         startCruising();
         Serial.println("cruising");// stop turning
       }
       else{
         ; // do nothing, still turning to do  
       }
     }
     else
     {
       keepTurning(); // still an obstacle, so we want to keep turning
     }
     break;
    
  }
  
  
}

void doSelftest()
{
  if( selftestPhase == 0 )
  {
    selftestPhase = 1;
    twoWheel.turn( -10 );
  }
  else if( selftestPhase == 1 )
  {
    if( twoWheel.arrived() )
    {
      selftestPhase = 2;
      twoWheel.turn( 20 );
    }
  }
  else if( selftestPhase == 2 )
  {
    if( twoWheel.arrived() )
    {
      selftestPhase = 3;
      twoWheel.turn( -10 );
    }
  }
  else if( selftestPhase == 3 )
  {
    if( twoWheel.arrived() )
      startCruising();
  }
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

