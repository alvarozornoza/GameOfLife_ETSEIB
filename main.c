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
  curs_set(0); // To disable cursor
  scrollok(stdscr, TRUE); // These two lines are used two change the behaviour of getchar. Now the function will not block the process until the user types.
  nodelay(stdscr, TRUE);
  int count = 0;
  do{
    changes = calculateNextState(); // Modify
    printFieldToSubwindow() ;
    wrefresh(win);
    cleanMenu();
    getStats();
    usleep(300000);
    if(!handlePossibles())
      break;
    //count++;
    //if (count==300)
    //  break;
  } while (changes);



	freeField(field,REAL_HEIGHT,REAL_WIDTH);
  enditall();
/*
  printf(" y min = %d\n",new_dim[0] );
  printf(" y max = %d\n",new_dim[1] );
  printf(" x min = %d\n",new_dim[2] );
  printf(" x max = %d\n",new_dim[3] );
  printf("real height = %d\n", REAL_HEIGHT);
  printf("real width = %d\n", REAL_WIDTH);
  printf("FIELD_POS = [%d]*[%d]\n",FIELD_POS[0],FIELD_POS[1]);
  printf("count = %d\n",count );
  */
  return -1;
}

/*************************************************/
