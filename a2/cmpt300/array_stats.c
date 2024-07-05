#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include "array_stats.h"

SYSCALL_DEFINE3(array_stats, struct array_stats *, user_stats, long *, user_data, long, size){
	// initialize variables
	int i = 0;
	long temp = 0;
	long min = 0;
	long max = 0;
	long sum = 0;
	struct array_stats stats;

	// check if size is 0
	if (size <= 0) {
		printk("Error: size of array is <= 0\n");
		return -EINVAL;
	}

	// check if there is any error accessing stats or data pointers
	else if (!access_ok(user_stats, sizeof(struct array_stats)) || !access_ok(user_data, sizeof(long) * size)) {
		printk("Error: problem with accessing user array or user data\n");
		return -EFAULT;
	}

	// find stat values
	else {
		for (i = 0; i < size; i++) {
			if (copy_from_user(&temp, user_data + i, sizeof(long))) {
			printk("Error: copy_from_user failed copying from array\n");
			return -EFAULT;
			}
			if (i == 0) {
				min = temp;
				max = temp;
				sum = temp;
			}
			// replace min or max with temp if conditions meet
			else {
				if (temp <= min) {
					min = temp;
				}

				if (temp >= max) {
					max = temp;
				}
				// add to sum
				sum += temp;
			}
		}

		// add values into struct
		stats.min = min;
		stats.max = max;
		stats.sum = sum;

		// move values into user struct
		if (copy_to_user(user_stats, &stats, sizeof(struct array_stats))) {
			printk("Error: copy_to_user failed copying stats into user struct\n");
			return -EFAULT;
		}

		// print data values in kernal space
		printk("Min: %ld\n", stats.min);
		printk("Max: %ld\n", stats.max);
		printk("Sum: %ld\n", stats.sum);

		return 0;
	}

}
