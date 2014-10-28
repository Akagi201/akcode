/*
 * @file standard_pipe.c
 * @author Akagi201
 * @date 2014/10/28
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZE  (1024)

int main(void) {
    /*定义文件流指针*/
    FILE *fp = NULL;
    char *cmd = "ls -l";
    char buf[BUFSIZE] = {0};

    /*调用popen()函数执行相应的命令*/
    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Popen error\n");
        exit(1);
    }
    /*将命令 ls -l的输出的内容存到buf中，并显示在屏幕上*/
    while ((fgets(buf, BUFSIZE, fp)) != NULL) {
        printf("%s", buf);
    }
    pclose(fp);/*关闭文件流*/

    exit(0);
}
