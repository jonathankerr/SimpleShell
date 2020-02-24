#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

// (Hares: why are some functions using MAX_SIZE AND MAX_USERINPUT and some hardcoded int values?)
#pragma region Debug
/*
	Prints full "tokens" array.
*/
void printFullArray(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	printf("---TESTING 2D ARRAY CONTENTS ---\n");
	for (int i = 0; i < MAX_SIZE; i++)
	{
		printf("%i : %s  size = %li \n",i, tokens[i], strlen(tokens[i]));
	}
}

/*
	Prints every entry in the "tokens" array, one on each line.
*/
void printTokens(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	//char **temp = tokens;
	int count = 0;

	while(strcmp(tokens[count], "\0"))
	{
		printf("\n %s ", tokens[count++]);
	}
}
#pragma endregion

#pragma region Miscellaneous
/*
	Creates an array of a constant size with the "\0" character as each entry.
*/
void emptyArray(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	memset(tokens,'\0', sizeof(tokens[0][0]) * MAX_SIZE * MAX_USERINPUT); // "NULL" character instad of leaving arry entry empty.
}

// Takes in user input, tokenizes it and fills the "tokens" array with these tokens.
void tokenize(char tokens[50][512], char* input)
{
	if (strlen(input) > MAX_USERINPUT)
	{
		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
		fflush(stdin); // Flush in case there is chars in buffer
	}
	else
	{
		char* token = strtok(input, DELIMS); // Tokenize input with the characters defined in the "DELIMS" array.
		int counter = 0;

		while (token != NULL)
		{
			//strcpy(tokens[1], "\0");
			strcpy(tokens[2], "\0"); // turn this into a function which sets all index to undefined which is called after every execution after input

			chomp(token); // Removes "new line" character.
			strcpy(tokens[counter], token); // Adds token to the "tokens" array.

			//printf("%s\n", token); // Uncomment to test (part 1)
			//printf("%s\n", tokens[counter]); // Uncomment to test (part 2)

			token = strtok(NULL, DELIMS); // Tokenize with same settings.

			counter++;
		}
	}
}

/*
	Reads in tokens to check what function to execute.
	Returns: integer less than 0 in case of failure and 0 or greater in case of success.
*/
int parseInput(char tokens[50][512])
{
	int success = 0;

	//addHistory(tokens[50][512]); //adds command to history

	if (!strcmp(tokens[0], "getpath")) //allows user to see their current env path
	{
		getPath();
	}
	else if (!strcmp(tokens[0], "setpath"))
	{
		setPath(tokens);
	}
	else if (!strcmp(tokens[0], "cd"))
	{
		success = changeDirectory(tokens);
    }
	else if(!strcmp(tokens[0], "history"))
	{
		viewHistory();
	}
	else if(!strcmp(tokens[0], "!"))
	{
		printf("I got ya"); // (Hares: I thought this was already implemented)
	}
	/*
	else
	{
		printf("That functionality is incoming soon."); // (Hares: what command is this?)
	}
	*/

	return success;
}

/*
	Determines whether user wants to exit the shell.
	Returns: false if shell isn't terminated and true if it is.
*/
bool exitShell(char* input, bool shellStatus, char* dir)
{
	printf("\n");

	if (shellStatus)
	{
		char output[25]; // String to hold exit message.

		// Determines what message to print.
		if (!strcmp(input, "exit"))
		{
			sprintf(output, "%s Closing program...\n", prompt);
		}
		else
		{
			strcpy(output, "\nClosing program...\n\n");
		}
		
		printf("%s", output); // Print correct exit message.

		chdir(dir); // Sets current working directory to initial working directory.
	}

	return shellStatus;
}

