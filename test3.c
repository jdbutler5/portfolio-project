#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for movie node
struct movieNode{
	int key;
	char * title;
	char * genre;
	char * runningTime;
	char * yearReleased;
	struct movieNode *left;
	struct movieNode *right;
	//int height;
};

//node constructor w/ input
/* struct movieNode *newNode(char *title){
	
} */

typedef int (*Compare) (const char *, const char *);

char * choppy(char *s){
	/* int len = strlen(s);
	if(s[len-1] == "\n"){
		s[len-1] = 0;
	} */
	
	if(s[strlen(s)-1] == "\n")
		s[strlen(s)-1] = '\0';
	/* if(strcmp(s, "'\N'") == 0){
		strcpy(s, "N/A");
	} */
	return s;
}

void insert(char* key, char * genres, char * runningTimeMinutes, char * startYear, struct movieNode** leaf, Compare cmp){
	int res;
	if (*leaf == NULL){
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
	}else{
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
int cmpStr(const char *a, const char *b){
	return (strcmp(a,b));
}

void in_order(struct movieNode *root){
	if(root != NULL){
		in_order(root->left);
		
		//printf("   %s | %s | %s | %s \n", root->title, root->genre, root->runningTime, root->yearReleased);
		printf("%s\n", root->title);
		
		in_order(root->right);
	}
}

//traversal function to show search path to a movie
/* void traversal(struct movieNode *root){
	char src[10000];
	if(root != NULL){
		if (strcmp(root->title, "Hamlet, Prince of Denmark") == 0){
			printf("Hamlet, Prince of Denmark");
			exit(1);
		}
		else{
			strcat(" %s\n", root->title);
		}
	}
} */


void search(char* key, struct movieNode* leaf, Compare cmp){
	int res;
	if(leaf != NULL){
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

//search that will return multiple results
// struct movieNode *search2(char*key, struct movieNode* leaf, Compare cmp){
void search2(char* key, struct movieNode* leaf, Compare cmp, struct movieNode ** searchRoot){
	int res;
	//struct movieNode* searchRoot = NULL;
	char * p;
	char * k;
	
	if(leaf != NULL){
		res = cmp(key, leaf->title);
		//printf("%s", leaf->title);
		if(res < 0){
			//If the search key doesn't match exactly, see if any entries in the left leaf contain a part of the search string
			
			p = strstr(leaf->title, key);
			if(p != NULL){
				//If they do, add it to the searchRoot mini-BST
				
				//printf("\n'%s' found! (L)\n", leaf->title);
				
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
				//if not, continue searching down the left leaf path
			search2(key, leaf->left, cmp, (*searchRoot));
			search2(key, leaf->right, cmp, (*searchRoot));
		}
		else if(res > 0){
			k = strstr(leaf->title, key);
			if(k != NULL){
				
				// printf("\n'%s' found! (R)\n", leaf->title);
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
			search2(key, leaf->left, cmp, (*searchRoot));
			search2(key, leaf->right, cmp, (*searchRoot));
		}
		else
			printf("\n'%s' found!\n", key);
	}
	/* if(searchRoot != NULL){
		return searchRoot;
	} */
	//in_order(*searchRoot);
	return (*searchRoot);
}

void delete_tree(struct movieNode** leaf){
	if(*leaf != NULL){
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
	
    file = fopen("movie_records","r");
	// file = fopen("datatest2.txt","r");
    if(file != NULL){

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
		
		while(fgets(file_content,fsize,file)){
			//puts(file_content);
			sscanf(file_content, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//sscanf(file_content, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//printf("%s %s %s %s %s %s %s %s %s\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			
			//genres[strlen(genres)-1] = '\0';
			
			//printf("   %s | %s | %s | %s\n", primaryTitle, genres, runtimeMinutes, startYear);
			//printf("%s", primaryTitle);
			insert(primaryTitle, genres, runtimeMinutes, startYear, &root, (Compare)cmpStr);
		}
		
		
		//in_order(root);
		
		//search("Taiga", root, (Compare)cmpStr);
		
		struct movieNode* searchRoot = NULL;
		search2("Life of", root, (Compare)cmpStr, &searchRoot);
		//in_order(searchRoot);
		
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