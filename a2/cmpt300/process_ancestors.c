#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include "process_ancestors.h"

SYSCALL_DEFINE3(process_ancestors, struct process_info *, info_array, long, size, long *, num_filled) {
    long process_count = 0;
    struct task_struct *p;
    struct process_info proc_info; /*struct for storing process info in kernel space*/
    
    // handle size <= 0 
    if (size <= 0) {
		printk("Error: size of array is <= 0\n");
		return -EINVAL;
	}

    // handle problems with accessing info_array or num_filled
    else if (!access_ok(info_array, sizeof(struct process_info) * size) || !access_ok(num_filled, sizeof(long))) {
		printk("Error: problem with accessing info_array or num_filled\n");
		return -EFAULT;
	}
    
    else {
        p = current; // point to current process

        /*loop unless array full or searched fully in process tree*/
        while (process_count < size && p->parent != p) {
            struct list_head *node;

            proc_info.pid = p->pid;
            strncpy(proc_info.name, p->comm, ANCESTOR_NAME_LEN - 1);
            proc_info.name[ANCESTOR_NAME_LEN - 1] = '\0';
            proc_info.state = p->state;
            proc_info.uid = p->real_cred->uid.val;
            proc_info.nvcsw = p->nvcsw;
            proc_info.nivcsw = p->nivcsw;

            // source : https://stackoverflow.com/questions/36536274/calculating-the-number-of-children-and-sibiling-of-a-process-in-the-kernel-mode (Author:falhumai)
            // count children
            proc_info.num_children = 0;
            list_for_each(node, &(p->children)) {
                proc_info.num_children++;
            }
            // count siblings
            proc_info.num_siblings = 0;
            list_for_each(node, &(p->sibling)) {
                proc_info.num_siblings++;
            }

            // move to user space
            if (copy_to_user(&info_array[process_count], &proc_info, sizeof(struct process_info))) {
                printk("Error: Fail to copy to user space array\n");
                return -EFAULT;
            }
            process_count++;
            p = p->parent;
        }

        // move num_filled to user space
        if (copy_to_user(num_filled, &process_count, sizeof(long))) {
            printk("Error: Fail to copy count into num_filled\n");
            return -EFAULT;
        }

        return 0;
    }
}