# CS201 Portfolio Project by JD Butler
## About the project

This portfolio project is intended to simulate a movie database where the user can search for movies to add to their catalog 
and then update or delete those records later if the user so chooses. When adding, users can select the type of movie (DVD, BluRay, Digital)
to add to their catalog. This can later be updated, as can the date that the user added the movie to their catalog. See the section titled Running the Program for more details.

## Creating movie database
Follow these steps to create the movie database file.

Click or copy/paste the link in the file "link.txt" to your browser of choice. Select the "title.basics.tsv.gz" for download.

Once the download has completed, move the title.basics.tsv.gz file to the directory where the project is stored, and use the bash command 
>tar xvzf title.basics.tsv.gz

to unzip the file.

Now, run the code
>grep "movie" title.basics.tsv > movie_records 

which will save only all movie records to a file called "movie_records". This file "movie_records" is what the program reads from to create its database. It is important that
this file "movie_records" is in the same folder that the program's main.c and Makefile are, and that the file is called "movie_records". The program cannot run without this file.

## Running the Program
To run the program, compile using make in the bash terminal and run the executable "imdb_database".

The user will be prompted for a username. Entering in a username will open a .log file if it exists, 
and if the username specified does not already have a .log file, one will be created. 

Usernames are limited to 30 characters.

(The open file and its changes are not saved until the program is closed, to ensure that the program writes to the file correctly. The user's catalog
will, however, be stored in memory until the program exits, at which point the catalog is written to the file.)

The user is then given a main menu, navigable with the up and down arrow keys and the enter key, with 4 options to choose from: Add, View/Update, Delete, and Exit.

Selecting "Add" allows the user to input a search string, and hitting Enter after typing in a search string will bring up all search results from the database with that search string.
The user can scroll through the results with the up and down arrow keys. 

The program will let the user know how many results their search returned.
If the user inputs a string with no results or an invalid string (i.e. they just hit Enter without typing anything), they will be informed that their search returned no results
and sent back to the main menu. Search strings MUST be entered with proper capitalization and punctuation.

After typing in a search string and pressing Enter on a movie record, the user is prompted to decide what type of media to add to their catalog, from DVD, BluRay, or Digital.
After the user makes this selection, the program will add it to their catalog and notify the user that it was a success. 

If the user already has a movie in their catalog,
adding the same movie again and selecting a media type will only update the movie already in the catalog with the new type.

Selecting "View/Update" from the main menu allows the user to either view or update their current catalog. This is the catalog that will be written to the file when the program exits.
The user can scroll with the arrow keys through their catalog in a similar fashion to the search results from Add. 

After selecting a movie record with Enter, the user is prompted
with a choice between updating the type of media for the record or the date. Selecting "Update Type" allows the user to choose again between DVD, BluRay, and Digital for that 
movie record's media type.

Selecting "Update Date" allows the user to type in a date string of the format DD/MM/YYYY. If the user attempts to input a string outside of this format, they will be notified
that only strings of format DD/MM/YYYY are accepted and will be sent back to the main menu.

Selecting "Delete" from the main menu allows the user to access their catalog the same way as with the View/Update option. However, in this submenu, any record that is selected 
by the user will be deleted from their catalog. The program will notify the user of the deletion and then send them back to the main menu.

Selecting "Exit" from the main menu will write the user's catalog to the .log file denoted with their username and then exit the program.
The user will be informed that this write has occured.

## Built With
- Ubuntu for Windows
- C
- C99 Compiler

## Citations
I borrowed some basic binary search tree functions from GeeksForGeeks as a part of my data structure for the movie database. The functions 
in main.c that I borrowed were delete_userNode, minValueNode, search, insert, and size.

I used the search, insert, and size functions as part of the algorithm to build my movie database, which I implemented as a binary search tree. The search function specifically
allowed me to implement a log(n) search time through the binary search tree. 

I used delete_userNode and minValueNode for deleting user movie nodes, which was used in both the update and delete functions.

https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/ 

https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/

https://www.geeksforgeeks.org/write-a-c-program-to-calculate-size-of-a-tree/

## Info
JD Butler

11761514

CS201 Spring 2019

Dr. Anderson
