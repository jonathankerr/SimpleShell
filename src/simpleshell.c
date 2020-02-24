#pragma region Libraries
#include "simpleshell.h"
#pragma endregion

//for Debugging
void printFullArray(char tokens[MAX_SIZE][MAX_USERINPUT]){
	
	
	printf("---TESTING 2D ARRAY CONTENTS ---\n");
	for(int i = 0; i < MAX_SIZE; i++){
		printf("%i : %s  size = %li \n",i, tokens[i], strlen(tokens[i]));
	}


}
// prints each token on the terminal, one on each line
void printTokens(char tokens[MAX_SIZE][MAX_USERINPUT]){

	//char **temp = tokens;
	int count = 0;

	while(strcmp(tokens[count], "\0")){
		printf("\n %s ", tokens[count++]);
	}
}

void emptyArray(char tokens[MAX_SIZE][MAX_USERINPUT])
{

	memset(tokens,'\0', sizeof(tokens[0][0]) * MAX_SIZE * MAX_USERINPUT); //instad of "empty"

}

void tokenize(char tokens[50][512], char* input)
{
	
	if (strlen(input) > MAX_USERINPUT)
	{
		printf("\nInvalid input - please enter no more than 512 characters.\n\n");
		fflush(stdin); // Flush in case there is chars in buffer
	}
	else
	{
		char* token = strtok(input, delims);
		int counter = 0;


		while (token != NULL)
		{
			///////////////////////////////////////////
			//strcpy(tokens[1], "\0");
			strcpy(tokens[2], "\0");  //turn this into a function which sets all index to undefined which is called after every execution after input
//////////////////////////////////////////////////////


			chomp(token);
			strcpy(tokens[counter], token); // Adds token to array of tokens

			//printf("%s\n", token); // Uncomment to test (part 1)
			//printf("%s\n", tokens[counter]); // Uncomment to test (part 2)
			token = strtok(NULL, delims);

			counter++;
		}
	}
}

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
	else if(!strcmp(tokens[0], "!")){
		printf("I got ya");
	}
	//else{
	//	printf("That functionality is incoming soon.");
	//}

	return success;
}

bool exitShell(char* input, bool shellStatus, char* dir)
{

	printf("\n");

	if (shellStatus)
	{
		char output[25];

		// Determines what message to print
		if (!strcmp(input, "exit"))
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

bool isInternalCmd(char* command){

	for(int i = 0; i < numOfFunctions; i++){
		
		if(strcmp(internalFunc[i], command) == 0) {

			return TRUE;
		}
	}
	//printf("test");
	return FALSE;
}

int tokensCount(char tokens[50][512]){
	int count = 0;

	for(int i = 0; i < MAX_SIZE; i++){
		if(strlen(tokens[i]) != 0){
			count++;
		}
	}
	return count;
}
void runExternalCmd(char tokens[50][512]){


	char *testArgs[] = {"/bin/ls", "-l", NULL};

	pid_t c_pid, pid;
	int status;
	char* tempArgs[51]; //possible work around allowing us to keep 2D array

	int numOfTokens = tokensCount(tokens);

	for(int i = 0; i < numOfTokens; i++){
		//if(strcmp(tokens[i], "undefined"))
		//{
		//	tempArgs[i] = "/0";
		//}
		//else 
		//{
			tempArgs[i] = tokens[i];
		//}
	}
	tempArgs[numOfTokens] = NULL;

	c_pid = fork();

	if(c_pid == -1){
		perror("forked up!");

		_exit(1);
	}
	if (c_pid == 0){
		//execv(tokens[0], tokens);
		//execv(testArgs[0], testArgs);
		execvp(tempArgs[0], tempArgs);
		perror("Invalid command entry \n");
		_exit(1); //makes sure it exits
	}
	else if(c_pid > 0){

		if((pid = wait(&status)) < 0){
			perror("wait failed\n");
			_exit(1);
		}
		//printf("%d", pid);
    }
}

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

char* getInitDir()
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

/* 
	Removes new line character (\n) from string
*/
void chomp(char *s) 
{
    while(*s && *s != '\n' && *s != '\r') s++;
 
    *s = 0;
}

#pragma region Command definitions

void getPath()
{
	printf("Current Path Is: %s", getenv("PATH"));
	printf("\n");
}


void setPath(char tokens[50][512])
{
	//The getpath command will only accept and set a path if the command is in the following notation:
	// getpath <Path>
	
	if(!strcmp(tokens[1], "\0")) //if the second token is empty
	{
		printf("You have not entered a path, please use format: getpath <path>");
	}
	else if(strcmp(tokens[2], "\0")) //if the third token has a string in it show error
	{
		printf("Sorry that is not a valid path please use format: getpath <path>");
	}
	else if(strcmp(tokens[3], "\0")) //if the fourth token has a string in it show error
	{
		printf("Sorry that is not a valid path please use format: getpath <path>");
	}
	else
	{
		printf("Set Path To: %s", tokens[1]);
		const char *path = tokens[1];
		setenv("PATH", path, 1);
	}


}

/* Change Directory (cd) command: changes directory to given input */
int changeDirectory(char tokens[50][512])
{
	
	int success = 0;

	if (tokens[1] == NULL)
	{
		printf("Please enter a directory using syntax: cd [directory name].\n\n");
	}
	else
	{
		printf("true\n");
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
		
		//if ()
		//{
			int success = chdir(nextDir);
		//}

		//printf("%s\n", getCWD()); // Uncomment to test (part 4)

		if (success == -1)
		{
			perror("Error");
			return success;
		}
		else
		{
			printf("success\n");
		}
	}
	
	return 0;
}


char history[20][512] = {"\0"};  //Empty history array
void addHistory(char *input){     //adding commands to history

	char *rawInput = input;

	if(!strcmp(history[0], "\0"))  //if this is the first command
	{
		strcpy(history[0], rawInput);	
	}
	else if(strcmp(history[0], "\0")) //if previous commands have been made
	{
		for(int i=20-1;i>0;i--)
		{
			strcpy(history[i], history[i-1]);
		}

		strcpy(history[0], rawInput);

	}
}

void viewHistory()
{
	printf("\nFull History Array: \n");
	for(int i = 0; i < 20; i++) //loops and shows fully history array
	{
		printf("%d: ", i+1);
		printf("%s\n", history[i]);
	}
}






#pragma endregion
