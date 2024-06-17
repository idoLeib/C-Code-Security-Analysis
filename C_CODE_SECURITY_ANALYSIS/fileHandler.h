#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "structre.h"

#define FILE_NAME "asm.s"
#define TEMP_FILE_NAME "temp.txt"
#define READ_AND_WRITE "r+"
#define WRITE_AND_READ "w+"
#define SKIP 1
#define DONT_SKIP 0
#define EMPTY 1
#define NOT_EMPTY 0
#define RESTORE 1
#define DO_NOT_RESTORE 0

void openAsm();
void openTemp();
int checkIfOpen();
void close();
int getNextAsmLine(char* line, int size, int skip);
void restoreLines();
#endif