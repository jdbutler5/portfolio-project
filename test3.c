#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct for movie node
struct movieNode{
	int key;
	char * title;
	struct movieNode *left;
	struct movieNode *right;
	//int height;
};

//node constructor w/ input
/* struct movieNode *newNode(char *title){
	
} */

typedef int (*Compare) (const char *, const char *);

void insert(char* key, struct movieNode** leaf, Compare cmp){
	int res;
	if (*leaf == NULL){
		*leaf = (struct movieNode*)malloc(sizeof(struct movieNode));
		(*leaf)->title = malloc(strlen (key) + 1);
		strcpy((*leaf)->title, key);
		(*leaf)->left = (*leaf)->right = NULL;
	}else{
		res = cmp(key, (*leaf)->title);
		if(res < 0)
			insert(key, &(*leaf)->left, cmp);
		else if(res > 0)
			insert(key, &(*leaf)->right, cmp);
		// else
			// printf("Key '%s' already in tree.", key);
	}
}

//compares value of new node against previous
int cmpStr(const char *a, const char *b){
	return (strcmp(a,b));
}

void in_order(struct movieNode *root){
	if(root != NULL){
		in_order(root->left);
		printf("   %s\n", root->title);
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

void search2(char*key, struct movieNode* leaf, Compare cmp){
	
}

void delete_tree(struct movieNode** leaf){
	if(*leaf != NULL){
		delete_tree(&(*leaf)->left);
		delete_tree(&(*leaf)->right);
		free((*leaf)->title);
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
			//printf("%s %s %s %s %s %s %s %s %s\n", tConst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres);
			insert(primaryTitle, &root, (Compare)cmpStr);
		}
		
		
		//in_order(root);
		search("Taiga", root, (Compare)cmpStr);
		// traversal(root);
		
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