#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "figures.h"

// CONSTANTS
#define BLOCK  ' ' | A_REVERSE
#define ESCAPE_BUTTON 27

int WIDTH, HEIGHT, REAL_WIDTH, REAL_HEIGHT, X_PADDING, Y_PADDING;
WINDOW *win;
bool **field;

//init functions
int initall(void);
int enditall(void);
int freeField(bool **, int, int);
_Bool **createField(int , int );
void readSubwindow(void);
void setPaddingAndReals();
void startGame(void);
void drawSquare(void);

//disp functions
void clearField(void);
void printFieldToSubwindow(void);
void printMenu(void);
void resetWindow(void);
void rmove(int,int);
void mymove(chtype);
void readfileAndPrint(char *);
void getPredefinedFigure(chtype);

// logic functions
void convolution_2D(int numberOfNeighbours[][REAL_WIDTH]);
bool updateFieldWithNextState(int numberOfNeighbours[][REAL_WIDTH]);
bool calculateNextState(void);
bool handlePossibles(void);
