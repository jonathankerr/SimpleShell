#pragma region Libraries
#include "simpleshell.h"
#include <sys/wait.h>
#pragma endregion

void emptyArray(char* tokens)
{
	int counter = 0;
	while (counter < MAX_SIZE)
	{
		strcpy(&tokens[counter], "\0");
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
			chomp(token);
			strcpy(tokens[counter], token); // Adds token to array of tokens

			//printf("%s\n", token); // Uncomment to test (part 1)
			//printf("%s\n", tokens[counter]); // Uncomment to test (part 2)
			
			token = strtok(NULL, delims);

			counter++;
		}
		//token = NULL;
		//strcpy(tokens[counter], token);
		
		
	}
<<<<<<< HEAD
	//testing
	int counter = 0;
		while (counter < 10)
		{
			for(int abc = 0; abc < 10; abc++){
				printf("%x", tokens[counter][abc]);
			}
			printf("\n");
			counter++;
		}
	fflush(stdout);
=======
	//int counter = 0;
	//	while (counter < 10)
	//	{
	//		printf("%s\n", tokens[counter]);
	//		counter++;
	//	}
>>>>>>> 6821cd42253b2fd886247d9ad47b9e99ccb6185f

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
// UNTESTED IN LINUX SHAME
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

void runCommand(char tokens[50][512]){
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if(c_pid == -1){
		perror("forked up");
		_exit(1);
	}

	if(c_pid == 0){
		printf("Child started"); //testing
		
		int check = execv(tokens[0], tokens);
		perror("ERROR: ");
		if(check == -1){ //checks for failed execv (-1)
			_exit(1);
		}
		
		
	}
	else if (c_pid > 0){

		if((pid = wait(&status)) < 0){
			perror("wait failed");
			_exit(1);
		}
		
		printf("parent exited");
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
	printf("%s\n", getenv("PATH"));
	printf("\n");
}


void setPath(char tokens[50][512])
{

	printf("%s\n\n", tokens[1]);

	const char *path = tokens[1];

	if(&tokens[1] == NULL){
		printf("We will now input");
	}

	setenv("PATH", path, 1);
}


/*
{
    if (&tokens[1] !=NULL)
	{
        if (&tokens[2] == NULL)
		{
            printf("Set path to: %sn", &tokens[1]);

            if (setenv("PATH", &tokens[1], 1 == 0))
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
}
*/

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

#pragma endregion