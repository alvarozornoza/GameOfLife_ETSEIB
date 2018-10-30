#include "conway.h"

int initall(void)
{
	initscr();			/* Start curses mode */
	return 0;
};

int enditall(void)
{
	endwin();			/* End curses mode */
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

bool **createField(int  sqy, int sqx) {
	/* Create the initial field of dimension sqy*sqx.
	 *   sqy: # of lines
	 *	 sqx: # of columns
	 * The field is a double-pointer structure of booleans. 'true' will represent
	 * alive cells and 'false' dead ones.
	 */
  bool **f = (bool **) malloc(sizeof(bool *) * sqy);
  for (int j = 0; j < sqy; j++) {
    f[j] = (bool *)malloc(sizeof(bool) * sqx);
  }
  for (int y = 0; y < sqy; y++) {
    for (int x = 0; x < sqx; x++)
      f[y][x] = false;										/* Initially, all cells are dead. */
  }
  return f;
}


void readSubwindow() {
	/* This funcion is used to retrieve the initial state entered by the user.
	 * A space ' ' character is used to indicate an alive cell.
	 */
	  for (int y = 0; y < HEIGHT-Y_PADDING-2; y++) {
    for (int x = 0; x < WIDTH-X_PADDING-2; x++) {
      field[y][x] = mvwinch(win,y,x) != ' ' ? true: false;
    }
  }
}

void setPaddingAndReals(){
	/* This function initialize the values of the global variables. */
  X_PADDING = 0;
  Y_PADDING = 4;		// Used for header display
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

void drawSquare() {
	/* Draws the border of the field:
	 * 		+ for vertexes
	 * 		| for vertical edges, - for horizontal edges
	 */
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
