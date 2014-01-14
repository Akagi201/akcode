#include <stdlib.h>
#include <time.h>
#include <stdio.h>

struct student
{
    char name[10];
    int num;
}
/* 初始化学生信息 */
stu[5] = {"A", 0, "B", 0, "C", 0, "D", 0, "E", 0};

int main()
{
    int i,j;
    srand(time(0));
    for (i = 0; i < 5; ++i)
    {
        stu[i].num = rand()%5 + 1;
        for (j = 0; j < i; ++j)
        {
            if (i != 0 &&stu[i].num == stu[j].num)
            {
                stu[i].num = rand()%5 + 1;
                j = -1;
            }/* 重新产生座位号 */
        }
    }

    for (i = 0; i < 5; ++i)
    {
        printf("\n%s:%d\n", stu[i].name, stu[i].num);
    }
}

