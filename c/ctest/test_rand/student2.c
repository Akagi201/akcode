#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct student
{
    char name[10];
    int num;
}
stu[5] = {"A", 0, "B", 0, "C", 0, "D", 0, "E", 0};

int main()
{
    int a[5];
    int i;
    int index;
    int x = 0;
    for (i = 0; i < 5; ++i)
    {
        a[i] = i + 1;  /* 存放座位号 */
    }
    srand(time(0));
    for (i = 0; i < 5;)
    {
        index = rand()%5; /* 随机抽取座位号 */
        if (a[index] != 0)
        {
            stu[i].num = a[index];
            a[index] = 0;
            ++i;
        }
    }
    for (i = 0; i < 5; ++i)
    {
        printf("\n%s:%d\n", stu[i].name, stu[i].num);
    }
    return 0;
}
