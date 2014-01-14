
#include <stdio.h>

#define MAXSIZE 8

int ringbuf[MAXSIZE];
int readidx = 0;
int writeidx = 0;

int next_data_handle(int addr)
{
    return (addr + 1) == MAXSIZE ? 0 : (addr + 1);
}

int write_data(int data)
{
    int i;
    *(ringbuf + writeidx) = data;
    writeidx = next_data_handle(writeidx);
    for(i = 0; i < MAXSIZE; ++i)
    {
        printf("%4d", *(ringbuf + i));
        if (MAXSIZE - 1 == i)
        {
            printf("\n");
        }
    }
}

void print_data()
{
    int i;
    for (i = 0; i < MAXSIZE; ++i)
    {
        printf("%4d", *(ringbuf + i));
        if (MAXSIZE - 1 == i)
        {
            printf("\n");
        }
    }
}

void read_data()
{
    printf("read data is : %d\n", *(ringbuf + readidx));
    readidx = next_data_handle(readidx);
}

int main(int argc, char **argv)
{
    int data;
    char cmd;

    do{
        printf("select:\nw\t--write\nr\t--read\np\t--print\nq\t--quit\n");
        scanf("%s", &cmd);

        switch (cmd)
        {
            case 'w' :
                printf("please input data:");
                scanf("%d", &data);
                write_data(data);
                break;
            case 'r' :
                read_data();
                break;
            case 'p' :
                print_data();
                break;
            case 'q' :
                printf("quit\n");
                break;
            default :
                printf("Command error.\n");
        }
    }while (cmd != 'q');
    return 0;
}

