#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(Outofbounds)

{
	printk("\nEnter Number of Customers between 0 & 30.\n");
	return 0;
}
