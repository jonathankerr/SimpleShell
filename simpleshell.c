#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

void tokenize(char* input)
{
	if (strlen(input) > MAX_INPUT) 
	{
		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
	}
	else
	{
		char* token = strtok(input, delims);
		int counter = 0;

		while (token != NULL)
		{	
			strcpy(tokens[counter], token); // Adds token to array of tokens

			//printf("%s\n", token); // Uncomment to test (part 1)
			printf("%s\n", tokens[counter]); // Uncomment to test (part 2)

			token = strtok(NULL, " \t|><&;");	
		}
	}
}

bool exitShell(char* input, bool shellStatus)
{
	if (shellStatus)
	{
		char output[25];

		// Determines what message to print
		if (strcmp(input, "exit\n") == 0)
		{
			sprintf(output, "%s Closing program...\n", prompt);
		}
		else
		{
			strcpy(output, "\nClosing program...");
		}
		
		printf("%s", output);
	}

	return shellStatus;
}