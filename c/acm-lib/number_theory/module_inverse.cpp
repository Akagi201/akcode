      
/** @file module_inverse.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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
/* watashi
// a * x + b * y = gcd(a, b)	?? 意义不明
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

// ASSUME : gcd(a, m) == 1

long long ModInv(long long a, long long m)
{
    long long x, y;
    ExtGcd(a, m, x, y);
    return (x % m + m) % m;
}
*/

// Extended Euclid 扩展欧几里德算法
// 如果Gcd(a, b) = d, 则存在x, y, 使 d = a * x + b * y
// extended_euclid(a, b) = a * x + b * y

int ExtendedEuclid(int a, int b, int &x, int &y)
{
	int d;
	if (0 == b)
	{
		x = 1;
		y = 0;
		return a;
	}
	d = ExtendedEuclid(b, a % b, y, x);
	y -= a / b * x;
	return d;
}


// ax = 1 (mod n) Module Inverse 模逆元
int ModInv(int a, int n)
{
	int d, x, y;
	d = ExtendedEuclid(a, n, x, y);
	if (1 == d)
	{
		return (x % n + n) % n;
	}
	else
	{
		return -1; // no solution
	}
}

int main()
{
    printf("ModInv(5, 7) = %d.\n", ModInv(5, 7));
    return 0;
}

