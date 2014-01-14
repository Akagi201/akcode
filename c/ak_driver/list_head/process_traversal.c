#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sem.h>
#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1");
MODULE_DESCRIPTION("process traversal using list");
MODULE_ALIAS("process traversal module");

/*
static int __init traversal_init(void)
{
    struct task_struct *task;
    struct task_struct *p;
    struct list_head *pos;
    int count = 0;
    printk(KERN_EMERG "Process traversal enter begin: \n");
    task = &init_task;
    list_for_each(pos, &task->tasks)
    {
        p = list_entry(pos, struct task_struct, tasks);
        ++count;
        printk(KERN_ALERT "%d ---> %s\n", p->pid, p->comm);
    }
    printk(KERN_EMERG "The number of process is: %d\n",count);
    return 0;
}
*/
static int __init traversal_init(void)
{
    struct task_struct *pos;
    struct list_head *current_head;
    int count = 0;

    printk(KERN_EMERG "Traversal module is working...\n");
    current_head = &(current->tasks);
    list_for_each_entry(pos, current_head, tasks)
    {
        ++count;
        printk(KERN_EMERG "[process %d]: %s\'s pid is %d\n", count, pos->comm, pos->pid);
    }
    printk(KERN_EMERG "The number of process is : %d\n", count);
    return 0;
}

static void __exit traversal_exit(void)
{
    printk(KERN_EMERG "Process traversal exit.\n");
}

module_init(traversal_init);
module_exit(traversal_exit);


