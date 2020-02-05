#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "simpleshell.h"
#pragma endregion

char** tokenize(char* input)
{
	char** tokens = (char **)malloc(MAX_SIZE * (MAX_INPUT + 1));

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

	return tokens;
}

bool exitShell(char* input, bool shellStatus, char* dir)
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
		
		printf("%s", output); // Print correct exit message

		//Sets current working directory to initial working directory
		chdir(dir);
	}

	return shellStatus;
}

/* UNTESTED IN LINUX
int launchChild(char** tokens)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) // Child process
	{
        if (execvp(tokens[0], tokens) == -1) 
		{
            printf("ohh shit");
			perror("lsh");
        }

        exit(EXIT_FAILURE);
    } 
	else if (pid < 0) // Error forking
	{
        perror("lsh");
    } 
	else // Parent process
	{
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
		{
            wpid = waitpid(pid, & status, WUNTRACED);
        } 
    }

    return 1;
}
*/

char* getCWD()
{
	char* cwd = malloc(PATH_MAX + 1);
	char buffer[PATH_MAX + 1];

	if (getcwd(buffer, PATH_MAX + 1) != NULL)
	{
		cwd = buffer;
	}

	return cwd;
}

void setToHomeDir()
{
	chdir(HOME_DIR);
}

char getPath()
{
	printf(getenv("PATH"));
	printf("\n");
}

void setpath(char** tokens)
{
    if (tokens[1] !=NULL)
	{
        if (tokens[2] == NULL)
		{
            printf("Set path to: %sn", token[1]);

            if (setenv("PATH", tokens[1], 1 == 0)
			{
                printf("Path change successful.\n");
            }
            else
			{
                printf("Invalid path.\n");
            }
        }
        else
		{
            printf("Excessive amount of parameters.\n");
        }
    }
    else
	{
        printf("No path has been provided.\n");
    }
	
    return;
}