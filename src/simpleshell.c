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
			//strcpy(tokens[1], "undefined");
			//strcpy(tokens[2], "undefined");  //turn this into a function which sets all index to undefined which is called after every execution after input
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
	//printf("%s\n\n", &tokens[1]);

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
	else{
		printf("That functionality is incoming soon.");
	}

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

	//execv needs null terminated vector so we need
	// this to work around us using "empty" to mark
	//the end of the tokenized input
	char *testArgs[] = {"/bin/ls", "-l", NULL};

	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid == -1){
		perror("forked up!");

		_exit(1); //do we need exit in a method call or just a return?
	}
	if (c_pid == 0){
		//execv(tokens[0], tokens);
		execv(testArgs[0], testArgs);
		perror("Invalid command entry \n");
	}
	else if(c_pid > 0){

		if((pid = wait(&status)) < 0){
			perror("wait failed\n");
			_exit(1);//do we need exit in a method call or just a return?
		}
		printf("%d", pid);
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

	if(strcmp(tokens[1], "undefined")) //Valid input
	{ 
		printf("Set Path To: %s", tokens[1]);
		const char *path = tokens[1];
		setenv("PATH", path, 1);
	}
	else if(!strcmp(tokens[2], "undefined")) //If the third token is still undefined then they have not input anything after "getpath"
	{
		printf("Sorry that is not a valid path please use format: getpath <path>");
	}
	else if(strcmp(tokens[2], "undefined")) //Checks if they have input a string after the path. E.g. stops getpath <path> <another string here>
	{
		printf("Sorry that is not a valid path please use format: getpath <path>");
	}

}

/* Change Directory (cd) command: changes directory to given input */
int changeDirectory(char tokens[50][512])
{
	/*
	int success = 0;

	if (&tokens[1] == NULL)
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
		if (strstr(&tokens[1], ".") != NULL || strlen(&tokens[1]) < 3)
		{
			strcpy(nextDir, cwd);
		}
		else
		{
			nextDir = strcat(cwd, &tokens[1]);
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
	*/
	return 0;
}
/*
char *history[20] = {0}; //Empty History Array

void viewHistory(char *tokens[50][512], char *history[])
{
    int counter = 0;
    if(tokens[1] == 0)
    {
    	if(history[0] == 0)
    	{
            printf("History is empty\n");
	    }
    	else{
            
		    int index = 0;
		    while(history[index] != 0){
                index++;
                if(index == (max_hist)){
                    break;
                }
		    }
		    
		    // Prints history starting from 1
		    while(history[index] != 0){
                printf("%d. %s\n",(index),history[counter]);
                counter++;
                index--;
                if(counter == max_hist){
                    break;
                }
    		}
    	}
    }
    else{
        //History does not require parameters - prints message if it receieves any
        printf("History does require any parameters\n");
    }	
    return;
}
*/
#pragma endregion
