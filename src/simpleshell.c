/*
	CS210: Simple shell exercise.  File: simpleshell.c
   
      Group 9.
	Hares Mahmood
	Justin Leung
	Jonathan Kerr
	Matthew King
	James Boner

*/
#pragma region Libraries 
#include "simpleshell.h"
#pragma endregion

#pragma region Debug
/*
	Debug function. Prints full "tokens" array.
*/
void printFullArray(char array[][MAX_USERINPUT], int maxSize)
{
	for (int i = 0; i < maxSize; i++)
	{
		printf("%i : %s  size = %li \n", i, array[i], strlen(array[i]));
	}
}

/*
	Prints every entry none null value in the "tokens" array, one on each line.
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
	Flushes the 2d array, replacing any vaues in all indexes with the null character '\0'
*/
void emptyArray(char** array, int maxSize, int maxUserInput)
{
	memset(array,'\0', sizeof(array[0][0]) * maxSize * maxUserInput); // "NULL" character instead of leaving array entry empty.
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

	// Removes aliased command from tokens, in order to properly execute command.
	findAndReplaceAliases(tokens, aliases);

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
	}
	else if (!strcmp(tokens[0], "alias") && tokensCount(tokens) < 2)
	{
		printAllAliases(aliases);
	}
	else if (!strcmp(tokens[0], "alias") && tokensCount(tokens) >= 2)
	{
		addAlias(tokens, aliases);
	}
	else if (!strcmp(tokens[0], "unalias"))
	{
		removeAlias(tokens[1], aliases);
	}
	else
	{
		runExternalCmd(tokens);
	}
}

