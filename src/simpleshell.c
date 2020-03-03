#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

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
void tokenize(char tokens[MAX_SIZE][MAX_USERINPUT], char* input)
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
int parseInput(char tokens[MAX_SIZE][MAX_USERINPUT], char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	int success = 0;

	char str1[15];          //Used for finding first char of input to find !<no> commands
	char str2[15];
	strcpy(str1, tokens[0]);
	strcpy(str2, "!");

	//addHistory(tokens[MAX_SIZE][MAX_USERINPUT]); //adds command to history

	if (!strcmp(tokens[0], "getpath")) //allows user to see their current env path
	{
		if(tokensCount(tokens) > 1){
			printf("Error: Please use getpath with no addition parameters./n");
		}
		else{
			getPath();
		}
	}
	else if (!strcmp(tokens[0], "setpath"))
	{
		setPath(tokens);
	}
	else if (!strcmp(tokens[0], "cd"))
	{
		success = changeDirectory(tokens);
    }
	else if (!strcmp(tokens[0], "history"))
	{
		viewHistory(history);
	}
	else if (startsWith(tokens[0], "!"))
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
bool exitShell(char* input, bool shellStatus, char* dir, char* path)
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
		setenv("PATH", path, 1);
		getPath();
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
void runExternalCmd(char tokens[MAX_SIZE][MAX_USERINPUT])
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
void setPath(char tokens[MAX_SIZE][MAX_USERINPUT])
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
int changeDirectory(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	int success = -1; // false to start with

	if(tokensCount(tokens[0]) > 2){
		//as stated in the testing pdf, an error should be printed if too many parameters are passed to changeDirectory
		printf("Error. Too many parameters for cd function: \nPlease make sure to use the following format: cd <directory> \n");
		return success;

	}
	if(!strcmp(tokens[1], "\0")){
		//if no parameter is given, set cwd to HOME directory
		success = chdir(HOME_DIR);
	}

	else{
		//else set cwd to the specifided address in parameter
		success = chdir(tokens[1]);
	}

	if(success < 0){ // directory change has failed, print an error

		char errorMsg[23 + MAX_USERINPUT]; // size of the custom errormsg + max possible size of token[1]
		snprintf(errorMsg, sizeof errorMsg, "Error on input: \"cd %s\" ", tokens[1]); // strcat wasn't good for this so snprinf it is
		perror(errorMsg);
		printf("\n"); // extra line for readability

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
		printf("%d: %s\n", (i + 1), history[i]);
	}

	printf("\n");
}

// Untested in Linux
void invokeHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT], char* token)
{
	int index = 0;

	if (token[0] == '!' && token[1] != '!')
	{
		memmove(token, token + 1, strlen(token)); // Removes first character from token.
		sscanf(token,"%02d",&index);
		index--;
	}

	char tokens[MAX_SIZE][MAX_USERINPUT];
	tokenize(tokens, history[index]);
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
        if(counter == 20)
        {
            break;
        }
    }
    fclose (writeFile);
}
*/

/*
void loadHistory(char* fileName) 
{
    FILE *fPointer;
    fileName = strcat(fileName,"/.history");
	fPointer = fopen(fileName, "r");
	char singleLine[MAX_USERINPUT];
	
    if (fPointer == NULL) {
        printf("File could not be found\n");
        exit(1);
    }

	while (!feof(fPointer)) {
		fgets(singleLine, MAX_USERINPUT, fPointer);
		puts(singleLine);
	}

	fclose(fPointer);
	return 0;
}
*/

#pragma endregion

#pragma region Utility
/*
	Determines the size of the "tokens" array.
	Returns: integer representing the size of the array.
*/
int tokensCount(char tokens[MAX_SIZE][MAX_USERINPUT])
{
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
