/*

This version is with field of boolean. I changed all the functions in order to work with booleans and I create two functions:
two more functions:
  - setField to create a new field from getInput.
  - getPrestations to calculate the mean time to commute each block and print the field.
The code with fiel of chtype is below.

*/

#include "conway.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// CONSTANTS
#define live  ' ' | A_REVERSE
#define dead ' '
const long period = 1000 * 500; // MICROS
// Main variables
int WIDTH;
int HEIGHT;
WINDOW *win;
_Bool **field;
int xx;
int yy;
int nextMove;
_Bool sow = true;

/**
 * WORKS
 */



_Bool **createField(int sqy, int sqx) {
  _Bool **f = (_Bool **)malloc(sizeof(_Bool *) * sqy);
  for (int j = 0; j < sqy; j++) {
    f[j] = (_Bool *)malloc(sizeof(_Bool) * sqx);
    // Init to spaces
  }

  for (int y = 0; y < sqy; y++) {
    for (int x = 0; x < sqx; x++)
      f[y][x] = 0;
      //  ' ';
  }
  //getInputF(f);
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
      if(field[j][i]){
      mvwaddch(w, j, i, live);
      }
      else{mvwaddch(w,j,i,dead);}

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
  printw("Conway's Game of Live"); // Print Hello World
  drawSquare();
}

/**
 * creates the global field variable.
 * and sets to blank spaces
 */

void startGame() {
  initall();
  resetWindow();
  refresh(); // Print it on to the real screen
  field = createField(HEIGHT, WIDTH);

}

void setField(_Bool **field) {
  xx = 0;
  yy = 0;
  while(sow){
    nextMove = getch();
    switch (nextMove) {
      case KEY_UP:
           if( xx>0){
             --xx;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_DOWN:
           if(xx<HEIGHT){
             ++xx;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_RIGHT:
           if(yy<WIDTH){
             ++yy;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_LEFT:
           if(yy>0){
             --yy;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;

      case 10: // enter key

           //field[xx][yy] = (field[xx][yy]==live) ? live:dead;
           field[xx][yy] = !field[xx][yy];

           //field[xx][yy] = live;
           printToSubwindow(win);
           break;
      case 263: //backspace key
           sow = false;
           break;
    }
}

}
double getPrestations() {

    clock_t start, end;
    double prest = 0;

    for(int k = 0; k<100; k++){
      start = clock();
      for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
          field[j][i] = !field[j][i];
          printToSubwindow(win);
        }
      }
      end = clock();
      prest += (double) (end-start)/CLOCKS_PER_SEC;
    }
    return prest/100;

}

int main() {
  double prest;
  startGame();
  win = subwin(stdscr, HEIGHT, WIDTH, 2, 1);

  keypad(stdscr,true);
  noecho();
  setField(field);
  printToSubwindow(win);
  refresh();

  prest = getPrestations();
  getch();
  do {
    readSubwindow(win); // get state.
    // TODO calculateNextState(); // Modify
    printToSubwindow(win);
    refresh();
    usleep(1000);
  } while (false);

  enditall();
  free(field);
  printf("Time needed (my case) = %f\n",prest );
  //  readSubwindow(win);
  //  printFieldToStdout();
}

/*************************************************/
/*
CODE WITH CHRTYPE
*/
/*
#include "conway.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// CONSTANTS
#define live  ' ' | A_REVERSE
#define dead ' '
const long period = 1000 * 500; // MICROS
// Main variables
int WIDTH;
int HEIGHT;
WINDOW *win;
chtype **field;
int xx;
int yy;
int nextMove;
_Bool sow = true;





chtype **createField(int sqy, int sqx) {
  chtype **f = (chtype **)malloc(sizeof(chtype *) * sqy);
  for (int j = 0; j < sqy; j++) {
    f[j] = (chtype *)malloc(sizeof(chtype) * sqx);
    // Init to spaces
  }

  for (int y = 0; y < sqy; y++) {
    for (int x = 0; x < sqx; x++)
      f[y][x] = dead;
      //  ' ';
  }
  //getInputF(f);
  return f;
}

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


void readSubwindow(WINDOW *w) {
  for (int j = 0; j < HEIGHT; j++) {
    // printf("%d:\t%s \n",i,(char*) field[i]);
    for (int i = 0; i < WIDTH; i++) {
      field[j][i] = mvwinch(w,j,i);
    }
  }
}

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
 /*
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
 /*
void resetWindow() {
  erase();
  printw("Conway's Game of Live"); // Print Hello World
  drawSquare();
}

/**
 * creates the global field variable.
 * and sets to blank spaces
 */
/*
void startGame() {
  initall();
  resetWindow();
  refresh(); // Print it on to the real screen
  field = createField(HEIGHT, WIDTH);

}

void setField(chtype **field) {
  xx = 0;
  yy = 0;
  while(sow){
    nextMove = getch();
    switch (nextMove) {
      case KEY_UP:
           if( xx>0){
             --xx;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_DOWN:
           if(xx<HEIGHT){
             ++xx;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_RIGHT:
           if(yy<WIDTH){
             ++yy;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;
      case KEY_LEFT:
           if(yy>0){
             --yy;
             wmove(win,xx,yy);
             wrefresh(win);
           }
           break;

      case 10: // enter key

           //field[xx][yy] = (field[xx][yy]==live) ? live:dead;
           if( (int)field[xx][yy] == (int)' ' ) {
             field[xx][yy] = live;
           }
           else{ field[xx][yy] = dead;}

           //field[xx][yy] = live;
           printToSubwindow(win);
           break;
      case 263: //backspace key
           sow = false;
           break;
    }
}

}
double getPrestations() {

    clock_t start, end;

    double prest = 0;
    for (int k = 0; k < 100; k++){
      start = clock();
      for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
          if( (int)field[j][i] == (int)' ' ) {
            field[j][i] = live;
          }
          else{ field[j][i] = dead;}
          printToSubwindow(win);
        }
      }
      end = clock();
       prest += (double) (end-start)/CLOCKS_PER_SEC;
    }
    return prest/100;

}

int main() {
  double prest;
  startGame();
  win = subwin(stdscr, HEIGHT, WIDTH, 2, 1);

  keypad(stdscr,true);
  noecho();
  setField(field);
  printToSubwindow(win);
  refresh();

  prest = getPrestations();
  getch();
  do {
    readSubwindow(win); // get state.
    // TODO calculateNextState(); // Modify
    printToSubwindow(win);
    refresh();
    usleep(1000);
  } while (false);

  enditall();
  free(field);
  printf("Time needed (juancarlos case) = %f\n",prest );
  //  readSubwindow(win);
  //  printFieldToStdout();
}
*/
/*************************************************/
