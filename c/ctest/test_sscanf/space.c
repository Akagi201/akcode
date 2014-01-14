
#include <stdio.h>

int main()
{
    // - 不是占位符，是区间符
    //char *p_time = "2006:03:18-2006:04:18";
    char *p_time = "2006:03:18 -2006:04:18";
    //char *p_time = "2006:03:18- 2006:04:18";
    //char *p_time = "2006:03:18 - 2006:04:18";

    char sz_time1[100] = {0};
    char sz_time2[100] = {0};

    sscanf(p_time, "%[0-9,:] - %[0-9,:]", sz_time1, sz_time2);
    sscanf(p_time, "%[0-9,:]-%[0-9,:]", sz_time1, sz_time2);
    
    printf("%s\n", sz_time1);
    printf("%s\n", sz_time2);

    return 0;
}
/*
看这个简单例子
从输入流中读取数据:

int a,b;
scanf("%d-%d",&a,&b); //输入数据的时候中间会用 - 分割数据,也是一种格式控制


sscanf换成了从pTime读取数据
sscanf(pTime, "%[0-9,:] - %[0-9,:]", szTime1, szTime2);

从pTime读取数据， %[0-9,:]读取的数据对应你后面的szTime1， 由于 - 不在集合[0-9,:]中，
所以在遇到 - 数据不再读取到szTime1中

后面%[0-9,:]是一样的道理
 */
