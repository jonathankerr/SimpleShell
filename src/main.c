#pragma region Libraries
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
		emptyArray(tokens);

		char input[MAX_USERINPUT];

		printf("%s", prompt);

		fgets(input, MAX_USERINPUT, stdin);
		//printf("\nInput: %s\n", input);

		tokenize(tokens, input);

		//printFullArray(tokens);  //uncomment to see if garbage still in arrays unused indexes
		//printTokens(tokens);  //uncomment to show part one working


		/* checks for internal/external cmd, then calls appropriate function */
		if(isInternalCmd(tokens[0])){
			parseInput(tokens);
		}
		else{
			//printf("Not an internal cmd - fork forked up atm, internals should work \"path\" and stuff");
			runExternalCmd(tokens);
		}
		
		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial working directory
		terminated = exitShell(tokens[0], (strcmp(input, "exit") == 0 || feof(stdin)), "lol");
    }

	return 0;
}

