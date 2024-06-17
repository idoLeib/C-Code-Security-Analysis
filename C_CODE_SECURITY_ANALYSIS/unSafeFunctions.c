#define _CRT_SECURE_NO_WARNINGS
#include "unSafeFunctions.h"

/*
* This function alerts the user if a word is an unsafe function.
* input: functions- array of unsafe functions (UnsafeFunction*), count- number of unsafe functions (int), word- the word to check (char*)
* output: nothing.
*/
void checkForUnsafeFunction(UnsafeFunction* functions, int count, char* word)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(functions[i].name, word) == 0)
        {
            printf("Found security risk!\n");
            printf("We found a problematic function in your code.\n");
            printf("The name of the function is %s\n", functions[i].name);
            printf("The problem is %s\n", functions[i].description);
            printf("Recommended replacement: %s\n", functions[i].replacement);
            putchar('\n');
        }
    }
}

/*
* This function alerts the user if a line contains an unsafe function.
* input: functions- array of unsafe functions (UnsafeFunction*), count- number of unsafe functions (int), line- the line to check (char*)
* output: nothing.
*/
void checkLine(UnsafeFunction* functions, int count, char* line)
{
    char* token;
    char* tempLine = dinamicCopy(line);
    if (!tempLine)
    {
        fprintf(stderr, "Memory allocation error\n");
        return;
    }

    token = strtok(tempLine, C_WORD_SPLITTERS);
    for (; token != NULL; token = strtok(NULL, C_WORD_SPLITTERS))
    {
        checkForUnsafeFunction(functions, count, token);
    }


    free(tempLine);
}

/*
* This function frees the memory allocated for an unsafe function.
* input: unsafeFunction - the unsafe function to be freed (UnsafeFunction*).
* output: nothing.
*/
void cleanUnsafeDunction(UnsafeFunction* unsafeFunction)
{
    free(unsafeFunction->name);
    free(unsafeFunction->description);
    free(unsafeFunction->replacement);
}

/*
* This function frees the memory allocated for the array of unsafe functions.
* input: functions - the array of unsafe functions to be freed (UnsafeFunction*), size- the size of the array (int).
* output: nothing.
*/
void cleanFunctionsArr(UnsafeFunction* functions, int size)
{
    for (int i = 0; i < size; i++)
    {
        cleanUnsafeDunction(&(functions[i]));
    }
    free(functions);
}