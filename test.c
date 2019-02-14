#include <stdio.h>
#include <curses.h>
#include <ncurses.h>

int main(){
    initscr();
	printw("Hello World !!!");
	refresh();
	getch();
	endwin();
	
    return 0;
}
