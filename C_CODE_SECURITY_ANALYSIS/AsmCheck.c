#define _CRT_SECURE_NO_WARNINGS
#include "AsmCheck.h"

/*
* This function calls to the asm check.
* input: filePath- the path to the c file to check it's asm (char*).
* output: nothing.
*/
void checkAsm(char* filePath)
{
    if (createAsmFile(filePath))
    {
        goThroughAsm();
    }
}

/*
* This function creates the assembly files and returns the status of the creation- success or failure.
* input: nameOfFile- the name of the c file to create the asm file for(char*).
* output: true if succeeded in creating, and false if not.
*/
bool createAsmFile(char* nameOfFile)
{
    //getting copy command
    char copyCToDockerCommand[COMMAND_SIZE];
    snprintf(copyCToDockerCommand, sizeof(copyCToDockerCommand), "%s %s %s", COPY_FROM_DOCKER_COMMAND, nameOfFile, DOCKER_ASM_FILE_NAME);

    int status = system(copyCToDockerCommand);//copying c file into docker
    system(RUN_DOCKER_FOR_ASM_COMMAND);//compiling c file to asm
    system(GET_ASM_FROM_DOCKER_COMMAND);//getting the asm file from the docker

    bool success = status == 0;
    if (success)
    {
        printf("The code can compile successfully.\n\n");
    }
    else
    {
        printf("Error: The code cannot be compiled.\n\n");
    }
    return success;
}

/*
* This function goes through the asm file and looks for coding errors.
* input: nothing.
* output: nothing.
*/
void goThroughAsm()
{
    char line[MAX_SIZE] = { 0 };
    char* word = NULL;
    char* tempLine = NULL;
    int counter = 0;
    bool flag = true;
    List* listOfFiles = createList();//list of file pointers
    List* listOfDinamics = createList();//kist of dinamic allocation pointers

    while( getNextAsmLine(line, sizeof(line), SKIP))
    {
        line[strcspn(line, "\n")] = 0;
        counter++;
        if (counter > NUM_OF_IRRELEVANT_LINES)//skip start of file- not relevant
        {
            //going through the line checking words
            tempLine = dinamicCopy(line);
            flag = true;
            for (word = strtok(tempLine, ASM_WORD_SPLITTERS); flag && word != NULL; word = strtok(NULL, ASM_WORD_SPLITTERS))
            {

                if (strcmp(FOPEN_KEY_WORD, word) == 0)
                {
                    checkFopen(listOfFiles);
                    flag = false;

                }
                if (strcmp(STRCHR_KEY_WORD, word) == 0)
                {
                    checkForUserValidCheck();
                    flag = false;

                }

                if (line[0] == ASM_COMMENT[0] && (strcmp(FCLOSE_KEY_WORD, word) == 0))
                {
                    removeFromList(listOfFiles);
                    flag = false;
                }
                if (strcmp(MALLOC_KEY_WORD, word) == 0)
                {
                    checkMalloc(line, listOfDinamics);
                    flag = false;
                   

                }
                if (strcmp(FREE_KEY_WORD, word) == 0)
                {
                    removeFromList(listOfDinamics);
                    flag = false;
                }
                restoreLines();
            }
            free(tempLine);
        }

    }
    
    printUnFreedAllocationsMsg(listOfDinamics);
    printUnclosedFilesMsg(listOfFiles);
    freeList(listOfFiles);
    freeList(listOfDinamics);
    close();
}

/*
* This function checks if an fopen was handled correctly. 
* input: listOfFiles- list of opened files (List*).
* output: nothing.
*/
void checkFopen(List* listOfFiles)
{
    char line[MAX_SIZE] = { 0 };
    char compare[MAX_SIZE];
    char* word = NULL;
    char* pointer = NULL;
    int lineNum = 0;
    char* expression = NULL;
    char* call = NULL;

    //getting the file pointer
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    pointer = dinamicCopy(word);

    //getting the number of the line
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_LINE_NUM_SPLITTERS);
    word = strtok(NULL, ASM_LINE_NUM_SPLITTERS);
    lineNum = atoi(word);

    //adding file pointer to the list of open files
    add(pointer, lineNum - 1, listOfFiles);

    // checking if the user makes sure the file opening was successful
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    expression = strtok(line, ASM_COMMENT);
    snprintf(compare, sizeof(compare), ASM_COMPARE_FORMATTED_POINTER_TO_ZERO_LINE, pointer);
    if (strcmp(compare, expression) != 0)
    {
        printf("Note: In line %d. you open a file. We strongly recommend you to add a valid check in line %d.\n\n", lineNum - 1, lineNum);
    }
    free(pointer);
}

