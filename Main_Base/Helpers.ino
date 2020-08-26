/*
 * Functions fo converting cartesian x and y motion
 * to corexy movements for each motor
 */
 
int calculateStepsLeft(int deltax,int deltay){
  int steps = -(-deltay+deltax);
  return steps;
}

int calculateStepsRight(int deltax, int deltay){
  int steps = (-deltay-deltax);
  return steps;
}
