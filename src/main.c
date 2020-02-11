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
	char tokens[MAX_SIZE][MAX_USERINPUT];  // Array of strings that will hold 50 strings of 50 characters each

    printf("CWD: %s\n\n", getInitDir()); // Uncomment to test (part 3)

	setToHomeDir(); // Sets cwd to users home dir

    while (!terminated)
    {
		char input[MAX_USERINPUT];

		printf("%s", prompt);

		fgets(input, MAX_USERINPUT, stdin);
		tokenize(*tokens, input);

		parseInput(*tokens);
	
		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial working directory
		terminated = exitShell(input, (strcmp(input, "exit\n") == 0 || feof(stdin)), "lol");
    }

	return 0;
}