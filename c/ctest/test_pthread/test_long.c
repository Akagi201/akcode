      
/** @file test_long.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ≤‚ ‘pthread_t¿‡–Õ
  * 
  * @author   liuboyf1
  * @date     2012-9-20
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <pthread.h>

int main()
{
    pthread_t p1 = -1;
    printf("p1 = %ld\n", p1);
    if (0xffffffffffffffff == p1)
    {
        printf("equal\n");
    }
    else
    {
        printf("not equal\n");
    }
    pthread_t p2 = (pthread_t)-1;
    printf("p2 = %ld\n", p2);
    if (0xffffffffffffffff == p2)
    {
        printf("equal\n");
    }
    else
    {
        printf("not equal\n");
    }

    printf("long = %d\n", sizeof(long));
    printf("unsigned long = %d\n", sizeof(unsigned long int));
    printf("float = %d\n", sizeof(float));
    printf("double = %d\n", sizeof(double));
    printf("short = %d\n", sizeof(short));
    return 0;
}


