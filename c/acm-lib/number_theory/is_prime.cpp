      
/** @file is_prime.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ÅÐ¶ÏËØÊý
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
#include <math.h>

bool IsPrime(int u)
{
	if (0 == u || 1 == u)
	{
		return false;
	}
	if (2 == u)
	{
		return true;
	}
	if (0 == u % 2)
	{
		return false;
	}
	for (int i = 3; i <= sqrt(u); i += 2)
	{
		if (0 == u % i)
		{
			return false;
		}
		return true;
	}
}

int main()
{
	printf("IsPrime(13) = %d .\n", IsPrime(13));
	printf("IsPrime(12) = %d .\n", IsPrime(12));
	return 0;
}

