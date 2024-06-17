#ifndef STRUCTRE_H
#define STRUCTRE_H
#include "Database.h"

typedef struct Node {
    char* name;
    int lineNum;
    struct Node* next;
}Node;

typedef struct List {
    Node* head;
}List;

#define RUN 1
#define STOP 0

List* createList();
Node* createNode(char* name, int lineNum);
void add(char* name, int lineNum, List* list);
void freeNode(Node* node);
void freeList(List* list);

void deleteNode(char* token, List* list);
#endif