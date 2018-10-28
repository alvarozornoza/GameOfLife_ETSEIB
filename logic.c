
#include "conway.h"
int changes;
int new_dim[4];

bool ** convolution_2D(void) {
  bool **new_field;
  new_field = createField(REAL_HEIGHT,REAL_WIDTH);
  int numberOfNeighbours;
  for (int i = 0; i < REAL_HEIGHT; i++){    // rows
    for (int j = 0; j < REAL_WIDTH; j++){ // columns
      numberOfNeighbours = 0;
      for (int m = 0; m < 3; m++){     // kernel rows
        for (int n = 0; n < 3; n++){ // kernel column
          // index of input signal, used for checking boundary
          int ii = i + (m - 1);
          int jj = j + (n - 1);

          // ignore input samples which are out of bound
          if (ii >= 0 && ii < REAL_HEIGHT && jj >= 0 && jj < REAL_WIDTH){
            if(field[ii][jj] && !(m == 1 && n == 1))
               numberOfNeighbours++;
          }
        }
      }
      if(field[i][j] && (numberOfNeighbours == 2 || numberOfNeighbours == 3))
         new_field[i][j] = true;
      else if(!field[i][j] && numberOfNeighbours == 3)
         new_field[i][j] = true;
      if(new_field[i][j] != field[i][j])
        changes++;
      check_dimensions(i,j,new_field[i][j]);
      }
  }
  return new_field;
}



void check_dimensions(int i, int j, bool isAlive){
  if (isAlive){
    if (i<new_dim[0])
      new_dim[0] = i;
    if (i>new_dim[1])
      new_dim[1] = i;
    if (j<new_dim[2])
      new_dim[2] = j;
    if (j>new_dim[3])
      new_dim[3] = j;
  }
}

void getNewDimensions(){
  REAL_HEIGHT = new_dim[1]-new_dim[0]+1+2;
  REAL_WIDTH = new_dim[3]-new_dim[2]+1+2;
  if (REAL_HEIGHT >= 500) {REAL_HEIGHT=500;} //threshold
  if (REAL_WIDTH >= 500) {REAL_WIDTH=500;} //threshold
}

void updateFieldWithNextState (bool ** new_field, int old_height, int old_width){
  int oldX;
  int oldY;
  for (int i = 0; i < REAL_HEIGHT; i++){    // rows
    for (int j = 0; j < REAL_WIDTH; j++){
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
  bool **new_field;
  int old_height;
  int old_width;
  changes = 0;
  new_dim[0] = REAL_HEIGHT;
  new_dim[1] = 0;
  new_dim[2] = REAL_WIDTH;
  new_dim[3] = 0;
  new_field = convolution_2D();

  freeField(field,REAL_HEIGHT, REAL_WIDTH);

  old_height = REAL_HEIGHT;
  old_width = REAL_WIDTH;
  getNewDimensions();
  if ((old_height == REAL_HEIGHT) && (old_width == REAL_WIDTH)){
    field = new_field;
  }
  else{
    field = createField(REAL_HEIGHT,REAL_WIDTH);
    updateFieldWithNextState(new_field,old_height,old_width);
    freeField(new_field,old_height,old_width);
  }
  FIELD_POS[0] = FIELD_POS[0]+new_dim[0]-1;
  FIELD_POS[1] = FIELD_POS[1]+new_dim[2]-1;
  if(changes > 0)
    return true;
  else
    return false;
}
