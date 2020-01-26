#include <stdio.h> 
#include <string.h>

/* boolean values */
#define true 0
#define false 1
typedef int bool;

#define MAX_INPUT 512
static const char prompt[] = "> ";

int main() 
{ 
    bool terminated = false;
    
    while (!terminated)
    {
        char input[MAX_INPUT]; 
        printf(prompt); 
        fgets(input, MAX_INPUT, stdin); 
        
        char* token = strtok(input, " ");
        while (token) 
        {
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }
    }
        
    if (strcmp(input, "exit\n") || feof(stdin))
    {
        terminated = true;   
    }
    
    return 0;
}
