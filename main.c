#include "conway.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "figures.h"

	
#define ESCAPE_BUTTON 27

// CONSTANTS
#define BLOCK  ' ' | A_REVERSE
const long period = 1000 * 500; // MICROS
// Main variables
int WIDTH, HEIGHT, REAL_WIDTH, REAL_HEIGHT, X_PADDING, Y_PADDING;

WINDOW *win;
bool **field;

/**
 * WORKS
 */
bool **createField(int sqy, int sqx) {
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

void clearField(){
  for (int y = 0; y < REAL_HEIGHT; y++) {
    for (int x = 0; x < REAL_WIDTH; x++)
      field[y][x] = false;
  }
}

/**
 * Fill the field global variable of values printed on the screen.
 * TODO
 */
void readSubwindow() {
  for (int y = 0; y < HEIGHT-Y_PADDING-2; y++) {
    for (int x = 0; x < WIDTH-X_PADDING-2; x++) {
      field[y][x] = mvwinch(win,y,x) != ' ' ? true: false;
    }
  }
}
/**
 * Fills the window with the values of the field
 */
void printFieldToSubwindow() {
  wmove(win, 0, 0);
  wrefresh(win);
  for (int y = 0; y < REAL_HEIGHT; y++) {
    for (int x = 0; x < REAL_WIDTH; x++) {
      mvwaddch(win, y, x, field[y][x] ? BLOCK : ' ');
    }
  }
  wmove(win, 0, 0);
  wrefresh(win);
}


void setPaddingAndReals(){
  X_PADDING = 0;
  Y_PADDING = 3;

  REAL_WIDTH = WIDTH-X_PADDING-2;
  REAL_HEIGHT = HEIGHT-Y_PADDING-2;
}

/**

 * Draws the border of the field game.
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
/**
 * Prints menu lines
 */

void printMenu(){
  printw("Conway's Game of Live\n");
  printw("Press W (up), S (down), A(right) or D(left) to move, SPACE to fill the cell, ENTER to start, P to pause aland ESC to exit\n");
  printw("Press 1 to print figure R-Pentomino, Diehard (2), Acorn (3), Gosper Glider (4)\n");
}

/**
 * Erases the value of the ncurses window.
 * Prints header
 * Prints field border
 */
void resetWindow() {
  erase();
  printMenu();
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
  field = createField(HEIGHT-Y_PADDING-2, WIDTH-X_PADDING-2);
  noecho();
}

void rmove(int b, int a){
	int y,x;
	getyx(win,y,x);
  wmove(win,y+b,x+a);
  wrefresh(win);
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

void readfileAndPrint(char* name){
	int fd;
	//if((fd = open(name, O_RDONLY)) == -1)
	fd = open(name, O_RDONLY);
  int figure_height = 0;
	int figure_width = 0;
  int width_aux = 0;

	char buf;
	while(read(fd, &buf, 1)){  // Analyzing contents of the file
		if(buf == '\n'){
			figure_height++;
      figure_width = width_aux;
      width_aux = 0;
    }
    else
			width_aux++;
	}
	lseek(fd, 0, SEEK_SET);

	int x = 0, y = 0;  // Reading the values and put them in the field
	while(read(fd, &buf, 1)){
		if(buf == '\n'){
			y++;
			x=0;
		} else {
			x++;
			if(buf == '1')
				field[REAL_HEIGHT/2-figure_height/2+y][REAL_WIDTH/2-figure_width/2+x] = true;
		}
	}
	close(fd);
}

void getPredefinedFigure(chtype in){
    switch (in){
        case '1':
          readfileAndPrint(ACORN);
        break;
        case '2':
          readfileAndPrint(DIEHARD);
        break;
        case '3':
          readfileAndPrint(RPENTOMINO);
        break;
        case '4':
          readfileAndPrint(GOSPER_GLIDER);
        break;
    }
}

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

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
  fflush(stdin);
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
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


int main() {
  startGame();
  win = derwin(stdscr, REAL_HEIGHT, REAL_WIDTH, Y_PADDING+1, X_PADDING+1);
  wrefresh(win);
  chtype in;
  bool predefinedFigure = false;
  // INIT
  do {
    in = getch();
    if (in == ' ') {
      waddch(win,BLOCK);
      rmove(0,-1);
      wrefresh(win);
    }
    else if(in == '1' || in == '2' || in == '3' || in == '4'){
      wclear(win);
      wrefresh(win);
      clearField();
      getPredefinedFigure(in);
      printFieldToSubwindow();
      predefinedFigure = true;
    }
    else{
      mymove(in);
    }
  }while(in !='\n');
  
  if(!predefinedFigure)
    readSubwindow();

  bool changes = false;
  do {
    readSubwindow();
    curs_set(0); // To disable cursor
    scrollok(stdscr, TRUE); // These two lines are used two change the behaviour of getchar. Now the function will not block the process until the user types.
    nodelay(stdscr, TRUE);
    changes = calculateNextState(); // Modify
    printFieldToSubwindow();
    wrefresh(win);
    usleep(100000);
    if(!handlePossibles())
      break;
  } while (changes);
  enditall();
  return -1;
}

/*************************************************/
