      
/** @file misc_test.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-29
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */
#include <stdio.h>  
#include <sys/stat.h>	
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{	
	 int fd;  
	 fd = open("/dev/misc_node", O_RDONLY);  
	 if (fd < 0)  
	 {	
		 printf("open /dev/misc_node failed!\n");  
		 //printf("%s\n", strerror(errno));
		 return -1;  
	 }	
	   
	 printf("open /dev/misc_node ok!\n");  
	 if (ioctl(fd, 6) != 0)  
	 {	
		 printf("ioctl failed!\n");  
		 //printf("%s\n", strerror(errno));  
	 }	
	 else
	 {
		 printf("ioctl ok!\n");
	 }
	 close(fd);  
	 return 0;	
}	

