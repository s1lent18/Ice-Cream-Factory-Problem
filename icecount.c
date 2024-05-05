#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE4(icecount,int, total, int, f1, int, f2,int,f3)
{
    printk("\nTotal Ice-creams sold today: %d\n", total);
    printk("Number of scoops of flavor 0 left: %d\n", f1);
    printk("Number of scoops of flavor 1 left: %d\n", f2);
    printk("Number of scoops of flavor 2 left: %d\n", f3);
    return 0;
}
