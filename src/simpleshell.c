#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "simpleshell.h"
#pragma endregion

void tokenize(char* tokens, char* input)
{
	if (strlen(input) > MAX_USERINPUT) 
	{
		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
		//fflush(stdin); // Flush in case there is chars in buffer
	}
	else
	{
		char* token = strtok(input, delims);
		int counter = 0;

		while (token != NULL)
		{	
			strcpy(&tokens[counter], token); // Adds token to array of tokens

			//printf("%s\n", token); // Uncomment to test (part 1)
			printf("%s\n", &tokens[counter]); // Uncomment to test (part 2)

			token = strtok(NULL, " \t|><&;");	
		}
	}
}

/*
// UNTESTE IN LINUX
int parse(char** tokens)
{
	printf("%s\n", tokens[0]);

	// Change Directory (cd) command: changes directory to given input.
	if (!strcmp(tokens[0], "cd"))
	{
        char* cwd = getCWD();
        char* dir = strcat(cwd, "/");
		//char* nextDir = (strstr(tokens[1], ".") != NULL || strlen(tokens[1]) < 3) ? (strlen(tokens[1]) < 2 ? cwd : cwd) : strcat(cwd, tokens[1]);
		char* nextDir;

		if (strstr(tokens[1], ".") != NULL || strlen(tokens[1]) < 3)
		{
			strcpy(nextDir, cwd);
		}
		else
		{
			nextDir = strcat(cwd, tokens[1]);
		}

		printf("%s\n", nextDir); // Uncomment to test (part 4)

        int success = chdir(nextDir);

		if (success == -1)
		{
			perror("Error");
			return success;
		}
    } 
}
*/

bool exitShell(char* input, bool shellStatus, char* dir)
{
	if (shellStatus)
	{
		char output[25];

		// Determines what message to print
		if (!strcmp(input, "exit\n"))
		{
			sprintf(output, "%s Closing program...\n", prompt);
		}
		else
		{
			strcpy(output, "\nClosing program...\n\n");
		}
		
		printf("%s", output); // Print correct exit message

		//Sets current working directory to initial working directory
		//chdir(dir);
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

// test

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


/*
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
*/

/* 
	Removes new line character (\n) from string
*/
void chomp(char *s) 
{
    while(*s && *s != '\n' && *s != '\r') s++;
 
    *s = 0;
}
