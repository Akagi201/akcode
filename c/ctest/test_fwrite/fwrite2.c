// fwrite2.c

#include <stdio.h>
#define SIZE 1

typedef struct
{
    char name[10];
    int num;
    int age;
    char addr[15];
}student;

student stu[SIZE];

void save()
{
    FILE *fp;
    int i;
    if (NULL == (fp = fopen("a.txt", "w")))
    {
        printf("Cannot open file!\n");
        return;
    }

    for (i = 0; i < SIZE; ++i)
    {
        if (1 != fwrite(&stu[i], sizeof(student), 1, fp))
        {
            printf("Write file error!\n");
        }
    }

    fclose(fp);
}

int main(void)
{
    int i;
    for (i = 0; i < SIZE; ++i)
    {
        scanf("%S%D%D%S", &stu[i].num, &stu[i].age, &stu[i].addr);
    }
    save();
    return 0;
}
