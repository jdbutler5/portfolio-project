#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

//global indexing integer
int index = 0;

//struct for movie node
struct movieNode
{
	char * title;
	char * genre;
	char * runningTime;
	char * yearReleased;
	struct movieNode *left;
	struct movieNode *right;
};

//struct for user catalog movie node
struct userMovieNode
{
	char * title;
	char * genre;
	char * runningTime;
	char * yearReleased;
	char * type;
	char * date;
	struct movieNode *left;
	struct movieNode *right;
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
	}
}

//Function for inserting into the user catalog. User catalog is stored in temporary binary search tree.
void userInsert(char* key, char * genres, char * runningTimeMinutes, char * startYear, char * mediaType, char * dateEntered, struct userMovieNode** leaf, Compare cmp)
{
	int res;
	if (*leaf == NULL)
	{
		*leaf = (struct userMovieNode*)malloc(sizeof(struct userMovieNode));
		(*leaf)->title = malloc(strlen(key) + 1);
		strcpy((*leaf)->title, key);
		
		(*leaf)->genre = malloc(strlen(genres) + 1);
		strcpy((*leaf)->genre, genres);
		
		(*leaf)->runningTime = malloc(strlen(runningTimeMinutes) + 1);
		strcpy((*leaf)->runningTime, runningTimeMinutes);
		
		(*leaf)->yearReleased = malloc(strlen(startYear) + 1);
		strcpy((*leaf)->yearReleased, startYear);
		
		(*leaf)->type = malloc(strlen(mediaType) + 1);
		strcpy((*leaf)->type, mediaType);
		
		(*leaf)->date = malloc(strlen(dateEntered) + 1);
		strcpy((*leaf)->date, dateEntered);
		
		(*leaf)->left = (*leaf)->right = NULL;
	}
	else
	{
		res = cmp(key, (*leaf)->title);
		if(res < 0)
			userInsert(key, genres, runningTimeMinutes, startYear, mediaType, dateEntered, &(*leaf)->left, cmp);
		else if(res > 0)
			userInsert(key, genres, runningTimeMinutes, startYear, mediaType, dateEntered, &(*leaf)->right, cmp);
	}
}

//compares value of new node against previous
int cmpStr(const char *a, const char *b)
{
	return (strcmp(a,b));
}

//Prints out the entire movie BST in order.
//TODO: delete, this was used for testing only
void in_order(struct movieNode *root)
{
	if(root != NULL)
	{
		in_order(root->left);
		
		printf("%s\n", root->title);
		
		in_order(root->right);
	}
}

//Prints out the user's catalog in order.
//TODO: delete, this was used for testing only
void user_in_order(struct userMovieNode *root)
{
	if(root != NULL)
	{
		user_in_order(root->left);
		
		printf("   %s | %s | %s | %s \n", root->title, root->genre, root->runningTime, root->yearReleased);
		
		user_in_order(root->right);
	}
}

//Returns the size of a movie node tree.
int size(struct movieNode* node)
{
	int c = 1;
	if(node == NULL)
		return 0;
	else
	{
		c += size(node->left);
		c += size(node->right);
		return c;
	}
}

//Returns the size of a user catalog tree.
int userSize(struct userMovieNode* node)
{
	int c = 1;
	if(node == NULL)
		return 0;
	else
	{
		c += userSize(node->left);
		c += userSize(node->right);
		return c;
	}
}

//Sends the current BST to an array for printing.
void in_order_to_array(struct movieNode *root, struct movieNode *array[])
{
	if(root == NULL)
	{	
		return;
	}
	
	in_order_to_array(root->left, array);
	
	array[index]->title = malloc(strlen(root->title) + 1);
	array[index]->title = root->title;
	array[index]->genre = malloc(strlen(root->genre) + 1);
	array[index]->genre = root->genre;
	array[index]->runningTime = malloc(strlen(root->runningTime) + 1);
	array[index]->runningTime = root->runningTime;
	array[index]->yearReleased = malloc(strlen(root->yearReleased) + 1);
	array[index]->yearReleased = root->yearReleased; 
	index++;
	
	in_order_to_array(root->right, array);
}

