#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pc, pr;
    int ps = fork();
    if (0 == pc)
    {
        printf("This is child process with pid of %d\n", getpid());
        sleep(10);
    }
    else if (pc > 0)
    {
        pr = wait(NULL);
        printf("I catched a child process with pid of %d\n",pr);
    }
    exit(0);
    return 0;
}
