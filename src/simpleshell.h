#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#pragma endregion

/* Boolean values */
#define TRUE 1
#define FALSE 0
typedef int bool;

/* Maximum number of characters user input 
   can contain */
#define MAX_USERINPUT 512 

/* Maximum number of tokens shell will parse */
#define MAX_SIZE 50

/* Path on start of application, saved for exit */
//#define START_PATH getenv("PATH")

/* Home Directory */
#define HOME_DIR getenv("HOME")

//#define ELhttp://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.htmlEMS(x)  (sizeof(x) / sizeof((x)[0]))

/* String that will be printed every time 
   user is able to enter a command */
static const char prompt[] = "-> ";

/* Delimiters at which imput will be tokenized */
static const char delims[] = " \t|><&;";

/* Function declarations */

void tokenize(char* tokenAd, char* input);
int parseInput(char* tokens);
bool exitShell(char* input, bool shellStatus, char* dir);
int launchChild(char* tokens);
char* getCWD();
char* getInitDir();
void setToHomeDir();
char getPath();
void setPath(char** tokens);
void chomp(char *s);

/* Command definitions */

int changeDirectory(char* tokens);