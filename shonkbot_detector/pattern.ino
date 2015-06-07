

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
      twoWheel.go( distance );
    else
      twoWheel.goForever( );
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





