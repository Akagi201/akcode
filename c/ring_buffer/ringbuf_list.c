
#include <stdio.h>
#include <stdlib.h>

typedef struct signal_loop_chain
{
    int data;
    struct signal_loop_chain *next;
}NODE;

// 返回值是链表头结点
NODE *create_loop_chain(int n)
{
    int i;
    NODE *head, *previous, *current;
    // 创建头结点
    previous = (NODE *)malloc(sizeof(NODE));
    if (previous == NULL)
    {
        exit(1);
    }
    previous->data = 0;
    previous->next = NULL;
    head = previous;

    // 依次添加循环链表中的每一个元素
    for (i = 0; i < n; ++i)
    {
        current = (NODE *)malloc(sizeof(NODE));
        if (current == NULL)
        {
            exit(1);
        }
        current->next = head;
        previous->next = current;
        previous = current;
    }
    return head;
}

int show(NODE *head)
{
    NODE *current;
    current = head->next;
    printf("List:\n");
    while (current != head) // 因为是循环链表, 判断结束标志为表头
    {
        printf("%4d", current->data);
        current = current->next;
    }
    printf("\n");
}

int read_buf(NODE *head)
{
    NODE *current;
    current = head->next;
    while(1) // 为什么是死循环, 什么时候停止?
    {
        printf("read number is %d\n", current->data);
        current = current->next;
        sleep(1);
    }
}

int write_buf(NODE *head)
{
    NODE *current;
    int i = 0;
    current = head->next;
    while(1) // 也是死循环?
    {
        current->data = i++;
        printf("write number is %d\n", current->data);
        current = current->next;
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    int num;
    char cmd;
    NODE *head;
    printf("please input node_num \n");
    scanf("%d", &num);
    head = create_loop_chain(num);
    printf("The ringbuf was found\n");
    show(head);

    while (1)
    {
        printf("please select r or w\n");
        scanf("%c", &cmd);

        if (cmd == 'r')
        {
            read_buf(head);
            show(head);
        }

        if (cmd == 'w')
        {
            write_buf(head);
            show(head);
        }
    }
    return 0;
}
