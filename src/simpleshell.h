#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#pragma endregion

#pragma region Boolean values
#define TRUE 1
#define FALSE 0

/* 
   Value representing either TRUE (1) or FALSE (0).
*/
typedef int bool;
#pragma endregion

#pragma region Constants
/* 
   Maximum number of characters user input can contain. 
*/
#define MAX_USERINPUT 512 

/* 
   Maximum number of tokens shell will parse.
*/
#define MAX_SIZE 50


/* 
   Home directory. // (Hares: remove this constant)
*/
#define HOME_DIR getenv("HOME")

/* 
   String that will be printed every time user is able to enter a command.
*/
static const char prompt[] = "-> ";

/* 
   Delimiters at which imput will be tokenized.
*/
static const char DELIMS[] = " \t|><&;";

/* 
   List of internal functions.
*/
static const char* INTERNAL_FUNCTIONS[] = {"cd", "get", "getpath", "setpath", "history", "!!", "!", "!-", "alias-", "alias", "unalias"};
#pragma endregion

#pragma region Function declerations
void printTokens(char tokens[MAX_SIZE][MAX_USERINPUT]); // Part 1 related
void emptyArray(char tokens[MAX_SIZE][MAX_USERINPUT]);
void tokenize(char tokens[50][512], char* input);
int parseInput(char tokens[50][512]);
bool exitShell(char* input, bool shellStatus, char* dir);
char* getCWD();
void addHistory(char *input);
bool isInternalCmd(char* command);
void runExternalCmd(char tokens[50][512]);

#pragma endregion
