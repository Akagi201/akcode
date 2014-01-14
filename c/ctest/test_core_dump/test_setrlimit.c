      
/** @file test_setrlimit.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    应用程序自己手动limit core dump
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

#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

int main(void)
{
	int ret;
	struct rlimit rlim;

	/* Get the core dump limitation */
	ret = getrlimit(RLIMIT_CORE, &rlim);
	printf("RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max);

	/* Set the core dump limitation to be unlimited */
	rlim.rlim_cur = RLIM_INFINITY;
	rlim.rlim_max = RLIM_INFINITY;
	ret = setrlimit(RLIMIT_CORE, &rlim);
	printf("RLIMIT_CORE returned %d (%d, %d)\n", ret, rlim.rlim_cur, rlim.rlim_max);

	return 0;
}

