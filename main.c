#include "conway.h"
/*
 * WORKS
*/
/*
 * Fill the field global variable of values printed on the screen.
 * TODO
*/

/*
   TODO:
	   - esc dosen't work if the game is not started (before pressing ENTER)
     - OPTIONAL : make the field dynamic, I ll try this next days.
*/
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

      waddch(win,winch(win) != ' ' ? ' ': BLOCK);
      rmove(0,-1);
      wrefresh(win);
    }
    else if(in == '1' || in == '2' || in == '3' || in == '4'){
      wclear(win);
      wrefresh(win);
      //clearField();
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
    printFieldToSubwindow() ;
    wrefresh(win);
    usleep(100000);
    if(!handlePossibles())
      break;
  } while (changes);

	freeField(field,HEIGHT-Y_PADDING-2, WIDTH-X_PADDING-2);
  enditall();
  return -1;
}

/*************************************************/
