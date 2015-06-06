

// Execute a fixed set of movements


float currentDirection = 0.0; // in radians clockwise from the Y axis

// Current position in mm, assuming we started pointing up the Y axis
float currentX = 0.0;
float currentY = 0.0;

class movement
{
  public:
  int turn;
  int distance;
};
#define MAX_MOVEMENTS 40

movement movements[MAX_MOVEMENTS];

int nextMovement = 0;
boolean doneTurn = false;
boolean doneDistance = false;
int numMovements = 0;


void setupScript()
{
  buildMovements();
  doNextMovement();
}

void loopScript()
{
    
  if (twoWheel.arrived())
    doNextMovement();
 
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
    twoWheel.enable( false ); // turn off motors
    return; // finished
  }
  
  if( ! doneTurn )
  {
    doneTurn = true;
    twoWheel.turn( movements[nextMovement].turn );
  }
  else   if( ! doneDistance )
  {
    doneDistance = true;
    twoWheel.go( movements[nextMovement].distance );
  }
 
    
  
}


void addMovement( float turn, float distance )
{
  if( numMovements >= MAX_MOVEMENTS )
    return;
    
  movements[numMovements].turn = turn;
  movements[numMovements].distance = distance;
  numMovements++;
  
}

void buildMovements()
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




