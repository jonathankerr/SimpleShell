#include <stdio.h> 
#include <string.h>
#include "simpleshell.h"

int main() 
{
    bool terminated = FALSE;
    
    while (!terminated)
    {
		char input[MAX_INPUT];

		printf("%s", prompt);

		fgets(input, 9999, stdin); // DO NOT REMOVE 9999
		tokenize(input);

		terminated = exitShell(input, (strcmp(input, "exit\n") == 0 || feof(stdin))); // Closes program if exit is typed or if Ctrl-D is pressed
    }
}