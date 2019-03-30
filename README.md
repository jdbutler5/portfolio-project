# CS201 Portfolio Project by JD Butler
## About the project

This portfolio project is intended to simulate a movie database where the user can search for movies to add to their catalog 
and then update or delete those records later if the user so chooses. When adding, users can select the type of movie (DVD, BluRay, Digital)
to add to their catalog. This can later be updated, as can the date that the user added the movie to their catalog. See the section titled Running the Program for more details.



## Getting movie file

## Running the Program
To run the program, compile using make in the bash terminal and run the executable "imdb_database".

The user will be prompted for a username. Entering in a username will open a .log file if it exists, 
and if the username specified does not already have a .log file, one will be created. 
(The open file and its changes are not saved until the program is closed, to ensure that the program writes to the file correctly. The user's catalog
will, however, be stored in memory until the program exits, at which point the catalog is written to the file.)

The user is then given a main menu, navigable with the up and down arrow keys and the enter key, with 4 options to choose from: Add, View/Update, Delete, and Exit.

Selecting "Add" allows the user to input a search string, and hitting Enter after typing in a search string will bring up all search results from the database with that search string.
The user can scroll through the results with the up and down arrow keys. The program will let the user know how many results their search returned.
If the user input a string with no results or an invalid string (i.e. they just hit Enter without typing anything), they will be informed that their search returned no results
and will be sent back to the main menu.

After typing in a search string and pressing Enter on a movie record, the user will be prompted to decide what type of media to add to their catalog, from DVD, BluRay, or Digital.
After the user makes this selection, the program will add it to their catalog and notify the user that it was a success.

Selecting "View/Update" allows the user to 

## Info
JD Butler

11761514

CS201 Spring 2019

Dr. Anderson
