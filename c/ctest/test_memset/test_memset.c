      
/** @file test_memset.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试memset() 初始化int 数组时候是赋值为 1111 还是 1
  * 
  * @author   liuboyf1
  * @date     2012-11-22
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

int main(void)
{
	int i;
	int a[8];
	memset(a, 1, sizeof(a));
	for(i = 0; i < 8; ++i)
	{
		printf("%d\n", a[i]);
	}
	return 0;
}
