#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

int main() 
{
	char* initDir = getCWD(); // String that holds the initial working directory.
	char* initPath = getenv("PATH"); // String that holds the initial path.

	bool terminated = FALSE; // Boolean value that determines whether shell should be terminated.
	char tokens[MAX_SIZE][MAX_USERINPUT];  // Array of strings that holds tokens.
	char history[MAX_HISTORY_SIZE][MAX_USERINPUT]; // Array of strings that holds history.
	//alias aliase[MAX_ALIAS_SIZE][MAX_USERINPUT]; // Array of custom type "alias" that will hold all aliases defined by the user.

	char input[MAX_USERINPUT]; // String that holds user input.

    //printf("CWD: %s\n\n", getCWD()); // Uncomment to test (part 3)

	chdir(getenv("HOME")); // Sets current working directory to the user's "HOME" directory.
	
	emptyArray(history, MAX_HISTORY_SIZE, MAX_USERINPUT); // Initializes "history" array with null characters.

    while (!terminated)
    {
		emptyArray(tokens, MAX_SIZE, MAX_USERINPUT); // Set all elements in "tokens" array to null characters.
		
		printf("%s", PROMPT); // Print prompt.

		fgets(input, MAX_USERINPUT, stdin); // Get user input.

		if (input[0] != '!')
		{
			// If user isn't invoking history, save input to history.
			chomp(input);
			addHistory(input, history);
		}

		tokenize(tokens, input); // Tokenizes input.

		//printFullArray(history, MAX_HISTORY_SIZE);  //uncomment to see if garbage still in arrays unused indexes
		//printTokens(tokens);  //uncomment to show part one working

		terminated = exitShell(tokens[0], (strcmp(input, "exit") == 0 || feof(stdin)), initDir, initPath, history); // Closes program if exit is typed or if Ctrl-D is pressed.

		if (!terminated)
		{
			parseInput(tokens, history); // Checks for internal/external commands, then calls appropriate function.
		}
    }

	return 0;
}

