      
/** @file lcm.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    求最小公倍数
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

int Gcd(int a, int b)
{
    return b ? Gcd(b, a % b) : a;
}


inline int Lcm(int a, int b)
{
    return a/Gcd(a, b) * b;
}

int main()
{
    printf("Lcm(12, 16) = %d\n", Lcm(12, 16));
    return 0;
}
