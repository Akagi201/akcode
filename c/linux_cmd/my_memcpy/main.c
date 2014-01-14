      
/**@file main.c
 * @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief  功能测试
 * 
 * @author   liuboyf1
 * @date     2012-8-27
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
#include <string.h>
//#include <assert.h>
#include "my_memcpy.h"

#define TEST_SIZE 12 // 要求为4的倍数,因为要不齐不足4位部分

int main(int argc, char *argv[])
{
    int i;
    int real_size;
    char test_src[TEST_SIZE];
    memset(test_src, 0, sizeof(test_src));
    strcpy(test_src, "1234567890");
	// 末尾不足的4位用后面的2位补齐4位
    real_size = sizeof(test_src) - 2;  
    //assert(10 == real_size);

    char test_dst[TEST_SIZE];
    memset(test_dst, 0, sizeof(test_dst));

    // dst is NULL
    printf("目标地址为空\n");
    if (-1 == my_memcpy(NULL, test_src, real_size))
    {
        printf("Error:目标地址为空！\n");
    }

    // src is NULL
    printf("源地址为空\n");
    if (-1 == my_memcpy(test_dst, NULL, real_size))
    {
        printf("Error:源地址为空！\n");
    }

    // size = 0
    printf("复制长度为空\n");
    if (-1 == my_memcpy(test_dst, test_src, 0))
    {
        printf("Error:复制长度为0！\n");
    }

    // dst == src
    my_memcpy(test_src, test_src, real_size);
    printf("源地址与目标地址相同，输出目标：\n");
    for (i = 0; i < real_size; ++i)
    {
        printf("%c ", *(test_src+i));
    }
    printf("\n");

    // dst > src
    my_memcpy(test_src+1, test_src, real_size);
    printf("目标地址大于源地址，输出目标：\n");
    for (i = 0; i < real_size; ++i)
    {
        printf("%c ", *(test_src+i+1));
    }
    printf("\n");

    // dst < src
    strcpy(test_src, "1234567890");
    memset(test_dst, 0, sizeof(test_dst));
    printf("目标地址小于源地址，输出目标：\n");
    my_memcpy(test_src-1, test_src, real_size);
    for (i = 0; i < real_size; ++i)
    {
        printf("%c ", *(test_src+i-1));
    }
    printf("\n");
    return 0;
}
