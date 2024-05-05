#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE4(revenue,int, customer, int, tickets, int, a,int,b)
{
    printk("\nStats:\n");
    printk("Number Of Customers: %d\n", customer);
    printk("Revenue Generated: $ %d.%d\n", a,b);
    printk("Tickets Remaining: %d\n", tickets);
    return 0;
}
