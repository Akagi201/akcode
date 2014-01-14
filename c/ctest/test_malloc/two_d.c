      
/** @file two_d.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    给二维数组(m*n)分配空间,m n 都未知的情况,由申请易知，这个内存不是全部连续的，只是部分连续。
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
    char **a;
	char i;
	char j;
	int m = 3;
	int n = 4;
	// 先分配m个指针单元,每个单元大小是sizeof(char *)
	a = (char **)malloc(m * sizeof(char *));
	// 再分配n个字符单元,上面的m个指针单元指向这n个字符单元首地址
	for (i = 0; i < m; ++i)
	{
		a[i] = (char *)malloc(n * sizeof(char));
	}
	for (i = 0; i < m; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			a[i][j] = i + j;
			printf("%2d", a[i][j]);
		}
		printf("\n");
	}
	
	// 释放为逆过程
	for (i = 0; i < m; ++i)
	{
		free((void *)a[i]);
	}
	free((void *)a);
    return 0;
}