/*
	Determines whether user wants to exit the shell.
	Returns: false if shell isn't terminated and true if it is.
*/
bool exitShell(char* input, bool shellStatus, char* dir, char* path, char tokens[MAX_HISTORY_SIZE][MAX_USERINPUT], char history[MAX_HISTORY_SIZE][MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE])
{
	printf("\n");

	if (shellStatus)
	{
		char output[25]; // String to hold exit message.

		writeHistory(history, dir);
		saveAliasesToFile(aliases, dir);
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
	char* tempArgs[51]; //temp varriable pointer for use with execvp

	int numOfTokens = tokensCount(tokens);

	for (int i = 0; i < numOfTokens; i++)
	{

			tempArgs[i] = tokens[i];

	}

	tempArgs[numOfTokens] = NULL; //execvp requries null terminator

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
		_exit(1); // Makes sure it exits (this causes unwanted parallel function to keep running if execv fails, so need exit to cover it)
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
void writeHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT], char* dir)
{
	FILE *fp;
	char tempFileName [MAX_USERINPUT]; 
	sprintf(tempFileName, "%s/%s", dir, historyFile); //constuct full path name needed to save file

	fp = fopen (tempFileName, "w"); 

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

void loadHistory(char history[MAX_HISTORY_SIZE][MAX_USERINPUT], char* dir) 
{
    FILE *fp;
	char tempFileName [MAX_USERINPUT]; 
	sprintf(tempFileName, "%s/%s", dir, historyFile); //construct full path name needed to load file

	fp = fopen(tempFileName, "r");
	char singleLine[MAX_USERINPUT];
	int i = 0;


    if (fp == NULL) {
        // no file exists, therefore do nothing
		return;
    }
	else {
		while (fgets(singleLine, MAX_USERINPUT, fp) != NULL) {
			
			char* temp = strtok(singleLine, "\n"); //remove \n characters
			addHistory(temp, history);
		
			i++;
		}
	}

	fclose(fp);
}

/* 
Prints the contensts of the Aliases array 
*/
void printAllAliases(alias aliases[MAX_ALIAS_SIZE]){

	if(strlen(aliases[0].command) < 1){
		printf("The aliases list is currently empty\n");
		return;
	}

	printf("The List of current Aliases\n\n");
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
int isAlias(char argument[MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE])
{
	for (int i = 0; i < MAX_ALIAS_SIZE; i++)
	{
		if (!strcmp(aliases[i].name, argument))
		{
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
int addAlias(char tokens[MAX_SIZE][MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){
	
	if(tokensCount(tokens) < 3){
		printf("Too few Arguements. Please use the following syntax; alias Alias Command\n");
		return 0;
	}

	if(isAliasesFull(aliases)){
		printf("Error: Aliases list is full\n");
		return 0;
	}
	
	int checkIfAlias = isAlias(tokens[0], aliases);
	char newCommand[MAX_USERINPUT];
	strcpy(newCommand, ""); // empty the variable, this is needed, strangely! (james)

	//add all the tokens to the command since it seems commands can have multiple arguments, i.e flags.
	strcat(newCommand, tokens[2]);
	for(int i = 3; i < tokensCount(tokens); i++){
		strcat(newCommand, " "); //add a space after each token
		strcat(newCommand, tokens[i]);
		
	}

	if(checkIfAlias >= 0){
		strcpy(aliases[checkIfAlias].name, tokens[1]);
		strcpy(aliases[checkIfAlias].command, newCommand);
		printf("Alias \"%s\" has been added.\n", aliases[checkIfAlias].name);
		return 1;
	}

	//else find end of used indexes and add new Alias
	int i = 0;
	while(strlen(aliases[i].name) > 0){
		i++;
	}

	strcpy(aliases[i].name, tokens[1]);
	strcpy(aliases[i].command, newCommand);

	printf("Alias \"%s\" has been added.\n", aliases[i].name);
}

/* Checks to see if the alias Array is full. */
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
/* If the alias, given by the char array, is present, it removes the alias and shifts the 
index all of following aliases as to avoid empty gaps in the array.
*/
void removeAlias(char deadAlias[MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){
	
	int checkIfAlias = isAlias(deadAlias, aliases);
	if(checkIfAlias < 0){
		printf("Error: %s is not an alias, therefore cannot remove from alias list\n", deadAlias);
		return;
	}

	int trackLastEmptyIndex = checkIfAlias;
	//shift all other entries down one index
	for(int i = checkIfAlias+1; i < MAX_ALIAS_SIZE; i++){
		strcpy(aliases[i-1].name, aliases[i].name);
		strcpy(aliases[i-1].command, aliases[i].command);
		trackLastEmptyIndex++;
	}

	strcpy(aliases[trackLastEmptyIndex].name, "");
	strcpy(aliases[trackLastEmptyIndex].command, ""); //empty this entry

	printf("Alias \"%s\" has been removed.\n", deadAlias);


}
/*
	Saves users aliases to file. Determines location of the file to save to by using
	the CWD from shell launch and the aliases filename defined in simpleshell.h. i.e
	CWD/filename
	Aliases are saved in the following format.
	<Alias>|<Command>
*/
void saveAliasesToFile(alias aliases[MAX_ALIAS_SIZE], char* dir){
	
	FILE *fp;
	char tempFileName [512];
	
	sprintf(tempFileName, "%s/%s", dir, aliasesFile); //constuct full path name needed to save file

	fp = fopen (tempFileName, "w");

	for (int i = 0; i < MAX_ALIAS_SIZE && strcmp(aliases[i].name, "\0"); i++)
	{
		char newLine[MAX_USERINPUT*2];
		sprintf(newLine, "%s|%s", aliases[i].name, aliases[i].command);
		fprintf(fp, newLine);
		fprintf(fp, "\n");
	}

	fclose (fp);
	
}
/*
	Loads user aliases from file. Uses CWD at shell launch and filename defined in header file
	to determine location of the file. i.e, CWD/filename
	Aliases in file must be stored in the following format.
	<Alias>|<Command> 
*/
void loadAliasesFromFile(alias aliases[MAX_ALIAS_SIZE], char* dir){
	FILE *fp;
	char tempFileName [512; 
	sprintf(tempFileName, "%s/%s", dir, aliasesFile); //construct full path name needed to load file

	fp = fopen(tempFileName, "r");
	
	char singleLine[MAX_USERINPUT*2];
	char tempTokens[MAX_SIZE][MAX_USERINPUT];
	int i = 0;

    if (fp == NULL) {
       //no file exists, therefore do nothing

		return;
    }
	else {
		while (fgets(singleLine, MAX_USERINPUT, fp) != NULL) {
			emptyArray(tempTokens, MAX_SIZE, MAX_USERINPUT);


			char* token = strtok(singleLine, "|"); // Tokenize input around '|' character
			int counter = 0;

			while (token != NULL)
			{
				chomp(token); // Removes any "new line" characters at begining of token
				strcpy(tempTokens[counter], token); 

				token = strtok(NULL, "|\n"); // Tokenize around '|' and remove rogue newline characters at end of tokens

				counter++;
			}
			strcpy(aliases[i].name, tempTokens[0]);
			strcpy(aliases[i].command, tempTokens[1]);
			i++;
		}
		fclose(fp);
	}

	
}

/*
	Part 9.
	Function that replaces tokens in an array of tokens if they are an alias defined in the aliases array.
	If the token is used as an alias, it is replaced with the corresponding command for that alias.
	Allows for the command associated with an alias to also be an alias.

*/
void findAndReplaceAliases(char tokens[MAX_SIZE][MAX_USERINPUT], alias aliases[MAX_ALIAS_SIZE]){

	//iterate through the tokens array
	for(int i = 0; i < tokensCount(tokens); i++){
		
		while(isAlias(tokens[i], aliases) >= 0){
			char command[MAX_USERINPUT];
			
			strcpy(command, "\0"); //protect against random data (seems to keep happening! ahh!) by assigning the data to null character
			strcpy(command, aliases[isAlias(tokens[i], aliases)].command); // get the command of the alias at this index
			
			char tempTokens[MAX_SIZE][MAX_USERINPUT]; //array of strings that will hold the tokenized command
			emptyArray(tempTokens, MAX_SIZE, MAX_USERINPUT);	
			
			char* token = strtok(command, " "); 
			int counter = 0;

			while (token != NULL)
			{
				strcpy(tempTokens[counter], token); 

				token = strtok(NULL, " "); // Tokenize and remove rogue newline characters at end of tokens

				counter++;
			}

			int IndexofEndOfTokens = tokensCount(tokens) - 1; //find the last used index of the tokens array
			
			for(int k = IndexofEndOfTokens; k > i; k--){
					strcpy(tokens[k+(counter-1)], tokens[k]); //shift all the tokens allow to make room for the new commands tokens

			}
			
			int temp = 0; // variable to increment as we iterate over the arrays
			for(int indexForAliasePlacement = i; indexForAliasePlacement <= i+(counter-1); indexForAliasePlacement++){
				strcpy(tokens[indexForAliasePlacement], tempTokens[temp++]); //place the command in the right place of the tokens array
			}
			
		}
	}
}


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
