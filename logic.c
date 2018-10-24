
#include "conway.h"

void convolution_2D(int numberOfNeighbours[][REAL_WIDTH]) {
  for (int i = 0; i < REAL_HEIGHT; i++){    // rows
    for (int j = 0; j < REAL_WIDTH; j++){ // columns
      numberOfNeighbours[i][j] = 0;
      for (int m = 0; m < 3; m++){     // kernel rows
        for (int n = 0; n < 3; n++){ // kernel column
          // index of input signal, used for checking boundary
          int ii = i + (m - 1);
          int jj = j + (n - 1);

          // ignore input samples which are out of bound
          if (ii >= 0 && ii < REAL_HEIGHT && jj >= 0 && jj < REAL_WIDTH){
            if(field[ii][jj] && !(m == 1 && n == 1))
               numberOfNeighbours[i][j]++;
          }
        }
      }
    }
  }
}

bool updateFieldWithNextState(int numberOfNeighbours[][REAL_WIDTH]) {
  bool **new_field;
  int changes = 0;
  new_field = createField(REAL_HEIGHT,REAL_WIDTH);
  for (int i = 0; i < REAL_HEIGHT; i++) {    // rows
    for (int j = 0; j < REAL_WIDTH; j++) {   // columns
      if(field[i][j] && (numberOfNeighbours[i][j] == 2 || numberOfNeighbours[i][j] == 3))
         new_field[i][j] = true;
      else if(!field[i][j] && numberOfNeighbours[i][j] == 3)
         new_field[i][j] = true;
      if(new_field[i][j] != field[i][j])
        changes++;
    }
  }
	freeField(field,HEIGHT-Y_PADDING-2, WIDTH-X_PADDING-2);
  field = new_field;
  if(changes > 0)
    return true;
  else
    return false;
}

bool calculateNextState(){
  int numberOfNeighbours[REAL_HEIGHT][REAL_WIDTH];
  convolution_2D(numberOfNeighbours);
  return updateFieldWithNextState(numberOfNeighbours);
}


bool handlePossibles(){
  chtype ch = getch();
  if(ch == ESCAPE_BUTTON)
    return false;
  else if(ch == 'P' || ch == 'p'){
    fflush(stdin);
    scrollok(stdscr, FALSE);
    nodelay(stdscr, FALSE);
    while(getch() != ('p' || 'P'))
      return true;
  }
  else
    return true;
}
