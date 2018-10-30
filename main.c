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
     -error at the second iteration
*/
int main() {
  // Initialize window and create field of dead cells
  startGame();
  // Create and refresh curses window
  win = derwin(stdscr, REAL_HEIGHT, REAL_WIDTH, Y_PADDING+1, X_PADDING+1);
  wrefresh(win);
  chtype in;
  bool predefinedFigure = false;
  // Initialize state using user input
  do {
    // Cell by cell manual selection
    in = getch();
    if (in == ' ') {
      waddch(win,winch(win) != ' ' ? ' ': BLOCK);
      rmove(0,-1);
      wrefresh(win);
    }
    else if(in == '1' || in == '2' || in == '3' || in == '4') {
      // Predefined figure selection
      wclear(win);
      wrefresh(win);
      //clearField();
      getPredefinedFigure(in);
      printFieldToSubwindow();
      predefinedFigure = true;
    }
    else {
      mymove(in);
    }
  }while(in !='\n'); // Start when 'ENTER' key is pressed

  if(!predefinedFigure)
    readSubwindow();

  bool changes = false;
  curs_set(0); // To disable cursor

  int count = 0;
  do {
    // These two lines are used two change the behaviour of getchar.
    // Now the function will not block the process until the user types.
    // Used to implement the PAUSE option.
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Compute next state and display it!
    changes = calculateNextState(); // Modify
    printFieldToSubwindow() ;
    wrefresh(win);
    usleep(100000);
    if(!handlePossibles())
      break;
    count++;
    //if ( count==300 )
    //usleep(10000000000000);
  } while (changes);

  // End of game: free allocated memory and close ncurses window
	freeField(field,REAL_HEIGHT,REAL_WIDTH);
  enditall();

  // Print some figures: dimensions of the field and number of iterations
  printf(" y min = %d\n",new_dim[0] );
  printf(" y max = %d\n",new_dim[1] );
  printf(" x min = %d\n",new_dim[2] );
  printf(" x max = %d\n",new_dim[3] );
  printf("real height = %d\n", REAL_HEIGHT);
  printf("real width = %d\n", REAL_WIDTH);
  printf("FIELD_POS = [%d]*[%d]\n",FIELD_POS[0],FIELD_POS[1]);
  printf("count = %d\n",count );
  return -1;
}

/*************************************************/
