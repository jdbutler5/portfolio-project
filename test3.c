#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//struct for movie node
struct movieNode
{
	int key;
	char * title;
	char * genre;
	char * runningTime;
	char * yearReleased;
	struct movieNode *left;
	struct movieNode *right;
	//int height;
};

//Array for storing menu options.
char *choices[] = {
	"Add",
	"Update",
	"Delete",
	"Exit",
};

//Array for storing menu option descriptions.
char *choice_desc[] = {
	"- Add a movie to your catalog",
	"- Update your catalog",
	"- Delete a movie from your catalog",
	"- Exit the Program"
};

//Comparison typedef.
typedef int (*Compare) (const char *, const char *);

//Function for inserting into the binary search tree.
void insert(char* key, char * genres, char * runningTimeMinutes, char * startYear, struct movieNode** leaf, Compare cmp)
{
	int res;
	if (*leaf == NULL)
	{
		*leaf = (struct movieNode*)malloc(sizeof(struct movieNode));
		(*leaf)->title = malloc(strlen (key) + 1);
		strcpy((*leaf)->title, key);
		
		(*leaf)->genre = malloc(strlen (genres) + 1);
		strcpy((*leaf)->genre, genres);
		
		(*leaf)->runningTime = malloc(strlen (runningTimeMinutes) + 1);
		strcpy((*leaf)->runningTime, runningTimeMinutes);
		
		(*leaf)->yearReleased = malloc(strlen (startYear) + 1);
		strcpy((*leaf)->yearReleased, startYear);
		
		(*leaf)->left = (*leaf)->right = NULL;
	}
	else
	{
		res = cmp(key, (*leaf)->title);
		if(res < 0)
			insert(key, genres, runningTimeMinutes, startYear, &(*leaf)->left, cmp);
		else if(res > 0)
			insert(key, genres, runningTimeMinutes, startYear, &(*leaf)->right, cmp);
		//else
			//printf("Key '%s' already in tree.", key);
	}
}

//compares value of new node against previous
int cmpStr(const char *a, const char *b)
{
	return (strcmp(a,b));
}

//Prints out the entire BST in order.
void in_order(struct movieNode *root)
{
	if(root != NULL)
	{
		in_order(root->left);
		
		//printf("   %s | %s | %s | %s \n", root->title, root->genre, root->runningTime, root->yearReleased);
		printf("%s\n", root->title);
		
		in_order(root->right);
	}
}

//Search for a specific node.
void search(char* key, struct movieNode* leaf, Compare cmp)
{
	int res;
	if(leaf != NULL)
	{
		res = cmp(key, leaf->title);
		if(res < 0)
			search(key, leaf->left, cmp);
		else if(res > 0)
			search(key, leaf->right, cmp);
		else
			printf("\n'%s' found!\n", key);
	}
	else printf("\nNot in tree.\n");
	return;
}

