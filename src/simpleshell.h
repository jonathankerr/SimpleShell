/* Boolean values */
#define TRUE 1
#define FALSE 0
typedef int bool;

/* Maximum number of characters user input can contain */
#define MAX_INPUT 512 

/* Maximum number of tokens shell will parse */
#define MAX_SIZE 50

/* String that will be printed every time 
   user is able to enter a command */
static const char prompt[] = "-> ";

/* Delimiters at which imput will be tokenized */
static const char delims[] = " \t|><&;";

/* Array of strings that will hold 50 strings 
   of 50 characters each */
static char tokens[MAX_SIZE][MAX_INPUT]; // TODO: Change to local variable

/* Function declarations */

void tokenize(char* input);
bool exitShell(char* input, bool shellStatus, char* dir);
//int launchChild(char*[] tokens);
void getInitDir(char* cwd);
