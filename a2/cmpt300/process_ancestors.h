// Structure to hold values returned by process_ancestors sys-call
#ifndef _PROCESS_ANCESTORS_H_
#define _PROCESS_ANCESTORS_H_

#define ANCESTOR_NAME_LEN 16
struct process_info {
    long pid;
    char name[ANCESTOR_NAME_LEN];
    long state;
    long uid;
    long nvcsw;
    long nivcsw;
    long num_children;
    long num_siblings;
};

asmlinkage long sys_process_ancestors(struct process_info *info_array, long size, long *num_filled);

#endif