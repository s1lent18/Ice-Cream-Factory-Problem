#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(shopclose)

{
	printk("\nClosing Shop.\n");
	return 0;
}
