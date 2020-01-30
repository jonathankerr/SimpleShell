/* Boolean values */
#define true 1
#define false 0
typedef int bool;

/* Maximum number of characters user input can contain */
#define MAX_INPUT 512 

/* String that will be printed every time user is able to enter a command */
static const char prompt[] = "-> ";

/* Function declarations */
char *strtok(char *str, const char *delim);
