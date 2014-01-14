      
/** @file prime_table.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    用素数表判定素数,先调用initprime
  * 
  * @author   liuboyf1
  * @date     2012-9-21
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>

int plist[10000];
int pcount = 0;

int prime(int n)
{
    int i;
    if ((n != 2 && !(n % 2)) || (n != 3 && !(n % 3)) || (n != 5 && !(n % 5)) || (n != 7 && !(n % 7)))
    {
        return 0;
    }

    for (i = 0; plist[i] * plist[i] <= n; ++i)
    {
        if (!(n % plist[i]))
        {
            return 0;
        }
    }

    return n > 1;
}

void initprime()
{
    int i;
    for (plist[pcount++] = 2, i = 3; i < 50000; ++i)
    {
        if (prime(i))
        {
            plist[pcount++] = i;
        }
    }
}

int main()
{
    int i;
    initprime();
    for (i = 0; i < 10000; ++i)
    {
        printf("%d ", plist[i]);
        if (0 == (i + 1) % 10)
        {
            printf("\n");
        }
    }
    return 0;
}
