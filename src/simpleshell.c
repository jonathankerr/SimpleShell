#pragma region Libraries
#include "simpleshell.h"
#pragma endregion



void emptyArray(char* tokens)
{
	int counter = 0;
	while (counter < MAX_SIZE)
	{
		strcpy(&tokens[counter], "empty");
		counter++;
	}
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
			strcpy(tokens[1], "undefined");
			strcpy(tokens[2], "undefined");  //turn this into a function which sets all index to undefined which is called after every execution after input
//////////////////////////////////////////////////////


			chomp(token);
			strcpy(tokens[counter], token); // Adds token to array of tokens

			//printf("%s\n", token); // Uncomment to test (part 1)
			//printf("%s\n", tokens[counter]); // Uncomment to test (part 2)
			token = strtok(NULL, delims);

			counter++;
		}
	}

	//int counter = 0;
	//	while (counter < 10)
	//	{
	//		printf("%s\n", tokens[counter]);
	//		counter++;
	//	}
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

/*
// UNTESTED IN LINUX
int launchChild(char* tokens)
{ 
	
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) // Child process
	{
        if (execvp(tokens[0], &tokens) == -1) 
		{
            printf("ohh shit, execvp failed"); //testing
			perror("execvp called and failed");
			fflush(stdin);
        }

        exit(EXIT_FAILURE);
    } 
	else if (pid < 0) // Error forking
	{
		printf("we dun forked up"); //testing
		fflush(stdin);
        perror("we dun forked up");
		exit(1);
    } 
	else // Parent process
	{
		printf("parent started to wait");
		fflush(stdin);
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
		{
            wpid = waitpid(pid, & status, WUNTRACED);
        }
		printf("parent stopped waiting");
		fflush(stdin); //safety 
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
	printf("%s\n", getenv("PATH"));
	printf("\n");
}


void setPath(char tokens[50][512])
{
	//The getpath command will only accept and set a path if the command is in the following notation:
	// getpath <Path>

	if(strcmp(tokens[1], "undefined")) //Valid input
	{ 
		printf("Valid input");
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
