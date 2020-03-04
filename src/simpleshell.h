#pragma region Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <math.h>
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
   Maxum number of commands that will be stored in history.
*/
#define MAX_HISTORY_SIZE 20

/* 
   String that will be printed every time user is able to enter a command.
*/
static const char PROMPT[] = "-> ";

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
void printFullArray(char array[][MAX_USERINPUT], int maxSize);
void emptyArray(char** tokens, int maxSize, int maxUserInput);
void tokenize(char tokens[50][512], char* input);
void parseInput(char tokens[50][512], char history[MAX_HISTORY_SIZE][MAX_USERINPUT]);
bool exitShell(char* input, bool shellStatus, char* dir, char* path, char history[MAX_HISTORY_SIZE][MAX_USERINPUT]);
char* getCWD();
void addHistory(char *input, char history[MAX_HISTORY_SIZE][MAX_USERINPUT]);
void chomp(char *s);
#pragma endregion