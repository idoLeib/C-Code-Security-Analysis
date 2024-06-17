#ifndef UN_SAFE_FUNCTIONS_H
#define UN_SAFE_FUNCTIONS_H

#include "Helper.h"

typedef struct UnsafeFunction {
    char* name;
    char* description;
    char* replacement;
}UnsafeFunction;

#define C_WORD_SPLITTERS " \t\n,.();"
void checkForUnsafeFunction(UnsafeFunction* functions, int count, char* word);
void checkLine(UnsafeFunction* functions, int count, char* line);
void cleanFunctionsArr(UnsafeFunction* functions, int size);
void cleanUnsafeDunction(UnsafeFunction* unsafeFunction);


#endif