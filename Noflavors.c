#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(Noflavors,int, toppings)

{
	printk("\nSorry, No more flavors available.\n");
	printk("No. of toppings left: %d\n", toppings);
	return 0;
}
