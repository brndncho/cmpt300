#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

// allocate memory for node and initialize it with item.
struct nodeStruct* List_createNode(void* block) {
    struct nodeStruct* newNode = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));

    if (newNode != NULL) {
        newNode->block = block;
        //printf("%d\n", newNode->item_size);
        newNode->next = NULL;
    }
    return newNode;
}


// insert node at the head of the list
void List_insertHead(struct nodeStruct **headRef, struct nodeStruct *node) {
    if (node != NULL) {
        node->next = *headRef;
        *headRef = node;
    }
}

// insert node after the tail of the list
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node) {
    if (node != NULL) {
        // if there is nothing before the tail, the head is the tail
        if (*headRef == NULL) {
            *headRef = node;
        }
        else {
            struct nodeStruct* cur = *headRef;
            while(cur->next != NULL) {
                cur = cur->next;
            }
            
            cur->next = node; // reached end of list, append new node
        }
    }    
}

// count number of nodes in the list
int List_countNodes (struct nodeStruct *head) {
    int count = 0;
    struct nodeStruct* cur = head;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }

    return count;
}


// return first node holding the item, return null if none found
struct nodeStruct* List_findNode(struct nodeStruct *head, void* block) {
    if (head != NULL) {
        struct nodeStruct* cur = head;
        while (cur != NULL) {
            if (cur-> block == block) {
                return cur;
            }
            cur = cur->next;
        }
    }
    return NULL;
}

// delete node from list and free memory.
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node) {
    struct nodeStruct* cur = *headRef;
    struct nodeStruct* nxt = cur->next;
    int count = List_countNodes(cur);
    if (count == 0) {
        printf("List given is empty, no nodes to delete");
        return;
    }
    // if head is the node we wish to delete
    else if (cur == node) {
        *headRef = nxt;
        free(cur);
        if (*headRef == NULL) {
            *headRef = NULL;
        }
    }
    else {
        while (nxt != NULL) {
            // if next node is the node we are looking for
            if (nxt == node) {
                // if nxt is the tail
                if (nxt->next == NULL) {
                    cur->next = NULL;
                    free(nxt);
                    break;
                }
                // if nxt is not the last node in the list
                else {
                    cur->next = nxt->next;
                    free(nxt);
                    break;
                }
            }
            else {
                nxt = nxt->next;
                cur = cur->next; 
            }
        }
    }
}

/*
// insertion sort
void List_sort (struct nodeStruct **headRef) {
    
    if (*headRef != NULL) {

        struct nodeStruct *sorted = NULL;
        struct nodeStruct *cur = *headRef;

        while (cur != NULL) {
            struct nodeStruct *nxt = cur->next;

            // Insert current node into sorted list
            if (sorted == NULL || strcmp(cur->item, sorted->item) <= 0) {
                cur->next = sorted;
                sorted = cur;
            } 
            else {
                struct nodeStruct *temp = sorted;
                while (temp->next != NULL && strcmp(cur->item, temp->next->item) > 0) {
                    temp = temp->next;
                }
                cur->next = temp->next;
                temp->next = cur;
            }

            cur = nxt;
        }

        *headRef = sorted;
        
    }
}
*/

/* assume head is not null
void List_free (struct nodeStruct **headRef) {
    struct nodeStruct* cur = *headRef;
    struct nodeStruct* nxt;

    while (cur != NULL) {
        nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    
    *headRef = NULL;
}
*/