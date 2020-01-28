#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    bool terminated = false;
    char tokens[512][512];
    
    while (terminated == false)
    {
        int index = 0;
        
        char input[MAX_INPUT]; 
        printf("%s", prompt); 
        fgets(input, MAX_INPUT, stdin); 
        
        char *temp = strtok(input, " \t><&;");
        strcpy(tokens[index], temp);
        index++;
        
        while (input != "\n") {
            temp = strtok(NULL, " \t><&;");
            strcpy(tokens[index], temp);
            index++;
            
            if (strcmp(tokens[0], "exit\n") == 0 || feof(stdin))
            {
              terminated = true;
            }         
        }
        printf("\n");
    }
    return 0;
}
