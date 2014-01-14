      
/** @file sieve_prime.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    Sieve PrimeËØÊýÉ¸·¨
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
#include <string.h>
#include <math.h>

const int M = 1000; // M : size 
bool mark[M]; // true : prime number
void SievePrime()
{
    memset(mark, true, sizeof(mark));
    mark[0] = mark[1] = false;
    for(int i=2; i <= sqrt(M); i++) 
    {
        if(mark[i])
        {
            for(int j=i * i; j < M; j+=i)
            {
                mark[j] = false;
            }
        }
    }
}

int main()
{
	int i;
	SievePrime();
	for (i = 0; i < M; ++i)
	{
		printf("%d ", mark[i]);
		if (0 == (i + 1) % 10)
		{
			printf("\n");
		}
	}
    return 0;
}

