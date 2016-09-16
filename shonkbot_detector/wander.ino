
// Wander - cruise about, turning when an object is detected ahead of us

#define STATE_SELFTEST 0
#define STATE_CRUISING 1
#define STATE_BACKING 2
#define STATE_TURNING 3
#define STATE_LOOK_FOR_DARK 4
#define STATE_TURNING_TO_DARK 5
#define STATE_CONSOLE_DRIVEN 6  // Set by Forth console to disable wandering

//#define DO_WANDER_LOGGING

// It's helpful if the size matches a YAFFA cell, hence the uint16_t.
uint16_t wanderState = STATE_SELFTEST;
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
  wanderState = STATE_SELFTEST;
  selftestPhase = 0;
  buildPattern();
}

boolean sawShonkbot = false;
boolean seeShonkbot = false;

void loopWander()
{
  boolean seeObject = false;
  int swarmRange = 0;

 int collisionRange = collisionDetector.getRangeInCm();
 seeObject = collisionRange != 0 && collisionRange < 20;
 
   #ifdef DO_SWARM
    swarmRange = swarmDetector.getRangeInCm();
    seeShonkbot = swarmRange != 0;
  #else
    seeShonkbot = false;
  #endif
  
  
  
  if( seeObject && seeShonkbot )
    if( collisionRange < swarmRange ) // if we see both, take the closest 
      seeShonkbot = false;
    else
      seeObject = false;
      
  boolean seeNewShonkbot = seeShonkbot && ! sawShonkbot;
  
  sawShonkbot = seeShonkbot;
  
  seeNewShonkbot = false; // this line turns off all attempts at swarming
  
  switch( wanderState )
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
    
    case STATE_CONSOLE_DRIVEN:
      if (twoWheel.arrived()) {
        twoWheel.enable(false);
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

#define PGMT(pgm_ptr) (reinterpret_cast<const __FlashStringHelper *>(pgm_ptr))
const char debugBanner[] PROGMEM = "*****************************************************************************************************";

void startCruising()
{
  if (debug) {
    Serial.println(PGMT(debugBanner));
    Serial.println(F("startCruising "));
  }

  wanderState = STATE_CRUISING;
  doNextMovement();
  //twoWheel.goForever();
}

void startBacking()
{
  if (debug) {
    Serial.println(PGMT(debugBanner));
    Serial.println(F("startBacking "));
  }

  wanderState = STATE_BACKING;
  twoWheel.go( -25.0 ); // go back a bit to get away from the obstacle
}

void startTurning()
{
  if (debug) {
    Serial.println(PGMT(debugBanner));
    Serial.println(F("startTurning "));
  }

  wanderState = STATE_TURNING;
  turningLeft = random( 0, 2 ); // 0 or 1
  
  keepTurning();

}

void keepTurning()
{
  if (debug) {
    Serial.println(PGMT(debugBanner));
    Serial.println(F("keepTurning "));
  }

  // Called at the start of a turn and when we find we still have an obstacle during a turn
  int angle = random(10, 90);
  if( turningLeft )
    angle = -angle;
    
  twoWheel.turn( angle );  // turn by a random amount
}

void startLookingForDark()
{
  if (debug) {
    Serial.println(PGMT(debugBanner));
    Serial.println(F("startLookForDark "));
  }

  wanderState = STATE_LOOK_FOR_DARK;
  
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
     
     if (debug) {
       Serial.print(F("keepLookingForDark new darkest is at "));
       Serial.println(twoWheel.getHeading());
     }
     
   }
  }
  else
  {
    if (debug) {
      int currentHeading = twoWheel.getHeading();
    
      int turnDegrees = (darkestHeading - currentHeading) % 360;

      Serial.println(PGMT(debugBanner));
      Serial.print(F("keepLookingForDark done, turning from "));
      Serial.print(currentHeading);
      Serial.print(F(" to "));
      Serial.print(darkestHeading);
      Serial.print(F(" so turning by "));
      Serial.println( turnDegrees );
    }

    // have completed scan, so turn to the darkest direction then start cruising
    wanderState = STATE_TURNING_TO_DARK;
    
    twoWheel.turnToHeading( darkestHeading );  
  }
  
}

