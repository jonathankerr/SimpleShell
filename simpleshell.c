#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    bool terminated = false;
    char tokens[50];
    int index = 0;
    char prompt[] = ":D ";
    
    while (!terminated)
    {
        char input[MAX_INPUT]; 
        printf("%s", prompt); 
        fgets(input, MAX_INPUT, stdin); 
        
        char *temp = strtok(input, " \t><&;");
        strcpy(tokens[index], temp);
        index++;
        
        while (input) {
            temp = strtok(NULL, " \t><&;");
            strcpy(tokens[index], temp);
            index++;
        }

        if (strcmp(tokens[0], "exit\n") || feof(stdin))
        {
            terminated = true;
        }
    }
    return 0;
}