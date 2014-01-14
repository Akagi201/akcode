
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int testdev;
    int i, ret;
    char buf[15];

    /*
     * open函数会调用:
     * static int mycdev_open(struct inode *inode, struct file *fp)
     */

    testdev = open("/dev/mycdev", O_RDWR);

    if (-1 == testdev)
    {
        printf("Cannot open file.\n");
        exit(1);
    }
    
    /*
     * read 函数会调用:
     * static int mycdev_read(struct file *fp, char __user *buf, size_t size,
     * loff_t *pos)
     */
    /* 关于15，具体数值要看在mycdev.c中定义了多长的字符串 */
    if (ret = read(testdev, buf, 15) < 15)
    {
        printf("Read error!\n");
        exit(1);
    }

    printf("%s\n", buf);
    close(testdev);
    return 0;
}