/*
	Determines whether input is an internal command.
	Returns: false in case input is not an internal command and true if it is.
*/
bool isInternalCmd(char* command)
{
	int size = 15;// this line caused a seg fault, duno why -> sizeof(INTERNAL_FUNCTIONS)/sizeof(INTERNAL_FUNCTIONS[0]);

	for (int i = 0; i < size; i++)
	{
		if (strcmp(INTERNAL_FUNCTIONS[i], command) == 0) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

/*
	Runs an internal command entered by the user.
*/
void runExternalCmd(char tokens[50][512])
{


	pid_t c_pid, pid;
	int status;
	char* tempArgs[51]; //possible work around allowing us to keep 2D array

	int numOfTokens = tokensCount(tokens);

	for (int i = 0; i < numOfTokens; i++)
	{

			tempArgs[i] = tokens[i];

	}

	tempArgs[numOfTokens] = NULL;

	c_pid = fork();

	if (c_pid == -1)
	{
		perror("Forked up! ");
		_exit(1);
	}

	if (c_pid == 0)
	{
		//execv(tokens[0], tokens);
		//execv(testArgs[0], testArgs);
		execvp(tempArgs[0], tempArgs);
		perror("Invalid command entry \n");
		_exit(1); // Makes sure it exits (Hares: ?) this causes concurence if execv fails so need exit to cover ass
	}
	else if (c_pid > 0)
	{
		if((pid = wait(&status)) < 0){
			perror("Wait failed; ");
			_exit(1);
		}

    }
}

/*
	Gets the current working directory.
	Returns: string representing the current working directory.
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
#pragma endregion

#pragma region Command definitions
/*
	Prints the current path.
*/
void getPath()
{
	printf("Current path: %s\n", getenv("PATH"));
}

/*
	Sets the path specified by the user.
*/
void setPath(char tokens[50][512])
{
	if (!strcmp(tokens[1], "\0") || strcmp(tokens[2], "\0") || strcmp(tokens[3], "\0")) // If input is not in corrent , display error message.
	{
		printf("Invalid input.\nPlease make sure to use the following format: getpath <path>\n");
	}
	else
	{
		const char *path = tokens[1];
		setenv("PATH", path, 1);

		printf("Path set to: %s\n", tokens[1]);
	}
}

/* 
	Changes current working directory to directory specified by user.
	Returns: integer less than 0 in case of failure and 0 or greater in case of success.
*/
int changeDirectory(char tokens[50][512])
{
	int success = 0;

	if (tokens[1] == NULL) // If no directory is specified, display error message.
	{
		printf("Invalid input.\nPlease make sure use to use the following format: cd <directory name>.\n");
	}
	else
	{
		char* cwd = getCWD();
		char* dir = strcat(cwd, "/");
		//char* nextDir = (strstr(tokens[1], ".") != NULL || strlen(tokens[1]) < 3) ? (strlen(tokens[1]) < 2 ? cwd : cwd) : strcat(cwd, tokens[1]); // (Hares: does this work?)
		char* nextDir;
		if (strstr(tokens[1], ".") != NULL || strlen(tokens[1]) < 3)
		{
			strcpy(nextDir, cwd);
		}
		else
		{
			nextDir = strcat(cwd, tokens[1]);
		}
		
		int success = chdir(nextDir);

		//printf("%s\n", getCWD()); // Uncomment to test (part 4)

		if (success == -1)
		{
			perror("Error: ");
			return success;
		}
	}
	
	return success;
}

/*
	Adds tokens entered by user to history.
*/
void addHistory(char *input)
{/*
	char *rawInput = input;

	if (!strcmp(history[0], "\0"))  // If "history" array was herebefore empty, ...
	{
		strcpy(history[0], rawInput);	
	}
	else if (strcmp(history[0], "\0")) // Else, ...
	{
		for (int i = 20 - 1; i > 0; i--)
		{
			strcpy(history[i], history[i-1]);
		}

		strcpy(history[0], rawInput);
	}
*/}

/*
	Prints all the entries in the "history" array.
*/
void viewHistory()
{/*
	printf("\nFull history array: \n"); // (Hares: I don't think this is how we're supposed to show history)

	for (int i = 0; i < 20; i++) // Loops through and prints whole history array.
	{
		printf("%d: %s\n", i + 1, history[i]);
	}
*/}

/* Commented Out until first marking to make sure it doesn't effect anything
void writeHistory(char* fileName, char *history[20])
{
    FILE *writeFile;
    int counter;
    filename = strcat(fileName,"/.history");
    writeFile = fopen ( fileName, "w");
    
    if (writeFile == NULL)
    {
        printf("Cannot Open File\n");
        exit(1);
    }
    
    counter = 0
    
    while(history[counter] !=0)
    {
        fprintf(writeFile,"%s\n", history[counter]);
        counter++;
        if(counter = 20)
        {
            break;
        }
    }
    fclose (writeFile);
}
*/


        }
    }
#pragma endregion

#pragma region Utility
/*
	Determines the size of the "tokens" array.
	Returns: integer representing the size of the array.
*/
int tokensCount(char tokens[50][512]){
	int count = 0;

	for(int i = 0; i < MAX_SIZE; i++)
	{
		if(strlen(tokens[i]) != 0)
		{
			count++;
		}
	}

	return count;
}

/* 
	Removes new line character "\n" or return character "\r" from string.
*/
void chomp(char *s) 
{
    while (*s && *s != '\n' && *s != '\r') 
	{
		s++;
	}
 
    *s = 0;
}
#pragma endregion
