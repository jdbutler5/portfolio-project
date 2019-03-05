#include <stdio.h>
#include <curses.h>
#include <ncurses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//Array for storing menu options.
char *choices[] = {
	"Create",
	"Search",
	"Update",
	"Delete",
	"Exit",
};

//Array for storing menu option descriptions.
char *choice_desc[] = {
	"- Create a New IMDB Record",
	"- Search Through the IMDB Database",
	"- Update an IMDB Record",
	"- Delete an IMDB Record",
	"- Exit the Program"
};

int main(){
    ITEM **menu_items;
	MENU **menu;
	int n_choices, n_choice_desc;
	int c;
	
	FILE * fp;
	char line[1000];
	size_t len = 0;
	size_t read;
	fp = fopen("data.tsv", "r");
	line = calloc(1000);
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	
	n_choices = ARRAY_SIZE(choices);
	n_choice_desc = ARRAY_SIZE(choice_desc);
	menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
	
	for (int i = 0; i < n_choices; i++)
		menu_items[i] = new_item(choices[i], choice_desc[i]);
	
	menu_items[n_choices] = (ITEM *)NULL;
	
	menu = new_menu((ITEM **)menu_items);
	hline("ls", LINES - 4);
	post_menu(menu);
	refresh();
	
	bool exit_cond = false;
	
	while((c = getch()) != KEY_F(1) && exit_cond == false)
	{
		switch(c)
		{
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
					break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
					break;
			case 10: /*Enter*/
				
				mvprintw(LINES - 3, 0, "Your choice was %s", item_name(current_item(menu)));
				
				if(item_name(current_item(menu)) == "Exit"){
					exit_cond = true;
				}
				else if(item_name(current_item(menu)) == "Create"){
					
					if(fp == NULL){
						exit_cond = true;
					}
					mvprintw(LINES - 4, 0, "WOW YOU DID IT :')'");
					fscanf(fp, "%[^\n]", line);
					mvprintw(LINES - 5, 0, "Line 1: %c", line);
					/*while ((read = getline(&line, &len, fp)) != -1){
						printf("Retrieved line of length %zu:\n", read);
						printf("%s", line);
					}*/
				}
				
				break;
		}
		
	}
	
	
	fclose(fp);
	free_item(menu_items[0]);
	free_item(menu_items[1]);
	free_menu(menu);
	endwin();
	
    return 0;
}
