#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

int main() 
{
	char* initDir = getCWD();
	bool terminated = FALSE;
	char tokens[MAX_SIZE][MAX_USERINPUT];  // Array of strings that will hold 50 strings of 50 characters each
	char history[MAX_HISTORY_SIZE][MAX_USERINPUT];
	char input[MAX_USERINPUT];

    //printf("CWD: %s\n\n", getCWD()); // Uncomment to test (part 3)

	chdir(HOME_DIR); // Sets cwd to users home dir

    while (!terminated)
    {
		emptyArray(tokens, MAX_SIZE, MAX_USERINPUT);

		//char input[MAX_USERINPUT];

		printf("%s", prompt);

		fgets(input, MAX_USERINPUT, stdin);

		addHistory(input, history);  //sends the raw input before tokenizing

		tokenize(tokens, input);

		//printFullArray(history, MAX_HISTORY_SIZE);  //uncomment to see if garbage still in arrays unused indexes
		//printTokens(tokens);  //uncomment to show part one working

		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial working directory
		terminated = exitShell(tokens[0], (strcmp(input, "exit") == 0 || feof(stdin)), initDir);

		// Checks for internal/external cmd, then calls appropriate function
		if(!terminated)
		{
			parseInput(tokens, history);
		}
    }

	return 0;
}

