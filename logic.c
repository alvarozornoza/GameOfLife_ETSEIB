#include "conway.h"

int changes;      // Number of changes state k and state k+1
int new_dim[4];   // new_dim = [y_min,y_max,x_min,x_max]

/* 'new_dim' contains the coordinates of the upper and the lower live cells in
 * the field 'new_field' with respect to the old field reference system.
 */

bool ** convolution_2D(void) {
  /* Implementing 2D convolution in order to compute the new state of a cell
   * according to the state of its neighbors.
   */

  // Create a new empty field of same dimension as the precedent one
  bool **new_field;
  LIVE_CELLS = 0;
  new_field = createField(REAL_HEIGHT,REAL_WIDTH);

  int numberOfNeighbours;
  // For each cell...
  for (int i = 0; i < REAL_HEIGHT; i++){    // rows
    for (int j = 0; j < REAL_WIDTH; j++){   // columns
      // ...Compute the number of alive neighbors
      numberOfNeighbours = 0;
      for (int m = 0; m < 3; m++){          // kernel rows
        for (int n = 0; n < 3; n++){        // kernel column
          // Index of input signal, used for checking boundary
          int ii = i + (m - 1);
          int jj = j + (n - 1);
          // Ignore input samples which are out of bound
          if (ii >= 0 && ii < REAL_HEIGHT && jj >= 0 && jj < REAL_WIDTH){
            if(field[ii][jj] && !(m == 1 && n == 1))
               numberOfNeighbours++;
          }
        }
      }
      // ...Compute new state following the rules of the Game of Life
      if(field[i][j] && (numberOfNeighbours == 2 || numberOfNeighbours == 3))
         new_field[i][j] = true;
      else if(!field[i][j] && numberOfNeighbours == 3)
         new_field[i][j] = true;
      // ...If relevant, increment number of cells which state changed
      if(new_field[i][j] != field[i][j])
        changes++;
      // ...If relevant, increment number of alive cells and check dimensions
      if(new_field[i][j]){
        LIVE_CELLS++;
        check_dimensions(i,j);
      }
    }
  }
  return new_field;
}

void check_dimensions(int i, int j){
  /* Check if the live cell in the new field is an upper or lower cell.
   * If so, the dimensions of the field are updated.
   */
    if (i<new_dim[0])
      new_dim[0] = i;
    if (i>new_dim[1])
      new_dim[1] = i;
    if (j<new_dim[2])
      new_dim[2] = j;
    if (j>new_dim[3])
      new_dim[3] = j;
}

void getNewDimensions(){
  /* Compute required dimensions for the field.
   * The height and width are computed by calculating the number of required
   * lines/columns using the coordinates in 'new_dim' and adding 2 to have one
   * additional empty line/column on each side (to avoid losing cells at the
   * borders).
   */
  int thresh = 300;
  REAL_HEIGHT = new_dim[1]-new_dim[0]+1+2;
  REAL_WIDTH = new_dim[3]-new_dim[2]+1+2;

  /* Dimensions remain bounded between 1 and 'thresh' */
  if (REAL_HEIGHT < 1) {REAL_HEIGHT = 1;}
  if (REAL_WIDTH < 1) {REAL_WIDTH = 1;}
  if (REAL_HEIGHT >= thresh) {REAL_HEIGHT=thresh;}
  if (REAL_WIDTH >= thresh) {REAL_WIDTH=thresh;}
}

void updateFieldWithNextState (bool ** new_field, int old_height, int old_width){
  /* Store computed new state into global 'field' variable by copying new_field
   * to a field with correct dimensions.
   */
  int oldX;
  int oldY;
  for (int i = 0; i < REAL_HEIGHT; i++){    // rows
    for (int j = 0; j < REAL_WIDTH; j++){   // columns
      oldY = i + new_dim[0]-1;
      oldX = j + new_dim[2]-1;
      if(oldY>=0 && oldY<old_height && oldX >=0 && oldX<old_width){
        if (new_field[oldY][oldX])
          field[i][j] = true;
      }
    }
  }
}

bool calculateNextState() {
  /* Computing the next state of the simulation and saving it.
   */
  bool **new_field;             // new bool** object
  int old_height;
  int old_width;
  changes = 0;
  new_dim[0] = REAL_HEIGHT;     // initialize new_dim at each iteration
  new_dim[1] = 0;
  new_dim[2] = REAL_WIDTH;
  new_dim[3] = 0;

  // Compute next state and store it in the new variable
  new_field = convolution_2D();
  // Free memory containing old state
  freeField(field,REAL_HEIGHT, REAL_WIDTH);
  // Update dimensions of the field
  old_height = REAL_HEIGHT;
  old_width = REAL_WIDTH;
  getNewDimensions();
  // Store the new state in the global variable 'field'
  if ((old_height == REAL_HEIGHT) && (old_width == REAL_WIDTH)){
    // If dimensions do not change, take 'new_field' directly
    field = new_field;
  }
  else{
    // Else, create field with updated dimensions and copy content
    field = createField(REAL_HEIGHT,REAL_WIDTH);
    updateFieldWithNextState(new_field,old_height,old_width);
    freeField(new_field,old_height,old_width);
  }

  // Update coordinates of origin (i.e. position of the field wrt the window)
  FIELD_POS[0] = FIELD_POS[0]+new_dim[0]-1;
  FIELD_POS[1] = FIELD_POS[1]+new_dim[2]-1;
  // Flag for changes
  if(changes > 0)
    return true;
  else
    return false;
}
