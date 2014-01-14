      
/** @file type.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试类型的长度
  * 
  * @author   liuboyf1
  * @date     2012-9-22
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */


#include <stdio.h>
#include <pthread.h>

int main()
{
    printf("sizeof(int) = %d\n",sizeof(int));
    printf("sizeof(unsigned) = %d\n",sizeof(unsigned));
	printf("sizeof(unsigned int) = %d\n",sizeof(unsigned int));
	printf("sizeof(char) = %d\n",sizeof(char));
	printf("sizeof(unsigned char) = %d\n",sizeof(unsigned char));
	printf("sizeof(long) = %d\n",sizeof(long));
	printf("sizeof(long int) = %d\n",sizeof(long int));
	printf("sizeof(unsigned long) = %d\n",sizeof(unsigned long));
	printf("sizeof(long long) = %d\n",sizeof(long long));
	printf("sizeof(unsigned long long) = %d\n",sizeof(unsigned long long));
	printf("sizeof(pthread_t) = %d\n",sizeof(pthread_t));
    return 0;
}
