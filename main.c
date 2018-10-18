#include "conway.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CONSTANTS
#define block  ' ' | A_REVERSE
const long period = 1000 * 500; // MICROS
// Main variables
int WIDTH;
int HEIGHT;
WINDOW *win;
chtype **field;

/**
 * WORKS
 */
chtype **createField(int sqy, int sqx) {
  chtype **f = (chtype **)malloc(sizeof(chtype *) * sqy);
  for (int j = 0; j < sqy; j++) {
    f[j] = (chtype *)malloc(sizeof(chtype) * sqx);
    // Init to spaces
  }
  for (int y = 0; y < sqy; y++) {
    for (int x = 0; x < sqx; x++)
      f[y][x] = block; //  ' ';
  }
  return f;
}
/**
 * WORKS
 */
void printFieldToStdout() {
  for (int j = 0; j < HEIGHT; j++) {
    // printf("%d:\t%s \n",i,(char*) field[i]);
    printf("%d:\t", j);
    for (int i = 0; i < WIDTH; i++) {
      printf("%d ", (int)field[j][i] - ' ');
    }
    printf("\n");
  }
}

/**
 * Fill the field global variable of values printed on the screen.
 * TODO
 */
void readSubwindow(WINDOW *w) {
  for (int j = 0; j < HEIGHT; j++) {
    // printf("%d:\t%s \n",i,(char*) field[i]);
    for (int i = 0; i < WIDTH; i++) {
      field[j][i] = mvwinch(w,j,i);
    }
  }
}
/**
 * Fills the window with the values of the field
 */
void printToSubwindow(WINDOW *w) {
  wmove(w, 0, 0);
  wrefresh(win);
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      mvwaddch(w, j, i, field[j][i]);
    }
  }
  wrefresh(win);
}
/**
 * Draws the border of the field game.
 * + for vertexes
 * | or - for edges
 */
void drawSquare() {
  int xx, yy;
  getmaxyx(stdscr, yy, xx);
  WIDTH = xx - 2;
  HEIGHT = yy - 3;
  printw("W :%d, H :%d", WIDTH, HEIGHT);
  int xpadding = 0;
  int ypadding = 1;
  int topx = xpadding + WIDTH + 1;
  int topy = ypadding + HEIGHT + 1;
  // Drawing top-bottom ends
  mvaddch(ypadding, xpadding, '+');
  mvaddch(topy, xpadding, '+');
  mvaddch(ypadding, topx, '+');
  mvaddch(topy, topx, '+');
  // Drawing borders
  for (int x = xpadding + 1; x < topx; x++) {
    mvaddch(ypadding, x, '-');
    mvaddch(topy, x, '-');
  }
  for (int y = ypadding + 1; y < topy; y++) {
    mvaddch(y, xpadding, '|');
    mvaddch(y, topx, '|');
  }
}

/**
 * Erases the value of the ncurses window.
 * Prints header
 * Prints field border
 */
void resetWindow() {
  erase();
  printw("Conway's Game of Live"); /* Print Hello World		  */
  drawSquare();
}

/**
 * creates the global field variable.
 * and sets to blank spaces
 */

void startGame() {
  initall();
  resetWindow();
  refresh(); /* Print it on to the real screen */
  field = createField(HEIGHT, WIDTH);
  noecho();
}

void rmove(int b, int a){
	int y,x;
	getyx(stdscr,y,x);
	move(y+b,x+a);
}

void mymove(chtype c){
  switch(c)
  {
	  case 'w':
    case 'W':
		  rmove(-1,0);break;
	  case 's':
    case 'S':
		  rmove(1,0);break;
	  case 'd':
    case 'D':
		  rmove(0,1);break;
	  case 'a':
    case 'A':
		  rmove(0,-1);break;
  }
}

int main() {
  startGame();
  win = subwin(stdscr, HEIGHT, WIDTH, 2, 1);
  // TODO setField();
  mvwaddch(win, 0, 0, block);
  wprintw(win, "aqui");
  wrefresh(win);
  move(3,2);
  chtype in = getch();
  // INIT
  while(in !=' '){
    in = getch();
    mymove(in);
    if (in == '\n') {
      addch(block);
      rmove(0,-1);
    }
    wrefresh(win);
    refresh();
  }

  getch();

  printToSubwindow(win);
  refresh();
  getch();
  do {
    readSubwindow(win); // get state.
    // TODO calculateNextState(); // Modify
    printToSubwindow(win);
    refresh();
    usleep(1000);
  } while (false);
  getch(); /* Wait for user input */
  enditall();
  //  readSubwindow(win);
  //  printFieldToStdout();
}

/*************************************************/
