// Sample test routine for the list module.
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"

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