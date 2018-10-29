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

int WIDTH, HEIGHT, REAL_WIDTH, REAL_HEIGHT, X_PADDING, Y_PADDING, LIVE_CELLS;
int FIELD_POS[2];
int new_dim[4];
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
void cleanMenu(void);
void getStats(void);

// logic functions
bool ** convolution_2D(void);
void getNewDimensions(void);
void updateFieldWithNextState(bool **, int, int);
bool calculateNextState(void);
bool handlePossibles(void);
void check_dimensions(int , int );
