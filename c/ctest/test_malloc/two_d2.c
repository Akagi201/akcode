      
/** @file two_d2.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    动态申请二维数组m*n, 其中n为定长, 申请到的内存是线性的
  * 
  * @author   liuboyf1
  * @date     2012-10-13
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char i;
	char j;
	char (*a)[5];
	a = (char (*)[5])malloc(4 * 5 * sizeof(char));
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 5; ++j)
		{
			a[i][j] = i + j;
			printf("%2d", a[i][j]);
		}
		printf("\n");
	}
	//free(a);
	free((void *)a);
	return 0;
}