//Sends the user's current catalog to an array in order for printing and writing to file.
void user_in_order_to_array(struct userMovieNode *root, struct userMovieNode *array[])
{
	if(root == NULL)
	{	
		return;
	}
	
	user_in_order_to_array(root->left, array);
	
	array[index]->title = malloc(strlen(root->title) + 1);
	array[index]->title = root->title;
	array[index]->genre = malloc(strlen(root->genre) + 1);
	array[index]->genre = root->genre;
	array[index]->runningTime = malloc(strlen(root->runningTime) + 1);
	array[index]->runningTime = root->runningTime;
	array[index]->yearReleased = malloc(strlen(root->yearReleased) + 1);
	array[index]->yearReleased = root->yearReleased; 
	array[index]->type = malloc(strlen(root->type) + 1);
	array[index]->type = root->type; 
	array[index]->date = malloc(strlen(root->date) + 1);
	array[index]->date = root->date; 
	index++;
	
	user_in_order_to_array(root->right, array);
}

//Zero indexes the global search index.
void zeroIndex()
{
	index = 0;
}

//Search for a specific node.
//TODO: delete, was used in testing
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
//TODO: rename, search2 is not a good algorithm name
void search2(char* key, struct movieNode* leaf, Compare cmp, struct movieNode ** searchRoot)
{
	int res;
	char * p;
	char * k;
	
	if(leaf != NULL)
	{
		res = cmp(key, leaf->title);
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
			insert(leaf->title, leaf->genre, leaf->runningTime, leaf->yearReleased, &(*searchRoot), (Compare)cmpStr);
		}
	}
	
	return (*searchRoot);
}

//Deletes a specified BST.
//IDK if I should even use this or not?
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

//Traverse to the farthest left node to find the minimum value.
struct userMovieNode * minValueNode(struct userMovieNode* node)
{
	struct userMovieNode* current = node;
	
	while(current->left != NULL)
	{
		current = current->left;
	}
	return current;
}

//Deletes a user movie node with the specified title.
struct userMovieNode* delete_userNode(struct userMovieNode* root, char* key, Compare cmp)
{
	if(root == NULL)
		return root;
	
	int res;
	res = cmp(key, root->title);
	if(res < 0 && root->left != NULL)
		root->left = delete_userNode(root->left, key, cmp);
	else if(res > 0 && root->right != NULL)
		root->right = delete_userNode(root->right, key, cmp);
	else
	{
		if(root->left == NULL)
		{
			struct userMovieNode *temp = root->right;
			return temp;
		}
		else if(root->right == NULL)
		{
			struct userMovieNode *temp = root->left;
			return temp;
		}
		
			struct userMovieNode *temp = minValueNode(root->right);
			root->title = malloc(temp->title + 1);
			root->title = temp->title;
			root->genre = malloc(temp->genre + 1);
			root->genre = temp->genre;
			root->runningTime = malloc(temp->runningTime + 1);
			root->runningTime = temp->runningTime;
			root->yearReleased = malloc(temp->yearReleased + 1);
			root->yearReleased = temp->yearReleased;
			root->type = malloc(temp->type + 1);
			root->type = temp->type;
			root->date = malloc(temp->date + 1);
			root->date = temp->date;
			
			root->right = delete_userNode(root->right, temp->title, (Compare)cmpStr);
	}
	return root;
}

