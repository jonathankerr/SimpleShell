#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

// (Hares: Change functions to take token array with constants)
#pragma region Debug
/*
	Prints full "tokens" array.
*/
void printFullArray(char array[][MAX_USERINPUT], int maxSize)
{
	for (int i = 0; i < maxSize; i++)
	{
		printf("%i : %s  size = %li \n", i, array[i], strlen(array[i]));
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
void emptyArray(char** array, int maxSize, int maxUserInput)
{
	memset(array,'\0', sizeof(array[0][0]) * maxSize * maxUserInput); // "NULL" character instad of leaving arry entry empty.
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
int parseInput(char tokens[50][512], char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	int success = 0;

	char str1[15];          //Used for finding first char of input to find !<no> commands
	char str2[15];
	strcpy(str1, tokens[0]);
	strcpy(str2, "!");



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
		viewHistory(history);
	}
	else if(startsWith(tokens[0], "!"))
	{
		invokeHistory(history, tokens[0]);
	}
	else
	{
		runExternalCmd(tokens);
	}
	

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
	Determines whether input is an internal command.&&
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
		perror("Invalid command entry");
		_exit(1); // Makes sure it exits (this causes concurence if execv fails so need exit to cover ass)
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

#pragma region Command definitions&&
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
	if (!strcmp(tokens[1], "\0") || strcmp(tokens[2], "\0")) // If input is incorrect, display error message.
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
void addHistory(char *input, char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	input;

	if (!strcmp(history[0], "\0"))  // If "history" array was herebefore empty, ...
	{
		strcpy(history[0], input);	
	}
	else
	{
		for (int i = 20 - 1; i > 0; i--)
		{
			strcpy(history[i], history[i-1]);
		}

		strcpy(history[0], input);
	}
}

/*
	Prints all the entries in the "history" array.
*/
void viewHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	for (int i = 0; i < MAX_HISTORY_SIZE && strcmp(history[i], "\0"); i++)
	{
		printf("%d: %s", (i + 1), history[i]);
	}

	printf("\n");
}

// Untested in Linux
void invokeHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT], char* token)
{
	token = malloc(MAX_USERINPUT);

	if (token[0] == '!' && isalpha(token[1]))
	{
		memmove(token, token + 1, strlen(token)); // Removes first character from token.
	}
	else
	{
		int buffer = (int)((ceil(log10(0))+1)*sizeof(char));
		char str[buffer];

		sprintf(str, "%d", 0);
		strcpy(token, str);
	}

	char tokens[MAX_SIZE][MAX_USERINPUT];
	tokenize(tokens, history[atoi(token)]);
	parseInput(tokens, history);
}

/*
void writeHistory(char* fileName, char *history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
    FILE *writeFile;
    int counter;
    fileName = strcat(fileName,"/.history");
    writeFile = fopen ( fileName, "w");
    
    if (writeFile == NULL)
    {
        printf("Cannot Open File\n");
        exit(1);
    }
    
    counter = 0;
    
    while(history[counter][MAX_USERINPUT] !=0)
	{
        fprintf(writeFile,"%s\n", history[counter][MAX_USERINPUT]);
        counter++;
        if(counter = 20)
        {
            break;
        }
    }
    fclose (writeFile);
}
*/
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

bool startsWith(const char *string, const char *substring)
{
   if (strncmp(string, substring, strlen(substring)) == 0) 
   {
	   return 1;
   }

   return 0;
}
#pragma endregion
