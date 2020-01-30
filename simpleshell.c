#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    bool terminated = false;
    
    while (terminated == false)
    {
		printf("%s", prompt);
		char input[MAX_INPUT];
		fgets(input, 9999, stdin); // DONT REMOVE 9999
		char *token = strtok(input, " \t|><&;");
		
		while(token != NULL)
		{				
			//printf("%s\n",token); // Uncomment print statement to test
			token = strtok(NULL, " \t|><&;");	
		}

		if (strlen(input) > MAX_INPUT) {
			printf("Invalid input - please enter no more than 512 characters.\n");
		}

		if (strcmp(input, "exit\n") == 0 || feof(stdin)) // Closes program if exit is typed // Closes program if Ctrl-D is pressed
		{	
			char* output = feof(stdin) ? "Closing program...\n" : "-> Closing program...\n";
			printf(output);
			terminated = true;
		}
    }
}
