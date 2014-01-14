      
/** @file two_d2.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    二维数组强制转换成一维数组，打印出来
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

int main()
{
    int i;
    char a[2][3] = {{1,2,3},{4,5,6}};
    char *p = (char *)a;

    for(i = 0; i < 6; ++i)
    {
        printf("%d ",p[i]);
    }
	printf("\n");
    return 0;
}

