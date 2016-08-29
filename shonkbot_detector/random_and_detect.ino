
//randomly go about, then go into colour detect mode

#define STATE_STILL 6
#define STATE_RANDOMWALK 7
#define STATE_WAGGLEDANCE 8

int rstate = STATE_WAGGLEDANCE;
int wagglePhase = 0;
int randomPhase = 0;


int piezoPin = PIEZO_PIN;


void setupRandomWalk()
{
  Serial.println("setup random walk");

  rstate = STATE_WAGGLEDANCE;
  wagglePhase = 0;
  randomPhase = 0;
}

void loopRandom()
{
  
  switch( rstate )
  {
   case STATE_WAGGLEDANCE:
      doWaggleDance();
      break;  
        
    case STATE_RANDOMWALK:
      doRandom();      
      break;

  }
   
}



void doRandom(){

  int toMove = random(150);
  int toTurn = random(360);
  if( randomPhase == 0 )
  {
    randomPhase = 1;
    twoWheel.turn( toTurn );
    Serial.print("turning ");
    Serial.println(toTurn);
  }
  else if( randomPhase == 1 )
  {
    if( twoWheel.arrived() )
    {
      twoWheel.go( toMove );
      Serial.print("moving ");
      Serial.println(toMove);
      randomPhase = 2;
    }
  } else if( randomPhase == 2 )
  {
    if( twoWheel.arrived() )
    {
      //check colour here?
      randomPhase = 0;
      twoWheel.enable( false );
      if(toMove < 20){
         Serial.println("moving to state waggledance");
         rstate = STATE_WAGGLEDANCE;
      }
    }
  }
  
}

void doWaggleDance()
{
  if( wagglePhase == 0 )
  {
    wagglePhase = 1;
    twoWheel.turn( -10 );
  }
  else if( wagglePhase == 1 )
  {
    if( twoWheel.arrived() )
    {
      wagglePhase = 2;
      tone(piezoPin, 8000);
      twoWheel.turn( 20 );
    }
  }
  else if( wagglePhase == 2 )
  {
    if( twoWheel.arrived() )
    {
      noTone(piezoPin);
      wagglePhase = 3;
      twoWheel.turn( -10 );
    }
  }
  else if( wagglePhase == 3 )
  {
    if( twoWheel.arrived() ){
      wagglePhase = 0;
      Serial.println("moving to state random");
       rstate = STATE_RANDOMWALK;
      doRandom();

    }
  }
}


