#include "fileHandler.h"
#pragma warning (disable : 4996)
FILE* tempFile = NULL;
FILE* asmFile = NULL;
int tempIsEmpty = EMPTY;
int restoreFromTemp = DO_NOT_RESTORE;

/*
* This function opens the asm file.
* input: nothing.
* output: nothing.
*/
void openAsm()
{
    asmFile = fopen(FILE_NAME, READ_AND_WRITE);
    if (!asmFile)
    {
        fprintf(stderr, "Error: file opening failed.\n\n");
        exit(1);
    }
}

/*
* This function opens the asm file.
* input: nothing.
* output: nothing.
*/
void openTemp()
{
    tempFile = fopen(TEMP_FILE_NAME, WRITE_AND_READ);//to create the file
    if (!tempFile)
    {
        fprintf(stderr, "Error: file opening failed.\n\n");
        exit(1);
    }
    fclose(tempFile);
    tempFile = fopen(TEMP_FILE_NAME, READ_AND_WRITE);
    if (!tempFile)
    {
        fprintf(stderr, "Error: file opening failed.\n\n");
        exit(1);
    }
}

/*
* This function checks if both temp file & asm file are opened.
  input: nothing.
  output: returns true if temp file & asm file are opened, and false if not (as int 0=false !0 = true).
*/
int checkIfOpen()
{
    return tempFile && asmFile;
}

/*
* This function closes the files.
* input:nothing.
* output:nothing.
*/
void close()
{
    fclose(tempFile);
    fclose(asmFile);
}

/*
* This function gets the next line from the asm, if the line will be needed again later it is saved in a temp file and restored using this function.
* input: line- pointer to set to the new line (char*), size- max size of line (int), skip- tells if to save the line for later (int).
* output: success status (int).
*/
int getNextAsmLine(char* line, int size, int skip)
{
    int ok = 0;
    if (!checkIfOpen())
    {
        openTemp();
        openAsm();
    }
    if (!restoreFromTemp)
    {
        //get lines from asm
        ok = fgets(line, size, asmFile);
    }
    else
    {
        //get the lines from the temp file
        ok = fgets(line, size, tempFile);
        if (!ok)//finished restoring from temp file, moving back to asm
        {
            tempIsEmpty = EMPTY;
            restoreFromTemp = DO_NOT_RESTORE;
            openTemp();
            ok = fgets(line, size, asmFile);
        }
    }
    if (!skip)
    {
        //saving line
        if (restoreFromTemp)
        {
            restoreFromTemp = DO_NOT_RESTORE;
            openTemp();
        }
        tempIsEmpty = NOT_EMPTY;
        fputs(line, tempFile);
    }
    return ok;
}

/*
* This function restores the saved lines.
* input: nothing.
* output: nothing.
*/
void restoreLines()
{
    if (!tempIsEmpty && !restoreFromTemp)
    {
        restoreFromTemp = RESTORE;
        rewind(tempFile);
    }
}