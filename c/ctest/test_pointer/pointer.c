      
/** @file test_pointer.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试指针类型强制转换
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

int main()
{
    char *p1 = "abcdefg";
    int *p2 = (int *)p1;

    printf("p2 = %d\n", *p2);
    return 0;
}


