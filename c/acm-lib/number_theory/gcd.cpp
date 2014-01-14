      
/** @file gcd.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    Greatest Common Divisor���Լ��
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

/** ������ʽ
int Gcd(int x, int y)
{
    int t;
    while(y > 0) 
    {
        t = x % y;
        x = y;
        y = t;
    }
    return x;
}
*/

// �ݹ���ʽ
int Gcd(int a, int b)
{
    return b ? Gcd(a, b) * b;
}


// a * x + b * y = gcd(a, b)  ?? ���岻��
long long ExtGcd(long long a, long long b, long long x, long long y)
{
    if (0 == b)
    {
        x = 1;
        y = 0;
        return a;
    }
    else
    {
        int g = ExtGcd(b, a % b, y, x);
        y -= a / b * x;
        return g;
    }
}


int main()
{
    printf("Gcd(12, 16) = %d\n", Gcd(12, 16));
    printf("ExtGCD(12, 16) = %d\n", ExtGcd(12, 16, -1, 1));
    return 0;
}

