#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0

#define MAX_PATH_LEN 300
#define MAX_SIZE_LINE 300
#define WHITE_CHARS " \t\r\v\n\f\0"
#define NUM_OF_IRRELEVANT_LINES 6
#define COMMENT_LEN 2

char* dinamicCopy(const char* str);
int myFgets(char string[], int len, FILE* input);
bool isSkipableLine(char* line);

#endif