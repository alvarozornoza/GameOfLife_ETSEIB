#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int initall(void)
{
	initscr();			/* Start curses mode 		  */
	return 0;
};

int enditall(bool **f, int sqy, int sqx)
{
	endwin();			/* End curses mode 		  */
	return 0;
};

int freeField(bool **f, int sqy, int sqx)
{
	/*Free allocated memory for the field */
	for (int j = 0; j < sqy; j++) {
		free(f[j]);
	}
	free(f);
	return 0;
}
