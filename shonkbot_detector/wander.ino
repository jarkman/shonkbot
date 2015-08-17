
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_SELFTEST 0
#define STATE_CRUISING 1
#define STATE_BACKING 2
#define STATE_TURNING 3
#define STATE_LOOK_FOR_DARK 4
#define STATE_TURNING_TO_DARK 5

//#define DO_WANDER_LOGGING

int state = STATE_SELFTEST;
int selftestPhase = 0;

int turningLeft = 0;
boolean reversingBeepOn;

long clearSteps = -1;
int reversingPiezoPin = PIEZO_PIN;

//#define HIDE_IN_THE_DARK  // define this to make shonkbot look for a dark place 

int darkestHeading;
int brightnessAtDarkestHeading;

void setupWander()
{
  state = STATE_SELFTEST;
  selftestPhase = 0;
  buildPattern();
}

boolean sawShonkbot = false;

void loopWander()
{

 int collisionRange = collisionDetector.getRangeInCm();
 int swarmRange = swarmDetector.getRangeInCm();

  boolean seeObject = collisionRange != 0 && collisionRange < 20;
  boolean seeShonkbot = swarmRange != 0;
  
  boolean seeNewShonkbot = seeShonkbot && ! sawShonkbot;
  
  sawShonkbot = seeShonkbot;
  
  seeNewShonkbot = false; // this line turns off all attempts at swarming
  
  switch( state )
  {
    case STATE_SELFTEST:
      doSelftest();
      break;
        
    case STATE_CRUISING:
      if( seeNewShonkbot )
      {
        doFirstMovement(); // start heading towards it
      }
      else if( seeObject )
      {
       // we see a thing! 
       startBacking();
      }
      else
      {
        if (twoWheel.arrived())
          doNextMovement();
      }
      
      break;
   
   case STATE_BACKING:
   
     reversingBeep();
        
    // did we back far enough ?    
    if( twoWheel.arrived() )
    {
       
       noTone(reversingPiezoPin); // finish any left-over reversing beep
       
       #ifdef HIDE_IN_THE_DARK
       
         startLookingForDark();
       
       #else
         
         startTurning();
       
       #endif
       
       
    }
    break;
    
   case STATE_TURNING:
     if ( ! seeObject)    // nothing there 
     {
       if( seeNewShonkbot )
      {
        doFirstMovement(); // start heading towards it
      }
      else if( twoWheel.arrived()) // turned enough
         startCruising();      // stop turning
       else
         ; // do nothing, still turning to do  
     }
     else
     {
       keepTurning(); // still an obstacle, so we want to keep turning
     }
     break;
     
    case STATE_LOOK_FOR_DARK:
      keepLookingForDark(collisionRange);
      break;
      
    case STATE_TURNING_TO_DARK:
      if( twoWheel.arrived() )
        startCruising();
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
      tone(reversingPiezoPin, 8000);
      twoWheel.turn( 20 );
    }
  }
  else if( selftestPhase == 2 )
  {
    if( twoWheel.arrived() )
    {
      noTone(reversingPiezoPin);
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

void reversingBeep()
{
  // reversing beeps
  // beep for the first 0.5s of each 1.5s period
      long t = millis() % 1000;
      if( t < 200 && t > 0 )
      {
        //if( ! reversingBeepOn )
        {
          tone(reversingPiezoPin, 5000);
          reversingBeepOn = true;
        }
      }
      else
      {
        if( reversingBeepOn )
        {
          noTone(reversingPiezoPin);  
          reversingBeepOn = false;
        }
      }
}

void startCruising()
{
  #ifdef DO_WANDER_LOGGING
   Serial.println( "*****************************************************************************************************" );
   Serial.println( "startCruising " );
#endif

  state = STATE_CRUISING;
  doNextMovement();
  //twoWheel.goForever();
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

void startLookingForDark()
{
#ifdef DO_WANDER_LOGGING
  Serial.println( "*****************************************************************************************************" );
   Serial.println( "startLookForDark " );
#endif

  state = STATE_LOOK_FOR_DARK;
  
  darkestHeading = twoWheel.getHeading()+180;
  brightnessAtDarkestHeading = 1024; // very bright


  turningLeft = random( 0, 2 ); // 0 or 1
  
  int angle = 360;
  
  if( turningLeft )
    angle = -angle;
    
  twoWheel.turn( angle );  

}

void keepLookingForDark(int range)
{
  if( ! twoWheel.arrived() )
  {
    // must still be doing our 360 scan - keep loooking for the darkest direction
   int brightness = collisionDetector.getAmbientBrightness(); 
 
   if( brightness < brightnessAtDarkestHeading && range == 0) // this direction is darker, so remember it
   {
     brightnessAtDarkestHeading = brightness;
     darkestHeading = twoWheel.getHeading();
     
      #ifdef DO_WANDER_LOGGING
     Serial.print( "keepLookingForDark new darkest is at " );
     Serial.println(twoWheel.getHeading());
     #endif
     
   }
  }
  else
  {
    
    int currentHeading = twoWheel.getHeading();
    
    int turnDegrees = (darkestHeading - currentHeading) % 360;
    
    #ifdef DO_WANDER_LOGGING
    Serial.println( "*****************************************************************************************************" );
    Serial.print( "keepLookingForDark done, turning from " );
    Serial.print(currentHeading);
    Serial.print( " to ");
    Serial.print(darkestHeading);
    Serial.print( " so turning by " );
    Serial.println( turnDegrees );
    #endif

    // have completed scan, so turn to the darkest direction then start cruising
    state = STATE_TURNING_TO_DARK;
    
    twoWheel.turnToHeading( darkestHeading );  
  }
  
}

