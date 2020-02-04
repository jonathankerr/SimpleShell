#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleshell.h"
#pragma endregion

int main() 
{
    printf("CWD: %s\n\n", getInitDir()); // Uncomment to test (part 3)

    printf("PATH : %s\n", getenv("PATH")); //Uncomment to test getting path
	printf("HOME : %s\n", getenv("HOME")); //Uncomment to test getting home directory

    bool terminated = FALSE;
    
    while (!terminated)
    {
		char input[MAX_INPUT];

		printf("%s", prompt);

		fgets(input, 9999, stdin); // DO NOT REMOVE 9999
		tokenize(input);
	
		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial current working directory
		terminated = exitShell(input, (strcmp(input, "exit\n") == 0 || feof(stdin)), "lol");
    }
}
