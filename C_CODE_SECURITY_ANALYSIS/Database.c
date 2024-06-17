#define _CRT_SECURE_NO_WARNINGS
#include "Database.h"
/*
* This function creates the database table if it is not exists.
* input: db- the database (sqlite3*).
* output: the status of creation, success or failure (int).
*/
int createTable(sqlite3* db)
{
    char* errorMsg = 0;
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS unsafe_functions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "description TEXT NOT NULL,"
        "replacement TEXT NOT NULL"
        ");";

    int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errorMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
        return FAILURE;
    }

    return SUCCESS;
}



/*
* This function inserts an unsafe function into the database.
* input: db- the database (sqlite3*), name- name of function (cher*), description- what is the problem with the function (char*), replacement- suggested functions to replace (char*).
* output: the status of insertion, success or failure (int).
*/
int insertUnsafeFunction(sqlite3* db, const char* name, const char* description, const char* replacement) {
    sqlite3_stmt* stmt;
    const char* insertSQL = "INSERT INTO unsafe_functions (name, description, replacement) VALUES (?, ?, ?);";
    int rc;

    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return FAILURE;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, replacement, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return FAILURE;
    }

    sqlite3_finalize(stmt);

    return SUCCESS;
}

/*
* This function retrieves all the unsafe functions from the database and returns them as an array.
* input: db- the database (sqlite3*), count- pointer to set to point at the number of functions (int*).
* output: unsafeFunctions- the array of the unsafe functions..
*/

UnsafeFunction* getAllUnsafeFunctions(sqlite3* db, int* count)
{
    sqlite3_stmt* stmt;
    const char* querySQL = "SELECT name, description, replacement FROM unsafe_functions;";
    int rc;
    rc = sqlite3_prepare_v2(db, querySQL, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Count the number of rows
    int rowCount = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        rowCount++;
    }
    *count = rowCount;

    // Allocate memory for the array of unsafe functions
    UnsafeFunction* unsafeFunctions = (UnsafeFunction*)malloc(rowCount * sizeof(UnsafeFunction));
    if (!unsafeFunctions) 
    {
        fprintf(stderr, "Memory allocation error\n");
        sqlite3_finalize(stmt);
        return NULL;
    }

    // Reset the statement
    rc = sqlite3_reset(stmt);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        free(unsafeFunctions);
        sqlite3_finalize(stmt);
        return NULL;
    }

    //getting funcs
    for(int i=0; sqlite3_step(stmt) == SQLITE_ROW; i++) 
    {
        const char* name = (const char*)sqlite3_column_text(stmt, 0);
        const char* description = (const char*)sqlite3_column_text(stmt, 1);
        const char* replacement = (const char*)sqlite3_column_text(stmt, 2);

        unsafeFunctions[i].name = dinamicCopy(name);
        unsafeFunctions[i].description = dinamicCopy(description);
        unsafeFunctions[i].replacement = dinamicCopy(replacement);
    }

    sqlite3_finalize(stmt);

    return unsafeFunctions;
}

/*
* This function receives the c code file path and searches the code for unsafe functions to alert to the user.
* input: filePath- the c code's file path (char*).
* output: nothing.
*/
void checkForUnsafeFunctionsInC(char* filePath)
{
    char line[MAX_SIZE_LINE] = { 0 };
    int lineCounter = 0;
    FILE* fpRead = fopen(filePath, READ_FILE);
    if (fpRead == NULL)
    {
        printf("Error: could not open the file \"%s\".\n", filePath);
        return;
    }

    sqlite3* db;
    int rc;
    int count;
    // Open or create the SQLite database
    rc = sqlite3_open("unsafe_functions.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Create the table if it doesn't exist
    if (!createTable(db)) {
        sqlite3_close(db);
        return;
    }

    //get array of unsafe functions
    UnsafeFunction* functions = getAllUnsafeFunctions(db, &count);
    sqlite3_close(db);

    //search for unsafe functions in c file
    while (myFgets(line, sizeof(line), fpRead))
    {
        lineCounter++;
        if (isSkipableLine(line) == false)
        {
            checkLine(functions, count, line);
        }
    }
    cleanFunctionsArr(functions, count);
    fclose(fpRead);
}

