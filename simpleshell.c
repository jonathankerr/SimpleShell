#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "simpleshell.h"

int main() 
{ 
    char* delims[] = {" ","\t",">","<","&",";"}; 
    
    bool terminated = false;
    char tokens[50];
    int index = 0;
    char prompt[] = ":D ";
    
    while (!terminated)
    {
        char input[MAX_INPUT]; 
        printf("%s", prompt); 
        fgets(input, MAX_INPUT, stdin); 
        
        char *temp = strktok(input, delims);
        strcpy(tokens[index], temp);
        index++;
        
        do{
            temp = strktok(NULL, delims);
            strcpy(tokens[index],strtok(NULL, delims));
            index++;
        }
        while (input); 
        
 
    }

        
    if (strcmp(tokens[0], "exit\n") || feof(stdin))
    {
        terminated = true;   
    }
    
    return 0;
}
