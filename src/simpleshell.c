/* 
TODO:
- INVOKE HISTORY
- PARAMETERS IN INVOKE HISTORY
- ERROR HANDLING IN GENERAL (RETURN WHEN ERROR OCCURS)
*/



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
	int count = 0;

	while (strcmp(tokens[count], "\0"))
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

/*
	Takes in user input, tokenizes it and fills the "tokens" array with these tokens.
*/
void tokenize(char tokens[MAX_SIZE][MAX_USERINPUT], char* input)
{
	if (strlen(input) > MAX_USERINPUT)
	{	 
   		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
		fflush(stdin); // Flush in case there is chars in buffer.
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
*/
void parseInput(char tokens[MAX_SIZE][MAX_USERINPUT], char history[MAX_HISTORY_SIZE][MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE])
{
	// Used for finding first char of input to find !<no> commands.
	char str1[15];          
	char str2[15];
	strcpy(str1, tokens[0]);
	strcpy(str2, "!");

	if (!strcmp(tokens[0], "getpath"))
	{
		getPath(tokens);
	}
	else if (!strcmp(tokens[0], "setpath"))
	{
		setPath(tokens);
	}
	else if (!strcmp(tokens[0], "cd"))
	{
		changeDirectory(tokens);
    }
	else if (!strcmp(tokens[0], "history"))
	{
		viewHistory(history);
	}
	else if (startsWith(tokens[0], "!"))
	{
		if (tokens[0][1] == '!' && tokens[1] != "\0")
		{
			printf("Invalid input.\nPlease make sure to use the following format: <!!>. \n");
		}
		else
		{
			invokeHistory(history, tokens[0]);
		}
	}/*
	else if(!strcmp(tokens[0], "alias") && tokensCount(tokens) < 2){
		printAllAliases(aliases);
	}
	else if(!strcmp(tokens[0], "alias")){
		addAlias(tokens, aliases);
	}
	else if(!strcmp(tokens[0], "unalias")){
		removeAlias(tokens[0], aliases);
	}*/
	else
	{
		runExternalCmd(tokens);
	}

}

/*
	Determines whether user wants to exit the shell.
	Returns: false if shell isn't terminated and true if it is.
*/
bool exitShell(char* input, bool shellStatus, char* dir, char* path, char tokens[MAX_HISTORY_SIZE][MAX_USERINPUT], char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	printf("\n");

	if (shellStatus)
	{
		char output[25]; // String to hold exit message.

		writeHistory(history);
		setenv("PATH", path, 1);
		getPath(tokens);
		
		// Determines what message to print.
		if (!strcmp(input, "exit"))
		{
			sprintf(output, "%s Closing program...\n", PROMPT);
		}
		else
		{
			strcpy(output, "\nClosing program...\n\n");
		}
		
		printf("%s", output); // Print correct exit message.
	}

	return shellStatus;
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
void getPath(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	if (tokensCount(tokens) > 1)
	{
		printf("Invalid input: no parameters allowed with <getpath>./n");
	}
	else
	{
		printf("Current path: %s\n", getenv("PATH"));
	}
}

/*
	Sets the path specified by the user.
*/
void setPath(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	if (!strcmp(tokens[1], "\0") || strcmp(tokens[2], "\0")) // If input is incorrect, display error message.
	{
		printf("Invalid input.\nPlease make sure to use the following format: setpath <path>\n");
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
*/
void changeDirectory(char tokens[MAX_SIZE][MAX_USERINPUT])
{
	int success = -1;

	if (tokensCount(tokens[0]) > 2)
	{
		printf("Invalid input: please make sure to use the following format: <cd> <directory> \n");
	}

	if (!strcmp(tokens[1], "\0")) // If no parameter is given, ... 
	{
		success = chdir(getenv("HOME")); // ... Set cwd to HOME directory.
	}
	else
	{
		success = chdir(tokens[1]); // Else, set cwd to the specifided address in parameter.
	}

	if (success < 0) // If changing directory fails, ...
	{ 
		// ... Print error message.
		char errorMsg[23 + MAX_USERINPUT]; // size of the custom errormsg + max possible size of token[1]
		snprintf(errorMsg, sizeof errorMsg, "Error on input: \"cd %s\" ", tokens[1]); // strcat wasn't good for this so snprinf it is
		perror(errorMsg);

		printf("\n"); // extra line for readability
	}
}

/*
	Adds tokens entered by user to history.
*/
void addHistory(char *input, char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	if (!strcmp(history[0], "\0"))  // If "history" array was herebefore empty, ...
	{
		strcpy(history[0], input); // ... Add input as first element of array.
	}
	else
	{
		// Else, add input to array in circular fashion.
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

/*
	Invokes the command specified by the user stored in history.
*/
void invokeHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT], char* token, alias aliases[MAX_ALIAS_SIZE])
{
	int index = 0;

	if (token[0] == '!' && token[1] != '!')
	{
		memmove(token, token + 1, strlen(token)); // Removes first character from token.
		sscanf(token,"%02d", &index);

		if (index < 0)
		{
			index *= -1;
		}
		
		if (index == 0 || (index - 1) >= historyCount(history)) 
		{
			printf("Invalid input.\nPlease make sure to use the following format: <!><index>. Type <history> to view valid indices.\n\n");
			return;
		}

		index--;	
	}
	else
	{
		printf("Invalid input.\nPlease make sure to use the following format: <!!>.\n\n");
		return;
	}

	if (historyCount(history) > 0)
	{
		char tokens[MAX_SIZE][MAX_USERINPUT];
		emptyArray(tokens, MAX_SIZE, MAX_USERINPUT);

		char tempInput[MAX_USERINPUT];
		strcpy(tempInput, history[index]);

		tokenize(tokens, tempInput);
		parseInput(tokens, history, aliases);
	}
	else
	{
		printf("No commands in history. Type a command to add it to history.\n\n");
	}
}

/*
	Writes hsitory to an external file.
*/
void writeHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	FILE *fp;
	fp = fopen ("C:/Users/jonat/Desktop/Uni/Year 2/CS210/Sem2/simple-shell/src/history.txt", "w"); //CHANGE THIS TO EITHER CWD OR INIT DIR

	for (int i = 1; i < MAX_HISTORY_SIZE && strcmp(history[i], "\0"); i++)
	{
		fprintf(fp, history[i]);
		fprintf(fp, "\n");
	}

	fclose (fp);
}

/*	 
	Loads history from an external file into the "history" array.
*/

void loadHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT]) 
{
    FILE *fp;
	fp = fopen(historyFile, "r");
	char singleLine[MAX_USERINPUT];
	int i = 0;


    if (fp == NULL) {
        printf("File could not be found\n");
    }
	else {
		while (fgets(singleLine, MAX_USERINPUT, fp) != NULL) {
			//history[i] == fgets(singleLine, MAX_USERINPUT, fp);
			//fgets(singleLine, MAX_USERINPUT, fp);
			addHistory(singleLine, history);
			//strcpy(history[i], ins);
			i++;
		}
	}

	fclose(fp);
}


void printAllAliases(alias aliases[MAX_ALIAS_SIZE]){

	if(strlen(aliases[0].command) < 1){
		printf("The aliases list is currently empty\n");
		return;
	}

	printf("The List of current Aliases\n");
	for(int i = 0; i < MAX_ALIAS_SIZE; i++){
		if(strlen(aliases[i].command) < 1){
			return; // we hit an empty alias
		}
		printf("Alias: %s , Command: %s\n", aliases[i].name, aliases[i].command);

	}
}

/*
Checks the Alias array and returns the index of the alias if it is present in the array.
Returns -1 if it is not found.
*/
int isAlias(char argument[MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){
	

	for(int i = 0; i < MAX_ALIAS_SIZE; i++){
		if(!strcmp(aliases[i].name, argument)){
			return i; // returns the index of the alias if it is found
		}
	}

	return -1; //return -1 to signify alias is not present

}
/*
Adds the new Alias to the Alias array. 
IF the alias is currently in use, it is overriden.
IF the aliases array is full, it is not added and an error printed
*/
/*
int addAlias(char tokens[MAX_SIZE][MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){
	
	if(countTokens(tokens) < 3){
		printf("Too few Arguements. Please use the following syntax; alias Alias Command");
		return;
	}

	if(isAliasesFull(aliases)){
		printf("Error: Aliases list is full");
		return;
	}
	
	int checkIfAlias = isAlias(tokens[0], aliases);
	char newAlias[MAX_USERINPUT] = tokens[1];
	char newCommand[MAX_USERINPUT];

	for(int i = 2; i < tokensCount(tokens); i++){
		strcat(newCommand, tokens[i]);
	}

	if(checkIfAlias >= 0){
		aliases[checkIfAlias].name = newAlias;
		aliases[checkIfAlias].command = newCommand;
	}

	//else find end of used indexes and add new Alias
	int i = 0;
	while(strlen(aliases[i].name) > 0){
		i++;
	}

	aliases[i].name = newAlias;
	aliases[i].command = newCommand;
}
*/
/*
bool isAliasesFull(alias aliases[MAX_ALIAS_SIZE]){
	int i = 0;
	while(strlen(aliases[i].name) > 0 && i < MAX_ALIAS_SIZE){  // gets index of first empty index
		i++;
	}

	if(i == MAX_ALIAS_SIZE-1) {
		return TRUE;
	}
	return FALSE;

}
*/

/*
void removeAlias(char deadAlias[MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){
	
	int checkIfAlias = isAlias(deadAlias, aliases);
	if(checkIfAlias < 0){
		printf("Error: %s is not an alias, therefore cannot remove from alias list", deadAlias);
		return;
	}

	aliases[checkIfAlias].name = "";
	aliases[checkIfAlias].command = ""; //empty this entry

	//shift all other entries down one index
	for(int i = checkIfAlias+1; i < MAX_ALIAS_SIZE; i++){
		aliases[i-1].name = aliases[i].name;
		aliases[i-1].command = aliases[i].command;
	}

	printf("%s alias has been removed.", deadAlias);


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

	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (strlen(tokens[i]) != 0)
		{
			count++;
		}
	}

	return count;
}

/*
	Determines the size of the "history" array.
	Returns: integer representing the size of the array.
*/
int historyCount(char history[MAX_HISTORY_SIZE][MAX_USERINPUT])
{
	int count = 0;

	for (int i = 0; i < MAX_HISTORY_SIZE; i++)
	{
		if (strlen(history[i]) != 0)
		{
			count++;
		}
	}

	return count;
}

/* 
	Removes new line character "\n" or return character "\r" from string.
*/
void chomp(char *string) 
{
    while (*string && *string != '\n' && *string != '\r') 
	{
		string++;
	}
 
    *string = 0;
}

/*
	Check whether string starts with the specified substring.
	Returns: false if substring is not part of string and true if it is.
*/
bool startsWith(const char *string, const char *substring)
{
   if (strncmp(string, substring, strlen(substring)) == 0) 
   {
	   return TRUE;
   }

   return FALSE;
}
#pragma endregion
