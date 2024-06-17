#include "structre.h"

/*
* This function creates a new empty list.
* input: nothing.
* output: newList- pointer to the newly created list (List*).
*/
List* createList()
{
    List* newList = (List*)malloc(sizeof(List));
    if (!newList)
    {
        printf("Error. memory allocation failed\n");
        exit(1);
    }
    newList->head = NULL;
    return newList;
}


/*
* This function creates a new node with the given name and line number.
* input: name - the name to be assigned to the new node (char*), lineNum - the line number to be assigned to the new node (int).
* output: newNode- pointer to the newly created node (Node*).
*/
Node* createNode(char* name, int lineNum)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode)
    {
        printf("Error. memory allocation failed\n");
        exit(1);
    }
    newNode->name = dinamicCopy(name);
    newNode->next = NULL;
    newNode->lineNum = lineNum;
    return newNode;
}

/*
* This function adds a new node to the list if it does not already exist.
* input: name - the name to be assigned to the new node (char*), lineNum - the line number to be assigned to the new node (int), list - the list to which the new node should be added (List*).
* output: nothing.
*/
void add(char* name, int lineNum, List* list)
{
    Node* current = list->head;
    Node* newNode = createNode(name, lineNum);
    if(current == NULL)
    {
        list->head = newNode;
    }
    else
    {
        while (current != NULL)
        {
            if (!strcmp(current->name, name))
            {
                return;
            }
            if (current->next == NULL)
            {
                current->next = newNode;
            }
            current = current->next;
        }
    }

}


/*
* This function frees the memory allocated for a node.
* input: node - the node to be freed (Node*).
* output: nothing.
*/
void freeNode(Node* node)
{
    free(node->name);
    free(node);
}

/*
* This function frees the memory allocated for the list and its nodes.
* input: list - the list to be freed (List*).
* output: nothing.
*/
void freeList(List* list)
{
    Node* current = list->head;
    Node* next = NULL;
    if (current == NULL)
    {
        free(list);
        return;
    }
    while (current != NULL)
    {
        next = current->next;
        freeNode(current);
        current = next;
    }
    free(list);

}

/*
* This function deletes a node with the given name from the list.
* input: token - the name of the node to be deleted (char*), list - the list from which the node should be deleted (List*).
* output: nothing.
*/
void deleteNode(char* token, List* list)
{
    Node* current = list->head;
    Node* prev = current;
    if (current == NULL)
    {
        return;
    }
    if (strcmp(current->name, token) == 0)
    {
        list->head = current->next;
        freeNode(current);        
        return;
    }
    int flag = RUN;
    for (; flag && current != NULL; prev = current, current = current->next)
    {
        if (strcmp(current->name, token) == 0)
        {
            prev->next = current->next;
            freeNode(current);
            flag = STOP;
        }
    }
}