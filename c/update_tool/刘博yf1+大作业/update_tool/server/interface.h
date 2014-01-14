      
/** @file interface.h
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-9-17
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#ifndef _INTERFACE_TOOL_
#define _INTERFACE_TOOL_

int process_cmdline(int argc, char **argv);
void usage(void);
int send_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int recv_data(int sock, char *buf, int buf_len, pthread_mutex_t *sock_lock);
int get_file_attr(int fd, struct stat *file_stat);
int stop_update(int sock, pthread_mutex_t *sock_lock);

#endif
