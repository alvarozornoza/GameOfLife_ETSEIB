#include "conway.h"



void clearField(){
  for (int y = 0; y < REAL_HEIGHT; y++) {
    for (int x = 0; x < REAL_WIDTH; x++)
      field[y][x] = false;
  }
}

void printFieldToSubwindow() {
  int x_field, y_field;
  wmove(win, 0, 0);
  wrefresh(win);
  for (int y = 0; y < HEIGHT-Y_PADDING-2; y++) {
    for (int x = 0; x < WIDTH-X_PADDING-2; x++) {
      y_field = y - FIELD_POS[0];
      x_field = x - FIELD_POS[1];
      if(y_field<REAL_HEIGHT && y_field>=0 && x_field<REAL_WIDTH && x_field>=0){
        mvwaddch(win, y, x, field[y_field][x_field] ? BLOCK : ' ');
      }
      else{mvwaddch(win, y, x,' ');}
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

void cleanMenu(){
  move(0,0); // to clean the input menu
  wclrtoeol(stdscr);
  move(1,0);
  wclrtoeol(stdscr);
  move(2,0);
  wclrtoeol(stdscr);

}

void getStats(){
  mvwprintw(stdscr,0,0,"LIVE_CELLS = %d",LIVE_CELLS);
  mvwprintw(stdscr,1,0,"REAL_HEIGHT = %d, REAL_WIDTH = %d",REAL_HEIGHT,REAL_WIDTH);
  mvwprintw(stdscr,2,0,"FIELD_POS = [ %d, %d]",FIELD_POS[0],FIELD_POS[1]);
  wrefresh(stdscr);
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
