#include "conway.h"



void clearField(){
  for (int y = 0; y < REAL_HEIGHT; y++) {
    for (int x = 0; x < REAL_WIDTH; x++)
      field[y][x] = false;
  }
}

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




/*
 * Prints menu lines
*/
void printMenu(){
  printw("Conway's Game of Live\n");
  printw("Press W (up), S (down), A(right) or D(left) to move, SPACE to fill the cell, ENTER to start, P to pause aland ESC to exit\n");
  printw("Press 1 to print figure R-Pentomino, Diehard (2), Acorn (3), Gosper Glider (4)\n");
}

/*
 * Erases the value of the ncurses window.
 * Prints header
 * Prints field border
*/
void resetWindow() {
  erase();
  printMenu();
  drawSquare();
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
