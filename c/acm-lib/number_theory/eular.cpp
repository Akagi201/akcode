      
/** @file eular.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ŷ����������1 ... n-1 ����n���ʵ����ĸ���
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

int Eular(int n)
{
    int ret = 1;
    int i;
    for (i = 2; i * i <= n; ++i)
    {
        if (0 == n % i)
        {
            n /= i;
            ret *= i - 1;
            while (0 == n % i)
            {
                n /= i;
                ret *= i;
            }
        }
    }
    if (n > 1)
    {
        ret *= n - 1;
    }
    return ret;
}

int main()
{
    printf("Eular(5) = %d.\n", Eular(5));
    return  0;
}
