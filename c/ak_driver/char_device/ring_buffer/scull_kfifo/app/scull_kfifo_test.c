#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */
#define SCULL_KFIFO_MAGIC  'k'
/* Please use a different 8-bit number in your code */

#define SCULL_KFIFO_SIZE  _IO(SCULL_KFIFO_MAGIC,   0)
#define SCULL_KFIFO_RESET _IO(SCULL_KFIFO_MAGIC,   1)

int main()
{
    int sculltest;
    int code;
    char input;


    if ((sculltest = open("/dev/scull_kfifo", O_RDWR )) < 0 )  
    {
        printf("cannot open scull_kfifo!\n");
        exit(1);
    }
    if ( lseek(sculltest,0,SEEK_SET) == -1) 
    {
        printf("scull_kfifo: the module can not lseek!\n");
    }

    for ( ; input != 'q' ; getchar()) 	
    {

        printf("please input the command :");
        input= getchar();
        switch(input) 
        {

#if 1
        case '1':
            if ((code=ioctl( sculltest , SCULL_KFIFO_SIZE  , NULL ) ) < 0) 
            {
                printf("scull_kfifo: ioctl SCULL_KFIFO_SIZE error! code=%d \n",code);
            }
            else  
            {
                printf("scull_kfifo: ioctl SCULL_KFIFO_SIZE ok! len=%d  \n",code);
            }
            break;
        case '2':
            if ((code=ioctl( sculltest , SCULL_KFIFO_RESET  , NULL ) ) < 0) 
            {
                printf("scull_kfifo: SCULL_KFIFO_RESET error! code=%d \n",code);
            }
            else  
            {
                printf("scull_kfifo: SCULL_KFIFO_RESET ok! code=%d \n",code);
            }
            break;
#endif
        case 'q':
            break;

        default:  /* redundant, as cmd was checked against MAXNR */
            printf("scull_kfifo:  Invalid input ! only 1,2,q!\n");
        }
    }	
    close(sculltest);

    exit(0);
}
