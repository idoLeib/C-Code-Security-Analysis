#include "AsmCheck.h"

/*
* This program receives a path to a c code from the user and prints a security analysis report of the code.
*/
int main()
{
    printf("HELLO!\n");
	printf("Welcome to our security analysis.\n");
    printf("Enter the path to your c code:\n");

    char path[MAX_PATH_LEN];
    if (!myFgets(path, MAX_PATH_LEN - 1, stdin))
    {
        exit(1);
    }

    //checking the code
    checkAsm(path);
    checkForUnsafeFunctionsInC(path);

}