//search for nodes with a search term of the value of key. returns a BST of all nodes containing the key in their title.
void search2(char* key, struct movieNode* leaf, Compare cmp, struct movieNode ** searchRoot)
{
	int res;
	char * p;
	char * k;
	
	if(leaf != NULL)
	{
		res = cmp(key, leaf->title);
		//printf("%s", leaf->title);
		if(res < 0)
		{
			//If the search key doesn't match exactly, see if any entries in the left leaf contain a part of the search string
			p = strstr(leaf->title, key);
			if(p != NULL)
			{
				//If they do, add it to the searchRoot mini-BST
				
				if (*searchRoot == NULL)
				{
					*searchRoot = (struct movieNode*)malloc(sizeof(struct movieNode));
					(*searchRoot)->title = malloc(strlen (leaf->title) + 1);
					strcpy((*searchRoot)->title, leaf->title);
					
					(*searchRoot)->genre = malloc(strlen (leaf->genre) + 1);
					strcpy((*searchRoot)->genre, leaf->genre);
					
					(*searchRoot)->runningTime = malloc(strlen (leaf->runningTime) + 1);
					strcpy((*searchRoot)->runningTime, leaf->runningTime);
					
					(*searchRoot)->yearReleased = malloc(strlen (leaf->yearReleased) + 1);
					strcpy((*searchRoot)->yearReleased, leaf->yearReleased);
					
					(*searchRoot)->left = (*searchRoot)->right = NULL;
				}
				else
					insert(leaf->title, leaf->genre, leaf->runningTime, leaf->yearReleased, &(*searchRoot), (Compare)cmpStr);
			}
			//if not, continue searching down the node's leaves
			search2(key, leaf->left, cmp, &(*searchRoot));
			search2(key, leaf->right, cmp, &(*searchRoot));
		}
		else if(res > 0)
		{
			k = strstr(leaf->title, key);
			if(k != NULL)
			{
				if (*searchRoot == NULL){
					*searchRoot = (struct movieNode*)malloc(sizeof(struct movieNode));
					(*searchRoot)->title = malloc(strlen (leaf->title) + 1);
					strcpy((*searchRoot)->title, leaf->title);
					
					(*searchRoot)->genre = malloc(strlen (leaf->genre) + 1);
					strcpy((*searchRoot)->genre, leaf->genre);
					
					(*searchRoot)->runningTime = malloc(strlen (leaf->runningTime) + 1);
					strcpy((*searchRoot)->runningTime, leaf->runningTime);
					
					(*searchRoot)->yearReleased = malloc(strlen (leaf->yearReleased) + 1);
					strcpy((*searchRoot)->yearReleased, leaf->yearReleased);
					
					(*searchRoot)->left = (*searchRoot)->right = NULL;
				}
				else
					insert(leaf->title, leaf->genre, leaf->runningTime, leaf->yearReleased, &(*searchRoot), (Compare)cmpStr);
			}
			search2(key, leaf->left, cmp, &(*searchRoot));
			search2(key, leaf->right, cmp, &(*searchRoot));
		}
		else
		{
			//printf("\n%s", key);
			insert(leaf->title, leaf->genre, leaf->runningTime, leaf->yearReleased, &(*searchRoot), (Compare)cmpStr);
		}
	}
	
	return (*searchRoot);
}


//Deletes a specified BST.
void delete_tree(struct movieNode** leaf)
{
	if(*leaf != NULL)
	{
		delete_tree(&(*leaf)->left);
		delete_tree(&(*leaf)->right);
		free((*leaf)->title);
		free((*leaf)->genre);
		free((*leaf)->runningTime);
		free((*leaf)->yearReleased);
		free((*leaf));
	}
}

