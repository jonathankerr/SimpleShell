#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleshell.h"
#pragma endregion

void tokenize(char* input)
{
	if (strlen(input) > MAX_INPUT) 
	{
		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
		fflush(stdin); //flush encase there is chars in buffer
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

/*
int launchChild(char*[] tokens)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) 
	{
        // Child process
        if (execvp(args[0], args) == -1) 
		{
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } 
	else if (pid < 0) 
	{
        // Error forking
        perror("lsh");
    } 
	else 
	{
        // Parent process
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
		{
            wpid = waitpid(pid, & status, WUNTRACED);
        } 
    }

    return 1;
}
*/