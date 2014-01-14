
/*
The functions gettimeofday and settimeofday can get and set the time as well as a timezone.
The tv argument is a timeval struct, as specified in <sys/time.h>
struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};
其中, 返回的timeval值为Epoch(00:00:00 1970-01-01 UTC)到创建struct timeval时的时间, tv_sec为秒数部分,
tv_usec为微秒部分(10^-6 s).
*/

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main(void)
{
    struct timeval tv;
    while (1)
    {
        gettimeofday(&tv, NULL);
        printf("time %u : %u\n", tv.tv_sec, tv.tv_usec);
        sleep(2);
    }

    return 0;
}
