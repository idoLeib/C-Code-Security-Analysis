#define _CRT_SECURE_NO_WARNINGS
#include "Helper.h"

/*
* This function returns a dinamic copy of the string.
* input: str- string to copy (const char*).
* output: new_str- the new copy of the string (char*).
*/
char* dinamicCopy(const char* str) {
    if (str == NULL) {
        return NULL; // Handle NULL input string
    }

    size_t len = strlen(str);
    char* new_str = (char*)malloc((len + 1) * sizeof(char));

    if (new_str) {
        strcpy(new_str, str);
    }

    return new_str;
}

/*
* This function gets input from the user.
* input: string- pointer to the string to set to the input (char[]), len- the max length (int), input- file to get the input from (FILE*).
* output: status- success or failure (int).
*/
int myFgets(char string[], int len, FILE* input)
{
    int status = fgets(string, len, input);
    int lastIndex = 0;
    lastIndex = strcspn(string, "\n");
    if (lastIndex == len)
    {
        printf("Error: invalid length\n");
        return FAILURE;
    }
    string[lastIndex] = '\0';
    return status && SUCCESS;
}

/*
* this function checks if a line is a comment so it can be skipped.
* input: line- line to check if is a comment and can be skipped (char*).
* output: true if can bes skipped and false if not.
*/
bool isSkipableLine(char* line)
{
    int i;
    char tempLine[MAX_SIZE_LINE] = { 0 };
    char* token = NULL;
    strcpy(tempLine, line);
    for (i = 0; isspace(tempLine[i]) != 0; i++);
    if (tempLine[i + 1] == '\0')
    {
        return true;
    }
    token = strtok(tempLine, WHITE_CHARS);
    if (strlen(token) >= COMMENT_LEN && token[0] == '/' && token[1] == '/')
        return true;
    return false;
}