/*
* This function checks if an malloc was handled correctly.
* input: listOfDinamics- list of allocated pointers(List*).
* output: nothing.
*/
void checkMalloc(char* mallocLine, List* listOfDinamics)
{
    char line[MAX_SIZE] = { 0 };
    char compare[MAX_SIZE];
    char* word = NULL;
    char* pointer = NULL;
    int lineNum = 0;
    char* expression = NULL;
    char* call = NULL;

    //getting the number of the line
    strcpy(line, mallocLine);
    word = strtok(line, ASM_LINE_NUM_SPLITTERS);
    word = strtok(NULL, ASM_LINE_NUM_SPLITTERS);
    lineNum = atoi(word);

    //checking if this is the actual malloc call (it is possible that the malloc call will be referred in the asm multiple times)
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    call = strtok(line, ASM_EXPRESSION_SPLITTERS);
    call = dinamicCopy(strtok(NULL, ASM_EXPRESSION_SPLITTERS));
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    if (strcmp(call, ASM_MALLOC_CALLING) == 0)
    {
        //adding pointer to the list of memory allocation pointers
        pointer = dinamicCopy(word);
        add(pointer, lineNum, listOfDinamics);

        // checking if the user makes sure the memory allocation was successful
        getNextAsmLine(line, sizeof(line), DONT_SKIP);
        getNextAsmLine(line, sizeof(line), DONT_SKIP);
        expression = strtok(line, ASM_COMMENT);
        snprintf(compare, sizeof(compare), ASM_COMPARE_FORMATTED_POINTER_TO_ZERO_LINE, pointer);
        if (strcmp(compare, expression) != 0)
        {
            printf("Note: In line %d. you used malloc. We strongly recommend you to add a valid check in line %d to make sure the return value wasn't null.\n\n", lineNum, lineNum + 1);
        }
    }
    free(call);
    free(pointer);
}

/*
* This function chandles free and fclose and remove them from the listy.
* input: list- list of allocated pointers or files(List*).
* output: nothing.
*/
void removeFromList(List* list)
{
    char line[MAX_SIZE] = { 0 };
    char compare[MAX_SIZE];
    char* word = NULL;
    char* pointer = NULL;
    int lineNum = 0;
    char* expression = NULL;

    //deleting pointer from list of used pointers
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_EXPRESSION_SPLITTERS);
    pointer = dinamicCopy(strtok(NULL, ASM_EXPRESSION_SPLITTERS));
    deleteNode(pointer, list);

    //getting the number of the line
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_LINE_NUM_SPLITTERS);
    word = strtok(NULL, ASM_LINE_NUM_SPLITTERS);
    lineNum = atoi(word);

    //checking for dangling pointers
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    expression = strtok(line, ASM_COMMENT);
    snprintf(compare, sizeof(compare), ASM_set_FORMATTED_POINTER_TO_ZERO_LINE, pointer);
    if (strcmp(compare, expression) != 0)
    {
        printf("Note: In line %d. you stopped using a pointer. We strongly recommend you set the unused pointer to null in line %d to avoid dangling pointers.\n\n", lineNum - 1, lineNum);
    }
    free(pointer);
}



/*
* This function checks if a function's return value was handled correctly.
* input: nothing.
* output: nothing.
*/
void checkForUserValidCheck()
{
    char line[MAX_SIZE] = {0};
    char compare[MAX_SIZE];
    char* word = NULL;
    char* pointer = NULL;
    int lineNum = 0;
    char* expression = NULL;

    //getting pointer
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    word = strtok(NULL, ASM_EXPRESSION_SPLITTERS);
    pointer = dinamicCopy(word);

    //gettimg the number of the line
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    word = strtok(line, ASM_LINE_NUM_SPLITTERS);
    word = strtok(NULL, ASM_LINE_NUM_SPLITTERS);
    lineNum = atoi(word);

    // checking if the user makes sure the function call was successful
    getNextAsmLine(line, sizeof(line), DONT_SKIP);
    expression = strtok(line, ASM_COMMENT);
    snprintf(compare, sizeof(compare), ASM_COMPARE_FORMATTED_POINTER_TO_ZERO_LINE, pointer);
    if (strcmp(compare, expression) != 0)
    {
        printf("Note: In line %d. you used a function with return value. We strongly recommend you to add a check in line %d  make sure the result wasn't null.\n\n", lineNum - 1, lineNum);
    }
    free(pointer);
}

/*
* This function prints a message with all of the uncloed files to the user.
* input: listOfFiles- list of opened files (List*).
* output: nothing.
*/
void printUnclosedFilesMsg(List* listOfFiles)
{
    if (!listOfFiles || !listOfFiles->head)
    {
        return;
    }
    printf("Note: It is possible that not all of the files were closed. We strongly recommend you to check it:\n");
    for (Node* temp = listOfFiles->head; temp != NULL; temp = temp->next)
    {
        printf("opened in line- %d\n", temp->lineNum);
    }
    putchar('\n');
}

/*
* This function prints a message with all of the unfreed dinamid pointers to the user.
* input: listOfDinamics- list of allocated pointers(List*).
* output: nothing.
*/
void printUnFreedAllocationsMsg(List* listOfDinamics)
{
    if (!listOfDinamics || !listOfDinamics->head)
    {
        return;
    }
    printf("Note: It is possible that not all of the dynamical allocations were freed. We strongly recommend you to check it:\n");
    for (Node* temp = listOfDinamics->head; temp != NULL; temp = temp->next)
    {
        printf("allocated in line- %d\n", temp->lineNum);

    }
    putchar('\n');
}