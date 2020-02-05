#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleshell.h"
#pragma endregion

int main() 
{
	char* initDir = getCWD();
	bool terminated = FALSE;
	char** tokens; // Array of strings that will hold 50 strings of 50 characters each

    //printf("CWD: %s\n\n", getInitDir()); // Uncomment to test (part 3)

	setToHomeDir(); // Sets cwd to users home dir

    while (!terminated)
    {
		char input[MAX_INPUT];

		printf("%s", prompt);

		fgets(input, 9999, stdin); // DO NOT REMOVE 9999
		tokens = tokenize(input);

		if (strcmp(input, "getpath\n") == 0) //allows user to see their current env path
		{
			printf("->");
			getPath();
		}
	
		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial current working directory
		terminated = exitShell(input, (strcmp(input, "exit\n") == 0 || feof(stdin)), initDir);
    }
}