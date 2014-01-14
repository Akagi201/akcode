
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

int main(void)
{
    int handle;
    char msg[] = "This is a test";
    char ch[50];
	int n;

    /* create a file */
    handle = open("hello.txt", O_CREAT | O_RDWR, 0777);
    /* write some data to the file */
    write(handle, msg, strlen(msg));
    /* seek to the beginning of the file */
    lseek(handle, 0L, SEEK_SET);
    /* reads chars from the file until we hit EOF */
   
/*    do
    {
        read(handle, &ch, 1);
        printf("%c", ch);
    }while(ch != EOF);*/
    
    while((n= read(handle, ch, 50)) > 0)
    {
	ch[n]='\0';
        printf("%s\n", ch);
        sleep(1);
    }
    close(handle);
    return 0;
}

