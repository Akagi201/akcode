      
/** @file test_prctl.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试设置应用自己的 Dumpable并且 limit为 Unlimited使用 prctl系统调用
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
#include <sys/prctl.h>

int main(void)
{
	int ret;

	/* Get the Dumpabe state */
	ret = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
	printf("PR_GET_DUMPABLE returned %d\n", ret);

	/* Set the Dumpable state to be enabled */
	ret = prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
	printf("PR_SET_DUMPABLE returned %d\n", ret);

	/* Get the Dumpable state again, make sure it was set */
	ret = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
	printf("PR_GET_DUMPABLE returned %d\n", ret);
	
	return 0;
}

