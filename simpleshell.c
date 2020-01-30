#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    bool terminated = FALSE;
    
    while (!terminated)
    {
		char tokens[50][50]; // Array of strings that will hold 50 strings of 50 characters each
		char input[MAX_INPUT];

		printf("%s", prompt);

		fgets(input, 9999, stdin); // DO NOT REMOVE 9999
		char *token = strtok(input, delims);
		
		int counter = 0;
		while (token != NULL)
		{	
			strcpy(tokens[counter], token);
			//printf("%s\n", token); // Uncomment to test (part 1)
			printf("%s\n", tokens[counter]); // Uncomment to test (part 2)
			token = strtok(NULL, " \t|><&;");	
		}

		if (strlen(input) > MAX_INPUT) 
		{
			printf("Invalid input - please enter no more than 512 characters.\n");
		}

		if (strcmp(input, "exit\n") == 0 || feof(stdin)) // Closes program if exit is typed or if Ctrl-D is pressed
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
			terminated = TRUE;
		}
    }
}
