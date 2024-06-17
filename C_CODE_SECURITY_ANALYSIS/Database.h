#ifndef DATABASE_H
#define DATABASE_H

#include "unSafeFunctions.h"
#include "sqlite3.h"

#define READ_FILE "r"

int createTable(sqlite3* db);
int insertUnsafeFunction(sqlite3* db, const char* name, const char* description, const char* replacement);
UnsafeFunction* getAllUnsafeFunctions(sqlite3* db, int* count);
void checkForUnsafeFunctionsInC(char* filePath);

#endif