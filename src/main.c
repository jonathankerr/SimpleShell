#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

int main() 
{
	char* initDir = getCWD();
	char* initPath = getenv("PATH");
	bool terminated = FALSE;
	char tokens[MAX_SIZE][MAX_USERINPUT];  // Array of strings that will hold 50 strings of 50 characters each
	char history[MAX_HISTORY_SIZE][MAX_USERINPUT];
	char input[MAX_USERINPUT];

    //printf("CWD: %s\n\n", getCWD()); // Uncomment to test (part 3)

	chdir(getenv("HOME")); // Sets cwd to users home dir
	emptyArray(history, MAX_HISTORY_SIZE, MAX_USERINPUT);
    while (!terminated)
    {
		emptyArray(tokens, MAX_SIZE, MAX_USERINPUT);
		
		//char input[MAX_USERINPUT];

		printf("%s", PROMPT);

		fgets(input, MAX_USERINPUT, stdin);

		if (input[0] != '!')
		{
			chomp(input);
			addHistory(input, history);  //sends the raw input before tokenizing
		}

		tokenize(tokens, input);

		//printFullArray(history, MAX_HISTORY_SIZE);  //uncomment to see if garbage still in arrays unused indexes
		//printTokens(tokens);  //uncomment to show part one working

		// Closes program if exit is typed or if Ctrl-D is pressed
		// Also sets the current working directory to the initial working directory
		terminated = exitShell(tokens[0], (strcmp(input, "exit") == 0 || feof(stdin)), initDir, initPath, history);



		// Checks for internal/external cmd, then calls appropriate function
		if(!terminated)
		{
			parseInput(tokens, history);
		}
    }

	return 0;
}

