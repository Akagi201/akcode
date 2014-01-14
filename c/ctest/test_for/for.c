
#include <stdio.h>
#include <time.h>

int main()
{
    int i = 0;
    for (i = 0; i < 10; ++i)
    {
        printf("#### %d.\n", i);
        if (5 == i)
        {
            sleep(1);
            --i;
            continue;
        }
        sleep(1);
    }
    return 0;
}
