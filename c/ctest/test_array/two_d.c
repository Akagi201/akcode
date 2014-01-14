      
/** @file two_d.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    打印二位数组的地址
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
    char a[4][6];
    int i,j;
    for(i=0;i<4;++i)
    {
        for(j=0;j<6;++j)
        {
            printf("&a[%d][%d]:%p ", i, j, &a[i][j]);
        }
        printf("\n");
    }
    return 0;
}

