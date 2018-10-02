#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "conway.h"

// CONSTANTS
const int block = ' '|A_REVERSE;
const long period = 1000*500; // MICROS
// Main variables
int WIDTH;
int HEIGHT;
WINDOW *win;

/**
 * Draws the border of the field game.
 * + for vertexes
 * | or - for edges
 */
void drawSquare(){
	int xx, yy;
	getmaxyx(stdscr,yy,xx);

	WIDTH = xx-2;
	HEIGHT = yy-3;
	printw("W :%d, H :%d",WIDTH,HEIGHT);
	int xpadding=0;
	int ypadding=1;
	int topx = xpadding+WIDTH+1;
	int topy = ypadding+HEIGHT+1;
	// Drawing top-bottom ends
	mvaddch(ypadding,xpadding,'+');
	mvaddch(topy,xpadding,'+');
	mvaddch(ypadding,topx,'+');
	mvaddch(topy,topx,'+');
	// Drawing borders
	for(int x=xpadding+1; x < topx; x++){
		mvaddch(ypadding,x,'-');
		mvaddch(topy,x,'-');
	}
	for(int y=ypadding+1; y < topy; y++){
		mvaddch(y,xpadding,'|');
		mvaddch(y,topx,'|');
	}
}

/**
 * Erases the value of the ncurses window.
 * Prints header
 * Prints field border
 */
void resetWindow(){
	erase();
	printw("Conway's Game of Live");	/* Print Hello World		  */
	drawSquare();
}

/**
 * creates the global field variable.
 * and sets to blank spaces
 */

void startGame()
{
  initall();
  resetWindow();
  refresh();			/* Print it on to the real screen */
}




int main()
{
  startGame();
  win =subwin(stdscr, HEIGHT, WIDTH, 2, 1);
  do{
    resetWindow();
    mvwaddch(win,0,0,block);
    mvwaddch(win,5,0,block);
    wprintw(win,"aqui");

    mvwaddch(win,5,5,block);
    mvwaddch(win,15,0,block);
    wrefresh(win);
    refresh();
    usleep(1000);
  }while(false);

  refresh();			/* Print it on to the real screen */
  //readSubwindow(win);


  getch();			/* Wait for user input */
  mvwprintw(win,4,4,"NEW");
  mvprintw(4,2,"QSA");
  wrefresh(win);
  refresh();
  getch();
  enditall();
//  readSubwindow(win);
//  printFieldToStdout();
}




/*************************************************/
