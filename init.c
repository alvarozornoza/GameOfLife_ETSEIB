#include "conway.h"

int initall(void)
{
	initscr();			/* Start curses mode 		  */
	return 0;
};

int enditall(void)
{
	endwin();			/* End curses mode 		  */
	return 0;
};

int freeField(bool **f, int sqy, int sqx)
{
	/*Free allocated memory for the field */
	for (int j = 0; j < sqy; j++) {
		free(f[j]);
	}
	free(f);
	return 0;
}

/* create the initial field of dimension sqy * sqx */
bool **createField(int  sqy, int sqx) {
  bool **f = (bool **)malloc(sizeof(bool *) * sqy);
  for (int j = 0; j < sqy; j++) {
    f[j] = (bool *)malloc(sizeof(bool) * sqx);
  }
  for (int y = 0; y < sqy; y++) {
    for (int x = 0; x < sqx; x++)
      f[y][x] = false;
  }
  return f;
}


void readSubwindow() {
  for (int y = 0; y < HEIGHT-Y_PADDING-2; y++) {
    for (int x = 0; x < WIDTH-X_PADDING-2; x++) {
      field[y][x] = mvwinch(win,y,x) != ' ' ? true: false;
    }
  }
}

void setPaddingAndReals(){
  X_PADDING = 0;
  Y_PADDING = 3;
  REAL_WIDTH = WIDTH-X_PADDING-2;
  REAL_HEIGHT = HEIGHT-Y_PADDING-2;
	FIELD_POS[0] = 0; // height 
	FIELD_POS[1] = 0; // width
}

void startGame() {
  initall();
  resetWindow();
  refresh(); /* Print it on to the real screen */
  field = createField(HEIGHT-Y_PADDING-2, WIDTH-X_PADDING-2);
  noecho();
}

/**
 * Draws the border of the field game:
 * + for vertexes
 * | or - for edges
*/
void drawSquare() {
  int xx, yy;
  getmaxyx(stdscr, yy, xx);
  WIDTH = xx;
  HEIGHT = yy;
  setPaddingAndReals();
  int limitx = WIDTH - 1;
  int limity = HEIGHT - 1;

  // Drawing corners of the subwindow
  mvaddch(Y_PADDING, X_PADDING, '+');
  mvaddch(limity, X_PADDING, '+');
  mvaddch(Y_PADDING, limitx, '+');
  mvaddch(limity, limitx, '+');

  // Drawing borders of the subwindow
  for (int y = Y_PADDING + 1; y < limity; y++) {
    mvaddch(y, X_PADDING, '|');
    mvaddch(y, limitx, '|');
  }
  for (int x = X_PADDING + 1; x < limitx; x++) {
    mvaddch(Y_PADDING, x, '-');
    mvaddch(limity, x, '-');
  }
}
