#ifndef LIST_H
#define LIST_H

struct nodeStruct {
    void* block;
    struct nodeStruct *next; // pointer to the next node
};

struct nodeStruct* List_createNode(void* block);

void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node);

void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node);

int List_countNodes (struct nodeStruct *head);

struct nodeStruct* List_findNode(struct nodeStruct *head, void* block);

void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node);

//void List_sort (struct nodeStruct **headRef);

//void List_free (struct nodeStruct **headRef);

#endif