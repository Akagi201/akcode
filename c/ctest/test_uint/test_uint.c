      
/** @file test_uint.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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
    unsigned int a = (unsigned int) -1;

    if (a < 10)
    {
        printf("a < 10\n");
    }
    else
    {
        printf("a > 10\n");
    }

    printf("(unsigned int) -1 = %u\n",a);
    return 0;
}
