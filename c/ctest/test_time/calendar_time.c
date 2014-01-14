// calendar_time.c

#include <time.h>
#include <stdio.h>

int main(void)
{
    time_t lt;
    lt = time(NULL);
    //time(&lt);
    printf("The number of seconds since January 1, 1970\n");
    printf("The Calendar Time now is %d\n", lt);
    return 0;
}
