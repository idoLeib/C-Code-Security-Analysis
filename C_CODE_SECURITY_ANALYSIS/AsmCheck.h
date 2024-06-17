#ifndef ASM_CHECK_H
#define ASM_CHECK_H

#include "fileHandler.h"

#define COMMAND_SIZE 300
#define MAX_SIZE_LINE_ASM 200
#define MAX_SIZE (MAX_SIZE_LINE_ASM + 1)
#define RUN_DOCKER_FOR_ASM_COMMAND "docker exec -it pspp ./compile_file.sh asm.c"
#define GET_ASM_FROM_DOCKER_COMMAND "docker cp pspp:/asm.s asm.s"
#define DOCKER_ASM_FILE_NAME "pspp:/asm.c"
#define COPY_FROM_DOCKER_COMMAND "docker cp"
//#define DELETE_DOCKER_ASM_FILE_COMMAND "docker exec -it pspp rm asm.s"

#define FOPEN_KEY_WORD "fopen@PLT"
#define FCLOSE_KEY_WORD "fclose"
#define STRCHR_KEY_WORD "strchr@PLT"
#define MALLOC_KEY_WORD "malloc"
#define FREE_KEY_WORD "free"
#define ASM_MALLOC_CALLING "malloc@PLT"
#define ASM_WORD_SPLITTERS " \t,.();"
#define ASM_EXPRESSION_SPLITTERS " \t,"
#define ASM_LINE_NUM_SPLITTERS ":"
#define ASM_COMMENT "#"

#define ASM_COMPARE_FORMATTED_POINTER_TO_ZERO_LINE "	cmpq	$0, %s	"
#define ASM_set_FORMATTED_POINTER_TO_ZERO_LINE	"	movq	$0, %s	"

void checkAsm(char* filePath);
bool createAsmFile(char* nameOfFile);
void goThroughAsm();
void checkFopen(List* listOfFiles);
void checkMalloc(char* mallocLine, List* listOfDinamics);
void removeFromList(List* list);
void checkForUserValidCheck();
void printUnclosedFilesMsg(List* listOfFiles);
void printUnFreedAllocationsMsg(List* listOfDinamics);

#endif