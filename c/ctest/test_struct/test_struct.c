      
/** @file test_struct.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试结构体初始化是否相同
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

typedef struct
{
    char str[10];
    int a[10];
    char *p;
}TEST;

int main()
{
    TEST A = {};

    TEST B;

    // memset(&B, 0, sizeof(B));

    if (0 == memcmp(&A, &B, sizeof(TEST)))
    {
        printf("equal\n");
    }
    else
    {
        printf("not equal\n");
    }
    return 0;
}