int main()
{
    FILE * file;
    long fsize;
	
	ITEM **menu_items;
	MENU **menu;
	int n_choices, n_choice_desc;
	int c;
	
    file = fopen("movie_records","r");
	// file = fopen("datatest2.txt","r");
    if(file != NULL)
	{

        //get file size
        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);
		
		//variable definitions
		char ** tConst = malloc(fsize*10*sizeof(char));
		char ** titleType = malloc(fsize*10*sizeof(char));
		char ** primaryTitle = malloc(fsize*100*sizeof(char));
		char ** originalTitle = malloc(fsize*100*sizeof(char));
		char ** isAdult = malloc(fsize*sizeof(char));
		char ** startYear = malloc(fsize*5*sizeof(char));
		char ** endYear = malloc(fsize*5*sizeof(char));
		char ** runtimeMinutes = malloc(fsize*100*sizeof(char));
		char ** genres = malloc(fsize*100*sizeof(char));

        // print
        char * file_content = (char*)malloc(fsize);
		//int i = 0;
		struct movieNode *root = NULL;
		
		while(fgets(file_content,fsize,file))
		{
			//puts(file_content);
			sscanf(file_content, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//sscanf(file_content, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//printf("%s %s %s %s %s %s %s %s %s\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//genres[strlen(genres)-1] = '\0';
			
			//printf("   %s | %s | %s | %s\n", primaryTitle, genres, runtimeMinutes, startYear);
			//printf("%s", primaryTitle);
			insert(primaryTitle, genres, runtimeMinutes, startYear, &root, (Compare)cmpStr);
		}
		
		
		//NCURSES start
		
		initscr();
		cbreak();
		noecho();
		
		int yBeg, xBeg, yMax, xMax;
		getbegyx(stdscr, yBeg, xBeg);
		getmaxyx(stdscr, yMax, xMax);
		
		if(!has_colors())
		{
			printw("Terminal does not support color");
			getch();
			return -1;
		}
		
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		
		attron(COLOR_PAIR(1));
		mvprintw(yBeg + 2, (xMax/2)-17, "Internet Movie Database - Main Menu\n\n");
		attroff(COLOR_PAIR(1));
		
		WINDOW * menuwin = newwin(10, 45, yMax - 20, 30);
		//reuse this line for the search menu: WINDOW * inputWin = newwin(10, xMax - 20, yMax - 25, 10);
		wborder(menuwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
		refresh();
		wrefresh(menuwin);
		
		keypad(menuwin, true);
		
		char * choices[] = {"Add - Add a movie to your catalog", "Update - Update a record in your catalog", "Delete - Delete a movie from your catalog", "Exit"};
		int choice;
		int highlight = 0;
		
		while(1)
		{
			for(int i = 0; i < 4; i++)
			{
				if(i==highlight)
					wattron(menuwin, A_REVERSE);
				mvwprintw(menuwin, i+1, 1, (char *)choices[i]);
				wattroff(menuwin, A_REVERSE);
			}
			choice = wgetch(menuwin);
			
			switch(choice)
			{
				case KEY_UP:
					highlight--;
					if(highlight == -1)
						highlight = 0;
					break;
				case KEY_DOWN:
					highlight++;
					if(highlight == 4)
						highlight = 3;
					break;
				default:
					break;
			}
			
			if(choice == 10)
				break;
		}
		
		if(choices[highlight] == "Exit")
		{
			endwin();
			exit(1);
		}
		else if(choices[highlight] == "Add - Add a movie to your catalog")
		{
			unpost_menu(menuwin);
			
			free_menu(menuwin);
			wborder(menuwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
			wrefresh(menuwin);
			delwin(menuwin);
			
			WINDOW * searchwin = newwin(10, xMax - 20, yMax - 25, 10);
			wborder(searchwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
			refresh();
			wrefresh(searchwin);
			
			keypad(searchwin, true);
		
			char * searchChoices[] = {"Type a movie name to search...\t"};
			char * searchChoice = malloc(100);			
			
			while(1)
			{
				for(int i = 0; i < 1; i++)
				{
					/*if(i==highlight)
						wattron(menuwin, A_REVERSE);*/
					mvwprintw(searchwin, i+1, 1, (char *)searchChoices[i]);
					//wattroff(menuwin, A_REVERSE);
				}
				echo();
				wgetstr(searchwin, searchChoice);
				noecho();
				
				/* struct movieNode* searchRoot = NULL;
				search2(searchChoice, root, (Compare)cmpStr, &searchRoot);
				in_order(searchRoot); */
			}
			
			
		}	
		
		getch();
		endwin();
		
		/* struct movieNode* searchRoot = NULL;
		search2("Life of", root, (Compare)cmpStr, &searchRoot);
		in_order(searchRoot); */
		
		
		//freeing up memory
		free(tConst);
		free(titleType);
		free(primaryTitle);
		free(originalTitle);
		free(isAdult);
		free(startYear);
		free(endYear);
		free(runtimeMinutes);
		free(genres);
		free(file_content);
    }
    else{
        printf("open failure\n");
    }
	
    fclose(file);

    return 0;
}