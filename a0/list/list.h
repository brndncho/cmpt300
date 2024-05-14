#ifndef LIST_H
#define LIST_H

#define MAX_ITEM_SIZE 10

struct nodeStruct {
    char item[MAX_ITEM_SIZE]; // pointer to the string
    int item_size; // size of the string
    struct nodeStruct *next; // pointer to the next node
};

struct nodeStruct* List_createNode(const char *item);

void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node);

void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node);

int List_countNodes (struct nodeStruct *head);

struct nodeStruct* List_findNode(struct nodeStruct *head, const char *item);

void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node);

void List_sort (struct nodeStruct **headRef);

void List_free (struct nodeStruct **headRef);

#endif