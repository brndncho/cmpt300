// Sample test routine for the list module.
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define MAX_ITEM_SIZE 10

struct nodeStruct {
    char item[MAX_ITEM_SIZE]; // pointer to the string
    int item_size; // size of the string
    struct nodeStruct *next; // pointer to the next node
};

// allocate memory for node and initialize it with item.
struct nodeStruct* List_createNode(const char *item) {
    struct nodeStruct* newNode = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));

    if (newNode != NULL) {
        strcpy(newNode->item, item);
        //printf("%s\n", newNode->item);
        newNode->item_size = strlen(item);
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
struct nodeStruct* List_findNode(struct nodeStruct *head, const char *item) {
    if (head != NULL) {
        struct nodeStruct* cur = head;
        while (cur != NULL) {
            if (strcmp(cur->item, item) == 0) {
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

// assume head is not null
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


/*
Tests
*/

// given test
void test1() {

    printf("Starting tests...\n");
	struct nodeStruct *head = NULL;

	// Starting count:
	assert(List_countNodes(head) == 0);

	// Create 1 node:
	struct nodeStruct* firstNode = List_createNode("a");
	List_insertHead(&head, firstNode);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, "a") == firstNode);
	assert(List_findNode(head, "b") == NULL);
    

	// Insert tail:
	struct nodeStruct* lastNode = List_createNode("b");
	List_insertTail(&head, lastNode);
	assert(List_countNodes(head) == 2);
	assert(List_findNode(head, "a") == firstNode);
	assert(List_findNode(head, "b") == lastNode);
	assert(List_findNode(head, "c") == NULL);
    
	// Verify list:
	struct nodeStruct *current = head;
	assert(strcmp(current->item, "a") == 0);
	assert(current->next != NULL);
	current = current->next;
	assert(strcmp(current->item, "b") == 0);
	assert(current->next == NULL);
    
	// Sort and verify:
	List_sort(&head);
	current = head;
	assert(strcmp(current->item, "a") == 0);
	assert(current->next != NULL);
	current = current->next;
	assert(strcmp(current->item, "b") == 0);
	assert(current->next == NULL);
    
	// Delete
	assert(List_countNodes(head) == 2);
	struct nodeStruct *nodeOf0 = List_findNode(head, "a");
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 1);
	assert(List_findNode(head, "a") == NULL);
	current = head;
	assert(strcmp(current->item, "b") == 0);
	assert(current->next == NULL);

    List_free(&head);
    
	printf("\nExecution finished.\n");
}

void test2() {
    
    printf("Starting test2\n");

    struct nodeStruct *head = NULL;
    assert(List_countNodes(head) == 0);
    struct nodeStruct* firstNode = List_createNode("bob");
    struct nodeStruct* secondNode = List_createNode("olive");
    struct nodeStruct* lastNode = List_createNode("aaron");
	List_insertHead(&head, firstNode);
    List_insertTail(&head, secondNode);
    List_insertTail(&head, lastNode);
    assert(List_countNodes(head) == 3);

    struct nodeStruct *current = head;
    assert(strcmp(current->item, "bob") == 0);
	assert(current->next != NULL);
	current = current->next;
	assert(strcmp(current->item, "olive") == 0);
	assert(current->next != NULL);
    current = current->next;
    assert(strcmp(current->item, "aaron") == 0);
    assert(current->next == NULL);

    List_sort(&head);
    current = head;
    /*
    while(current != NULL) {
        printf("%s\n", current->item);
        current = current->next;
    }
    */
    current = head;
	assert(strcmp(current->item, "aaron") == 0);
	assert(current->next != NULL);
	current = current->next;
	assert(strcmp(current->item, "bob") == 0);
    assert(current->next != NULL);
    current = current->next;
    assert(strcmp(current->item, "olive") == 0);
	assert(current->next == NULL);

	assert(List_countNodes(head) == 3);
	struct nodeStruct *nodeOf0 = List_findNode(head, "bob");
	List_deleteNode(&head, nodeOf0);
	assert(List_countNodes(head) == 2);
	assert(List_findNode(head, "bob") == NULL);
	current = head;
	assert(strcmp(current->item, "aaron") == 0);
	assert(current->next != NULL);
    current = current->next;
    assert(strcmp(current->item, "olive") == 0);
    


    List_free(&head);
    printf("\nTest2 Complete\n");
}

void test3() {

    printf("starting test 3 \n");

    struct nodeStruct *head = NULL;
    assert(List_countNodes(head) == 0);
    struct nodeStruct* firstNode = List_createNode("andy");
	List_insertHead(&head, firstNode);
	assert(List_countNodes(head) == 1);
    struct nodeStruct *nodeOf0 = List_findNode(head, "andy");
    //printf("%s\n", nodeOf0->item);
	List_deleteNode(&head, nodeOf0);

    List_free(&head);
    printf("test 3 complete \n");
}

/*
 * Main()
 */
int main(int argc, char** argv)
{
	test1();
    test2();
    test3();
	return 0;
    
}
