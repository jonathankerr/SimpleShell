#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    bool terminated = false;

    
    while (terminated == false)
    {

	printf("%s", prompt);

	char input[MAX_INPUT];


	fgets(input, 9999, stdin); // DONT REMOVE 9999




	
	char *token = strtok(input, " \t|><&;");
	
	while(token != NULL){				//Uncomment printf to test
		//printf("%s\n",token);
		token = strtok(NULL, " \t|><&;");	
	}





	if(strlen(input) > MAX_INPUT) {
		printf("Invalid Input too long (no more than 512 characters)\n");
	}
	

	if(strcmp(input, "exit\n") == 0){
		printf("-> Closing Program\n");	 // Closes program if exit is typed
		terminated = true;
	}
	
	if(feof(stdin)){
		printf("Closing Program\n");	// Closes program if Ctrl-D is pressed
		terminated = true;
	}

    }
}
