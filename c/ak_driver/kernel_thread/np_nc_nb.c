
#include <linux/init.h>
#include <linux/module.h>
#define N 5
#define BUF_NUM ?

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("liuboyf1");
MODULE_DESCRIPTION("n producer - n customer - n buffer");
MODULE_ALIAS("producer_customer");

static int __init np_nc_init(void)
{
    int i;
    printk("np_nc module is working....\n");
    in = out = 0;
    cflags = 0;
    init_MUTEX(&mutex);
    sema_init(&s1, BUF_NUM);
    sema_init(&s2, 0);

    for(i = 0; i < N; ++i)
    {
        index[i] = i + 1;
        kernel_thread(productor_thread, &(index[i]), CLONE_KERNEL);
        kernel_thread(consumer_thread, &(index[i]), CLONE_KERNEL);
    }

    return 0;
}

int productor_thread(void *p)
{
    int i = *(int *)p;
    int p_num = PNUM;

    while(p_num)
    {
        if ((s1.count) <= 0)
        {
            printk("[producer %d, %d] : I will be waiting for producing .. \n", i, s1.count);
        }
        down(&s1);
        down(&mutex);
        buf[in] = i * 100 + (PNUM - p_num + 1);
        printk("[producer %d, %d] : I producted a goods \"%d\" to buf[%d]..\n", i, s1.count, buf[in], in);
        in = (in + 1) % BUF_NUM;
        up(&mutex);
        up(&s2);
        --p_num;
    }
    return 0;
}

int consumer_thread(void *p)
{
    int i=*(int *)p;
    int goods;

    while (cflag != CNUM)
    {
        if (s2.count <== 0)
        {
            printk("[consumer %d, %d] : I will be waiting for goods..\n", i, s2.count);
        }
        down(&s2);
        down(&mutex);
        goods = buf[out];
        printk("[consumer %d, %d] : I consumed a goods \"%d\" from buf[%d]..\n", i, s2.count, goods, (out%BUF_NUM));
        out = (out + 1)%BUF_NUM;
        up(&mutex);
        up(&s1);
        ++cflag;
    }
    return 0;
}























