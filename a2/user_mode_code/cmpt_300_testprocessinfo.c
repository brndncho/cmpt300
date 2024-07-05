#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "process_ancestors.h"
#include <assert.h>
#include <errno.h>

#define _PROCESS_ANCESTORS_ 438
#define ARRAY_SIZE 10

int main(int argc, char *argv[]) {
    struct process_info info_array[ARRAY_SIZE];
    int result;
    long num_filled;
    
    printf("\nDiving to kernel level\n\n");
	result = syscall(_PROCESS_ANCESTORS_, info_array, ARRAY_SIZE, &num_filled);
	printf("\nRising to user level with result = %d\n\n", result);

    if (result == 0) {
        printf("Number of filled entries: %ld\n", num_filled);
        for (long i = 0; i < num_filled; i++) {
            printf("Process ID = %ld, Program Name = %s, Current Process State = %ld, User ID = %ld, Voluntary Context Switches = %ld, Involuntary context swtiches = %ld, Children Processes = %ld, Sibling Processes = %ld\n", 
                    info_array[i].pid, info_array[i].name, info_array[i].state, info_array[i].uid, info_array[i].nvcsw, info_array[i].nivcsw, info_array[i].num_children, info_array[i].num_siblings);
        }
    }
    return 0;
}
