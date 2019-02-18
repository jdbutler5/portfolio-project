#include <stdio.h>
#include <curses.h>
#include <ncurses.h>
#include <menu.h>

char *choices[] = {
	"Choice 1",
	"Choice 2",
	"Choice 3",
	"Choice 4",
	"Exit",
};

int main(){
    ITEM **menu_items;
	MENU **menu;
	int n_choices;
	int c;
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	n_choices = 5;
	menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for (int i = 0; i < n_choices; i++)
		menu_items[i] = new_item(choices[i], choices[i]);
	menu_items[n_choices] = (ITEM *)NULL;
	
	menu = new_menu((ITEM **)menu_items);
	post_menu(menu);
	refresh();
	
	while((c = getch()) != KEY_F(1))
	{
		switch(c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
					break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
					break;
		}
		
	}
	
	
	
	free_item(menu_items[0]);
	free_item(menu_items[1]);
	free_menu(menu);
	endwin();
	
    return 0;
}