int main()
{
    FILE * file;
	FILE * file2;
    long fsize;
	long fsize2;
	
	//open movie records file
    file = fopen("movie_records","r");
	char * userFilename = malloc(30 * sizeof(char));
    if(file != NULL)
	{

        //get file size
        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);
		
		//variable definitions
		char ** tConst = malloc(fsize*10*sizeof(char));
		char ** titleType = malloc(fsize*10*sizeof(char));
		char ** primaryTitle = malloc(fsize*200*sizeof(char));
		char ** originalTitle = malloc(fsize*200*sizeof(char));
		char ** isAdult = malloc(fsize*sizeof(char));
		char ** startYear = malloc(fsize*5*sizeof(char));
		char ** endYear = malloc(fsize*5*sizeof(char));
		char ** runtimeMinutes = malloc(fsize*100*sizeof(char));
		char ** genres = malloc(fsize*100*sizeof(char));

        // insert all records in movie_records into 
        char * file_content = (char*)malloc(fsize);
		
		struct movieNode *root = NULL;
		
		while(fgets(file_content,fsize,file))
		{
			sscanf(file_content, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			insert(primaryTitle, genres, runtimeMinutes, startYear, &root, (Compare)cmpStr);
		}
		
		//ncurses initialization
		initscr();
		cbreak();
		noecho();
		
		//get user's terminal size
		int yBeg, xBeg, yMax, xMax;
		getbegyx(stdscr, yBeg, xBeg);
		getmaxyx(stdscr, yMax, xMax);
		
		bool fileNameExitCond = false;
		
		//get filename from user to begin
		while(!fileNameExitCond)
		{
			mvprintw(yMax/2, (xMax/4), "Type a username to get started:");
			echo();
			wgetnstr(stdscr, userFilename, 30);
			fileNameExitCond = true;
		}
		
		noecho();
		
		struct userMovieNode ** userRoot = NULL;
		bool keepNULL = false;
		
		//If the username is bad input, inform the user and forcequit the program.
		if(userFilename == NULL || strcmp(userFilename, "\n") == 0 || strcmp(userFilename, "") == 0 )
		{
			mvprintw(yMax/2 + 1, (xMax/4), "Error: You must type something in for your username.");
			getch();
			endwin();
			exit(1);
		}
		else
		{
			char * userlog = malloc(10 + sizeof(userFilename) * sizeof(char));
			strcat(userlog, userFilename);
			strcat(userlog, ".log");
			file2 = fopen(userlog, "r");
			//If log file already exists, read from it and add each record to catalog BST
			//If log file of that username does not exist, inform the user that a new one is being created.
			if(file2 != NULL)
			{
				
				fseek(file2,0,SEEK_END);
				fsize2 = ftell(file2);
				rewind(file2);
				
				char * file_content2 = (char*)malloc(fsize2);
				char * tTitle = malloc(200 * sizeof(char));
				char * tRunningTime = malloc(10 * sizeof(char));
				char * tYearReleased = malloc(5 * sizeof(char));
				char * tGenre = malloc(100 * sizeof(char));
				char * tType = malloc(10 * sizeof(char));
				char * tDate = malloc(64 * sizeof(char));
				
				userRoot = NULL;
				int i = 0;
				while(fgets(file_content2,fsize2,file2))
				{
					sscanf(file_content2, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]", tTitle, tRunningTime, tYearReleased, tGenre, tType, tDate);
					if(file_content2[0] != '\0' && file_content2[0] != '\t' && file_content2[0] != '\n' && file_content2[0] != '\r')
						userInsert(tTitle, tGenre, tRunningTime, tYearReleased, tType, tDate, &userRoot, (Compare)cmpStr);
				}
				mvprintw(yMax-1, 0, "Loading %s's catalog.", userFilename);
				getch();
			}
			else 
			{
				mvprintw(yMax-1, 0, "Catalog not found, opening new catalog.");
				getch();
			}
		}
		
		while(1)
		{
			
			if(!has_colors())
			{
				printw("Terminal does not support color");
				getch();
				return -1;
			}
			
			//Printing main menu 
			start_color();
			init_pair(1, COLOR_YELLOW, COLOR_BLACK);
			clear();
			attron(COLOR_PAIR(1));
			mvprintw(yBeg + 2, (xMax/4), "Internet Movie Database - Main Menu\n\n");
			attroff(COLOR_PAIR(1));
			
			keypad(stdscr, true);
			
			WINDOW * menuwin = newwin(10, 45, yMax/4, (xMax/4));
			wborder(menuwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
			mvprintw(yBeg+4, (xMax/4), "Editing user catalog for %s", userFilename);
			refresh();
			wrefresh(menuwin);
			
			keypad(menuwin, true);
			
			//Printing menu options
			char * choices[] = {"Add - Add a movie to your catalog", "Update Catalog - Update current catalog", "Delete - Delete a movie from your catalog", "Exit"};
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
				//When the user exits, send the user catalog BST to a new array and save that array to the user file
				FILE * file3;
				char userlog[30] = "";
				strcpy(userlog, userFilename);
				strcat(userlog, ".log");
				
				bool file2Null = false;
				
				if(file2 == NULL)
				{
					file2Null = true;
					file3 = fopen(userlog, "w");
				}
				else
				{
					file3 = freopen(userlog, "w", file2);
				}
				
				int userArraySize = userSize(userRoot);

				struct userMovieNode * userArray[(userArraySize)];
				for(int i = 0; i < userArraySize; i++)
				{
					userArray[i] = (struct userMovieNode*)malloc(sizeof(struct userMovieNode));
				}
				
				mvprintw(yMax-1, 0, "%d", userArraySize);
				user_in_order_to_array(userRoot, userArray);  
				
				char * stringToAdd = (char*)malloc(500 * sizeof(char));
				
				for(int i = 0; i < userArraySize; i++)
				{
					if(userArray[i]->date[strlen(userArray[i]->date) - 1] == '\n')
						userArray[i]->date[strlen(userArray[i]->date) - 1] = '\0';
					if(stringToAdd[0] != '\0' && stringToAdd[0] != '\t' && stringToAdd[0] != '\n' && stringToAdd[0] != '\r')
					{
						sprintf(stringToAdd, "%s\t%s\t%s\t%s\t%s\t%s\n", userArray[i]->title, userArray[i]->runningTime, userArray[i]->yearReleased, userArray[i]->genre, userArray[i]->type, userArray[i]->date);
						mvprintw(yBeg+i, 0, "%s", stringToAdd);
						if(file2Null)
							fputs(stringToAdd, file3);
						else
							fputs(stringToAdd, file2);
					}
				} 
				getch();
				if(file2 != NULL)
					fclose(file2);
				if(file3 != NULL)
					fclose(file3);
				endwin();
				exit(1);
			}
			else if(choices[highlight] == "Add - Add a movie to your catalog")
			{
				wborder(menuwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wclear(menuwin);
				wrefresh(menuwin);
				delwin(menuwin);
				
				attron(COLOR_PAIR(1));
				mvprintw(yBeg + 2, (xMax/4), "Internet Movie Database - Add to Catalog\n\n");
				attroff(COLOR_PAIR(1));
				
				refresh();
				
				WINDOW * searchwin = newwin(yMax/1.5, xMax, yMax/4, xBeg+(xMax/16));
				wborder(searchwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
				
				wrefresh(searchwin);
				
				keypad(searchwin, true);
			
				char * searchMenu[] = {"Type a movie name to search... "};
				char * searchChoice = malloc(200 * sizeof(char));			
				
				bool exitCond = false;
				
				//User enters search string
				while(!exitCond)
				{
					mvwprintw(searchwin, 1, 1, (char *)searchMenu[0]);
					echo();
					wgetnstr(searchwin, searchChoice, 200);
					exitCond = true;
				}
				
				/*  
					Incorrect (i.e. NULL or hitting enter without typing anything) 
					search strings will be rejected,
					and the user will be notified and sent back to the main menu.
				*/
				if(searchChoice == NULL || strcmp(searchChoice, "\n") == 0 || strcmp(searchChoice, "") == 0 )
				{
					mvprintw(yMax/2 + 1, (xMax/4), "Error: You must type in a valid movie title.");
					getch();
				}
				else 
				{
					noecho();
					struct movieNode** searchRoot = NULL;
					search2(searchChoice, root, (Compare)cmpStr, &searchRoot);
					
					int searchBSTsize = size(searchRoot);
					
					//If the user's search returned no records, they will be notified and returned to the main menu.
					if(searchBSTsize == 0)	
					{
						mvprintw(yMax/2 + 1, (xMax/4), "Search returned no results. Please enter another search.", searchBSTsize);
						getch();
					
					}
					else
					{
						//Create an array and fill it with an inorder traversal of the search results.
						struct movieNode * array[(searchBSTsize)];
						
						for(int i = 0; i < size(searchRoot); i++)
						{
							array[i] = (struct movieNode*)malloc(sizeof(struct movieNode));
						}
						
						in_order_to_array(searchRoot, array);
						char * searchChoices[searchBSTsize]; 
						
						for(int i = 0; i < searchBSTsize; i++)
						{
							searchChoices[i] = (char*)malloc(400 * sizeof(char));
						}
						
						int searchHighlight = 0;
						int numRecordsToPrint = 10;
						if(yMax < 24)
						{
							numRecordsToPrint = 5;
						}
						
						bool tooManyRecords = false;
						if(searchBSTsize > numRecordsToPrint)
						{
							tooManyRecords = true;
						}
						
						wrefresh(searchwin);
						
						int choice2;
						bool exitSearch = false;
						
						while(1)
						{
							wborder(searchwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
							wrefresh(searchwin);
							int i = 0; 
							int arrayCounter = 0;
							if(searchBSTsize < numRecordsToPrint)
							{
								arrayCounter = searchBSTsize;
							}
							else 
							{
								arrayCounter = numRecordsToPrint;
							}
							
							for(i = 0; i < arrayCounter; i++)
							{
								if(i==searchHighlight)
									wattron(searchwin, A_REVERSE);
								sprintf(searchChoices[i], "%s | %s mins | %s | %s", array[i]->title, array[i]->runningTime, array[i]->yearReleased, array[i]->genre);
								mvwprintw(searchwin, (i+1)+1, 1, "%s", searchChoices[i]);
								
								wborder(searchwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
								wattroff(searchwin, A_REVERSE);
								wrefresh(searchwin);
							}
							
							choice2 = wgetch(searchwin);
						
							switch(choice2)
							{
								case KEY_UP:
									searchHighlight--;
									if(searchHighlight == -1)
									{
										searchHighlight = 0;						
									}
									if(tooManyRecords)
									{
										if(searchHighlight > arrayCounter)
										{
											searchHighlight =  arrayCounter - 1;
										}
										else if(searchHighlight == arrayCounter)
										{
											wattron(searchwin, A_REVERSE);
											mvwprintw(searchwin, arrayCounter+2, 1, "Printed the first %d out of %d records. Please make your search more specific to access more movies.", arrayCounter, searchBSTsize);
											wattroff(searchwin, A_REVERSE);
										}
										else
										{
											mvwprintw(searchwin, arrayCounter+2, 1, "Printed the first %d out of %d records. Please make your search more specific to access more movies.", arrayCounter, searchBSTsize);
										} 
									}
									else 
									{
										if(searchHighlight > arrayCounter)
										{
											searchHighlight = arrayCounter - 1;
										}
									}
									break;
								case KEY_DOWN:
									searchHighlight++;
									if(tooManyRecords)
									{
										if(searchHighlight >= arrayCounter)
										{
											searchHighlight =  arrayCounter - 1;
										}
										else if(searchHighlight == arrayCounter)
										{
											wattron(searchwin, A_REVERSE);
											mvwprintw(searchwin, arrayCounter+2, 1, "Printed the first %d out of %d records. Please make your search more specific to access more movies.", arrayCounter, searchBSTsize);
											wattroff(searchwin, A_REVERSE);
										}
										else
										{
											mvwprintw(searchwin, arrayCounter+2, 1, "Printed the first %d out of %d records. Please make your search more specific to access more movies.", arrayCounter, searchBSTsize);
										} 
									}
									else 
									{
										if(searchHighlight >= arrayCounter)
										{
											searchHighlight = arrayCounter - 1;
										}
									}
									
									break;
								default:
									break;
							}
							
							if(choice2 == 10)
								break;
						}
						
						char * typeChoices[] = {"DVD", "BluRay", "Digital"};
						char * mediaType = (char *)malloc(10 * sizeof(char));
						time_t t = time(NULL);
						struct tm *tm = localtime(&t);
						char date[64];
						strftime(date, sizeof(date), "%c", tm);
						
						wclear(searchwin);
						wborder(searchwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
						wrefresh(searchwin);
						
						int typeHighlight = 0;
						int typeChoice;
						
						while(1)
						{
							mvwprintw(searchwin, 1, 1, "%s", array[searchHighlight]->title);
							
							for(int i = 0; i < 3; i++)
							{
								if(i==typeHighlight)
									wattron(searchwin, A_REVERSE);
								mvwprintw(searchwin, i+2, 1, (char *)typeChoices[i]);
								wattroff(searchwin, A_REVERSE);
							}
							typeChoice = wgetch(searchwin);
							
							switch(typeChoice)
							{
								case KEY_UP:
									typeHighlight--;
									if(typeHighlight == -1)
										typeHighlight = 0;
									break;
								case KEY_DOWN:
									typeHighlight++;
									if(typeHighlight == 3)
										typeHighlight = 2;
									break;
								default:
									break;
							}
							
							if(typeChoice == 10)
								break;
							wrefresh(searchwin);
						}
						
						if(typeChoices[typeHighlight] == "DVD")
						{
							mediaType = "DVD";
						}
						else if(typeChoices[typeHighlight] == "BluRay")
						{
							mediaType = "BluRay";
						}
						else if(typeChoices[typeHighlight] == "Digital")
						{
							mediaType = "Digital";
						}
						
						char * recordToAdd = (char*)malloc(500 * sizeof(char));
						if(array[searchHighlight]->genre[strlen(array[searchHighlight]->genre) - 1] == '\n')
							array[searchHighlight]->genre[strlen(array[searchHighlight]->genre) - 1] = '\0';
						
						userInsert(array[searchHighlight]->title, array[searchHighlight]->genre, array[searchHighlight]->runningTime, array[searchHighlight]->yearReleased, mediaType, date, &userRoot, (Compare)cmpStr);
						mvprintw(yMax-1, xBeg, "Added %s to %s's catalog", array[searchHighlight]->title, userFilename);
						
						
						getch();
						wborder(searchwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
						wclear(searchwin);
						wrefresh(searchwin);
						delwin(searchwin);
						erase();
						endwin();
						
					}
					
					
				}
				
				zeroIndex();
				wrefresh(searchwin);
			}	
			else if(choices[highlight] == "Update Catalog - Update current catalog")
			{
				wborder(menuwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wclear(menuwin);
				wrefresh(menuwin);
				delwin(menuwin);
				erase();
				
				attron(COLOR_PAIR(1));
				mvprintw(yBeg + 2, (xMax/4), "Internet Movie Database - Update Catalog\n\n");
				attroff(COLOR_PAIR(1));
				
				refresh();
				
				WINDOW * printwin = newwin(yMax/1.5, xMax, yMax/4, xBeg+(xMax/16));
				wborder(printwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
				
				wrefresh(printwin);
				
				keypad(printwin, true);
				
				int userBSTsize = userSize(userRoot);
				
				if(userBSTsize == 0)
				{
					mvprintw(yMax/2, (xMax/4), "Error: User catalog currently has no records to update.");
					getch();
					endwin();
					exit(1);
				}
				
				struct userMovieNode * array[userBSTsize];
				
				for(int i = 0; i < userBSTsize; i++)
				{
					array[i] = (struct userMovieNode*)malloc(sizeof(struct userMovieNode));
				} 
				
				user_in_order_to_array(userRoot, array);
				int printChoice;
				int printHighlight = 0;
				
				while(1)
				{
					for(int i = 0; i < userBSTsize; i++)
					{
						if(i==printHighlight)
							wattron(printwin, A_REVERSE);
						mvwprintw(printwin, i+1, 1, "%s | %s mins | %s | %s | %s | %s", array[i]->title, array[i]->runningTime, array[i]->genre, array[i]->yearReleased, array[i]->type, array[i]->date);
						wattroff(printwin, A_REVERSE);
					}
					printChoice = wgetch(printwin);
					
					switch(printChoice)
					{
						case KEY_UP:
							printHighlight--;
							if(printHighlight < 0)
								printHighlight = 0;
							break;
						case KEY_DOWN:
							printHighlight++;
							if(printHighlight == userBSTsize)
								printHighlight = userBSTsize - 1;
							break;
						default:
							break;
					}
					
					if(printChoice == 10)
						break;
				} 
				
				char * typeChoices[] = {"DVD", "BluRay", "Digital"};
				char * updateChoices[] = {"Update Type", "Update Time"};
				char * mediaType = (char *)malloc(10 * sizeof(char));
				time_t t = time(NULL);
				struct tm *tm = localtime(&t);
				char date[64];
				strftime(date, sizeof(date), "%c", tm);
				
				wclear(printwin);
				wborder(printwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
				wrefresh(printwin);
				
				int updateHighlight = 0;
				int updateChoice;
				
				while(1)
				{
					mvwprintw(printwin, 1, 1, "%s | %s | %s", array[printHighlight]->title, array[printHighlight]->type, array[printHighlight]->date);
					
					for(int i = 0; i < 2; i++)
					{
						if(i==updateHighlight)
							wattron(printwin, A_REVERSE);
						mvwprintw(printwin, i+2, 1, (char *)updateChoices[i]);
						wattroff(printwin, A_REVERSE);
					}
					updateChoice = wgetch(printwin);
					
					switch(updateChoice)
					{
						case KEY_UP:
							updateHighlight--;
							if(updateHighlight == -1)
								updateHighlight = 0;
							break;
						case KEY_DOWN:
							updateHighlight++;
							if(updateHighlight == 2)
								updateHighlight = 1;
							break;
						default:
							break;
					}
					
					if(updateChoice == 10)
						break;
					wrefresh(printwin);
				}
				
				int updateChoiceHighlight = 0;
				if(updateChoices[updateHighlight] == "Update Type")
				{
					wclear(printwin);
					wrefresh(printwin);
					while(1)
					{
						wborder(printwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
						wrefresh(printwin);
						mvwprintw(printwin, 1, 1, "%s | %s", array[printHighlight]->title, array[printHighlight]->type);
						
						for(int i = 0; i < 3; i++)
						{
							if(i==updateChoiceHighlight)
								wattron(printwin, A_REVERSE);
							mvwprintw(printwin, i+2, 1, (char *)typeChoices[i]);
							wattroff(printwin, A_REVERSE);
						}
						updateChoice = wgetch(printwin);
						
						switch(updateChoice)
						{
							case KEY_UP:
								updateChoiceHighlight--;
								if(updateChoiceHighlight == -1)
									updateChoiceHighlight = 0;
								break;
							case KEY_DOWN:
								updateChoiceHighlight++;
								if(updateChoiceHighlight == 3)
									updateChoiceHighlight = 2;
								break;
							default:
								break;
						}
						
						if(updateChoice == 10)
							break;
						wrefresh(printwin);
					}
					
					if(typeChoices[updateChoiceHighlight] == "DVD")
					{
						mediaType = "DVD";
					}
					else if(typeChoices[updateChoiceHighlight] == "BluRay")
					{
						mediaType = "BluRay";
					}
					else if(typeChoices[updateChoiceHighlight] == "Digital")
					{
						mediaType = "Digital";
					}
					
					struct userMovieNode * updatedNode = malloc((struct userMovieNode*)sizeof(struct userMovieNode));
					updatedNode->title = malloc(array[printHighlight]->title + 1);
					updatedNode->title = array[printHighlight]->title;
					updatedNode->genre = malloc(array[printHighlight]->genre + 1);
					updatedNode->genre = array[printHighlight]->genre;
					updatedNode->runningTime = malloc(array[printHighlight]->runningTime + 1);
					updatedNode->runningTime = array[printHighlight]->runningTime;
					updatedNode->yearReleased = malloc(array[printHighlight]->yearReleased + 1);
					updatedNode->yearReleased = array[printHighlight]->yearReleased;
					updatedNode->type = malloc(mediaType + 1);
					updatedNode->type = mediaType;
					updatedNode->date = malloc(array[printHighlight]->date + 1);
					updatedNode->date = array[printHighlight]->date; 
					
					userRoot = delete_userNode(userRoot, updatedNode->title, (Compare)cmpStr);
					
					userInsert(updatedNode->title, updatedNode->genre, updatedNode->runningTime, updatedNode->yearReleased, updatedNode->type, updatedNode->date, &userRoot, (Compare)cmpStr);
					mvprintw(yMax-1, xBeg, "Added %s (%s) to %s's catalog", updatedNode->title, updatedNode->type, userFilename);
					getch();
				}
				else if(updateChoices[updateHighlight] == "Update Time")
				{
					wclear(printwin);
					wrefresh(printwin);
					
					bool dateExitCond = false;
					char * dateStr = (char*)malloc(25 * sizeof(char));
		
					wborder(printwin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
					wrefresh(printwin);
		
					while(!dateExitCond)
					{	
						mvwprintw(printwin, 0, 1, "Type in a date of format: DoW Mth DD Hr:Mn:Sc YYYY");
						mvwprintw(printwin, 1, 1, "%s | ", array[printHighlight]->title);
						echo();
						wgetnstr(printwin, dateStr, 25);
						dateExitCond = true;
					}
					
					if(dateStr == NULL || strcmp(dateStr, "\n") == 0 || strcmp(dateStr, "") == 0 )
					{
						mvprintw(yMax/2 + 1, (xMax/4), "Error: You must type in a valid date.");
						getch();
						endwin();
						break;
					}
					
					struct userMovieNode * updatedNode = malloc((struct userMovieNode*)sizeof(struct userMovieNode));
					updatedNode->title = malloc(array[printHighlight]->title + 1);
					updatedNode->title = array[printHighlight]->title;
					updatedNode->genre = malloc(array[printHighlight]->genre + 1);
					updatedNode->genre = array[printHighlight]->genre;
					updatedNode->runningTime = malloc(array[printHighlight]->runningTime + 1);
					updatedNode->runningTime = array[printHighlight]->runningTime;
					updatedNode->yearReleased = malloc(array[printHighlight]->yearReleased + 1);
					updatedNode->yearReleased = array[printHighlight]->yearReleased;
					updatedNode->type = malloc(array[printHighlight]->type + 1);
					updatedNode->type = array[printHighlight]->type;
					updatedNode->date = malloc(dateStr + 1);
					updatedNode->date = dateStr; 
					
					userRoot = delete_userNode(userRoot, updatedNode->title, (Compare)cmpStr);
					
					userInsert(updatedNode->title, updatedNode->genre, updatedNode->runningTime, updatedNode->yearReleased, updatedNode->type, updatedNode->date, &userRoot, (Compare)cmpStr);
					mvprintw(yMax-1, xBeg, "Added %s (%s) to %s's catalog", updatedNode->title, updatedNode->date, userFilename);
					getch();
				}
				
				wborder(printwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wclear(printwin);
				wrefresh(printwin);
				delwin(printwin);
				erase();
				endwin();
				
				zeroIndex();
				wrefresh(printwin);
			}	
			else if(choices[highlight] == "Delete - Delete a movie from your catalog")
			{
				wborder(menuwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wclear(menuwin);
				wrefresh(menuwin);
				delwin(menuwin);
				erase();
				
				attron(COLOR_PAIR(1));
				mvprintw(yBeg + 2, (xMax/4), "Internet Movie Database - Delete from Catalog\n\n");
				attroff(COLOR_PAIR(1));
				
				refresh();
				
				WINDOW * deletewin = newwin(yMax/1.5, xMax, yMax/4, xBeg+(xMax/16));
				wborder(deletewin, 0, 0, ' ', ' ', ' ', ' ', ' ', ' ');
				
				wrefresh(deletewin);
				
				keypad(deletewin, true);
				
				int userBSTsize = userSize(userRoot);
				
				if(userBSTsize == 0)
				{
					mvprintw(yMax/2, (xMax/4), "Error: User catalog currently has no records to delete.");
					getch();
					endwin();
					exit(1);
				}
				
				struct userMovieNode * array[userBSTsize];
				
				for(int i = 0; i < userBSTsize; i++)
				{
					array[i] = (struct userMovieNode*)malloc(sizeof(struct userMovieNode));
				} 
				
				user_in_order_to_array(userRoot, array);
				int deleteChoice;
				int deleteHighlight = 0;
				
				while(1)
				{
					for(int i = 0; i < userBSTsize; i++)
					{
						if(i==deleteHighlight)
							wattron(deletewin, A_REVERSE);
						mvwprintw(deletewin, i+1, 1, "%s | %s | %s", array[i]->title, array[i]->type, array[i]->date);
						wattroff(deletewin, A_REVERSE);
					}
					deleteChoice = wgetch(deletewin);
					
					switch(deleteChoice)
					{
						case KEY_UP:
							deleteHighlight--;
							if(deleteHighlight < 0)
								deleteHighlight = 0;
							break;
						case KEY_DOWN:
							deleteHighlight++;
							if(deleteHighlight == userBSTsize)
								deleteHighlight = userBSTsize - 1;
							break;
						default:
							break;
					}
					
					if(deleteChoice == 10)
						break;
				} 
				
				userRoot = delete_userNode(userRoot, array[deleteHighlight]->title, (Compare)cmpStr);
				mvprintw(yMax-1, xBeg, "Deleted %s from %s's catalog", array[deleteHighlight]->title, userFilename);
				getch();
				
				wborder(deletewin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wclear(deletewin);
				wrefresh(deletewin);
				delwin(deletewin);
				erase();
				endwin();
				
				zeroIndex();
				wrefresh(deletewin);
			}
		}
		
		
		endwin();
		
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
	fclose(file2);

    return 0;
}