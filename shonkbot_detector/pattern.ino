

class movement
{
  public:
  int turn;
  int distance;
  float curvature;
};
#define MAX_MOVEMENTS 40

movement movements[MAX_MOVEMENTS];

int nextMovement = 0;
boolean doneTurn = false;
boolean doneDistance = false;
int numMovements = 0;



void doFirstMovement()
{
   doneTurn = false;
    doneDistance = false;
    nextMovement = 0;
}

void doNextMovement()
{

  if( doneTurn && doneDistance )
  {
    doneTurn = false;
    doneDistance = false;
    nextMovement ++;
  }  
  
  if( nextMovement >= numMovements )
  {
    if( true )
    {
      // loop
      nextMovement = 0;
    }
    else
    {
      // finish when done
      twoWheel.enable( false ); // turn off motors
      return; // finished
    }
  }
  
  if( ! doneTurn )
  {
    doneTurn = true;
    twoWheel.turn( movements[nextMovement].turn );
  }
  else   if( ! doneDistance )
  {
    doneDistance = true;
    int distance = movements[nextMovement].distance;
    if( distance >= 0 )
      twoWheel.go( distance, movements[nextMovement].curvature );
    else
      twoWheel.goForever( );
  }
 
    
  
}


void addMovement( float turn, float distance )
{
  addMovement( turn, distance, 0.0 );
}


void addMovement( float turn, float distance, float curvature )
{
  if( numMovements >= MAX_MOVEMENTS )
    return;
    
  movements[numMovements].turn = turn;
  movements[numMovements].distance = distance;
  movements[numMovements].curvature = curvature;
  numMovements++;
  
}


void curve( int mm, float curvature )
{
  addMovement( 0, mm, curvature );
}

// Functions to match Scratch for users with a bit of Scratch experience
void move( int mm )
{
  addMovement( 0, mm );
}

void turnLeft( int degrees )
{
  addMovement( -degrees, 0 );
}

void turnRight( int degrees )
{
  addMovement( degrees, 0 );
}




