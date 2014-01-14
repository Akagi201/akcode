/**@file  client_check_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief 客户端中的检测升级文件的各个函数定义 
 *
 * @author		yanghongwen
 * @date		2012/09/14
 * @version  V1.0
 *
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning 
 */

#include "client_check_file.h"
#include "upgrade_file.h"

/**@brief     利用简单的异或进行数据变换，用于升级文件头的打包和解包
* @param[in] char *p_src,  数据的源地址
* @param[in] int len，     数据的长度
* @param[out] char *p_dst, 数据的目的地址
* @return 0 - 成功，-1 - 失败
*/
int convert_data(char *p_src, char *p_dst, int len)
{
   // 固定的幻数，用于异或变换
   unsigned char magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
                            0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
   int i = 0;
   int j = 0;
   int magic_len = 0;
   int start_magic = 0; 
   
   // 参数检查
   if ((NULL == p_src) || (NULL == p_dst))
   {
        return -1;
   }
   magic_len = sizeof(magic);   
   while(i < len)
   {
        // 用start_magic控制每次内循环magic的起始位置
        for (j = 0; (j < magic_len) && (i < len); j++, i++)
        {
            // 进行异或变换
            *p_dst++ = *p_src++ ^ magic[(start_magic+j)%magic_len]; 
        }
        start_magic = (start_magic + 1) % magic_len;
   }

   return 0;
}

/** @brief  对文件进行校验和检测
 *  @param memStart  数据起始位置 
 *  @param len       数据长度
 *  @param checkSum  校验和
 *  @return 0:匹配成功，-1：匹配失败
 */ 
int check_checksum(char *memStart, int len, int checkSum)
{
    int i = 0;
    int sum = 0;
    
	//参数检测
    assert(memStart != NULL);    
    
    sum = 0;
    for(i = 0; i < len; i++)
    {        
        sum += (unsigned char)memStart[i];                     
    } 
    
    if(sum != checkSum)
    {
    	return -1;
    }
            
    return 0;    
}

/**@brief             判断新文件是否可以写入
 * @param file_buffer 文件数据缓冲区
 * @param file_head   文件头
 * @param dir         目录名
 * @return   0:可以写入  -1：不可以写入
 */
int check_is_write(char *file_buffer, UPGRADE_FILE_HEADER *file_head, char *dir)
{	
    int i = 0;
    int ret = 0;
    int err = 0;
	int length = 0;	
	char *p_buff = NULL;	
	char *memStart = NULL;
	char file_name[FILE_NAME_LEN];
	FILE *fp = NULL;
	
	
	// 参数有效性检测	
	assert(dir != NULL);
	assert(file_head != NULL);
	assert(file_buffer != NULL);
	
	//拼接 目录 + 文件名 
	if(dir[0] != '/')
	{   
	    //相对路径
		sprintf(file_name, "./%s/", dir);
	}
	else
	{   
	    //绝对路径
		sprintf(file_name, "%s/", dir);
	}	
	strcat(file_name, file_head->file_name);
	
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		printf("\t\t文件<%20s>：不存在，可以写入!\n", file_head->file_name);					
		ret = 0;		    
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		if(length != file_head->file_len)
		{
			printf("\t\t文件<%20s>：存在，但大小不同，可以写入!\n", file_head->file_name);					
			ret =  0;
		}
		else
		{
			p_buff = (char *)malloc(length);
			if(p_buff == NULL)
			{
				printf("\n\t\tcheck_is_write: malloc error\n");
				exit(-1);
			}
			
			fseek(fp, 0, SEEK_SET);
			err = fread(p_buff, 1, length, fp);			
			if( err < length)
			{
				printf("\n\t\tcheck_is_write: fread error\n");
				free(p_buff);
				exit(-1);
			}
			
			memStart = file_buffer + file_head->start_offset;
			for(i = 0; i < length; i++)
			{
				if(p_buff[i] == memStart[i])
				{
					continue;
				}
				else
				{					
					printf("\t\t文件<%20s>：存在，但数据不同，可以写入!\n", file_head->file_name);					
					ret =  0;
					break;
				}
			}
						
			if(i == length)
			{
				printf("\t\t文件<%20s>：存在，且数据相同，放弃写入!\n", file_head->file_name);				
				ret = -1;
			}
			free(p_buff);			
		}
	}
	return ret;
}

/**@brief             将升级文件从升级包中写入设备
 * @param file_buffer 文件数据缓冲区
 * @param file_head   文件头
 * @param dir         目录名
 * @return   0:写入成功  -1：写入失败
 */
int check_write_file(char *file_buffer, UPGRADE_FILE_HEADER *file_head, char *dir)
{		
	int i = 0;	
	int sum = 0;
	int rdlen = 0;
	int wrlen = 0;
	int length = 0;	
	char buffer[BUFF_SIZE];	
	char file_name[FILE_NAME_LEN];
	FILE *fp_new = NULL;
	
	// 参数有效性检测	
	assert(dir != NULL);
	assert(file_head != NULL);
	assert(file_buffer != NULL);
	
	
	//新建文件,文件提取
	if(dir[0] != '/')
	{   
	    //相对路径
		sprintf(file_name, "./%s/", dir);
	}
	else
	{   
	    //绝对路径
		sprintf(file_name, "%s/", dir);
	}
	
	strcat(file_name, file_head->file_name);
	fp_new = fopen(file_name, "w+");
	if(fp_new == NULL)
	{
		printf("create file:%s fail\n", file_head->file_name);					
		return -1;		    
	}
		
	/*往新文件中写数据*/
	sum = 0;
	while(sum < file_head->file_len)
	{
		if(file_head->file_len - sum > BUFF_SIZE)
		{		
            //待写数据长度大于BUFF_SIZE时		
			memcpy(buffer, file_buffer + file_head->start_offset + sum, BUFF_SIZE);
				
			wrlen = fwrite(buffer, 1, BUFF_SIZE, fp_new);
			if(rdlen < 0)
			{
				printf("fwrite error\n");				
				fclose(fp_new);				
				return -1;
			}				
			//printf("rdlen = %d, wrlen = %d\n", rdlen, wrlen);
				
			if(BUFF_SIZE != wrlen)
			{	
				printf("1:write data to new file error\n");				
				fclose(fp_new);
				return -1;				
			}
				
			sum += BUFF_SIZE;
		}
		else
		{		
            //待写数据长度小于BUFF_SIZE时		
			memcpy(buffer, file_buffer + file_head->start_offset + sum, 
			       file_head->file_len - sum);
				
			wrlen = fwrite(buffer, 1, (file_head->file_len - sum), fp_new);
			if(wrlen < 0)
			{
				printf("fwrite error\n");				
				fclose(fp_new);
				return -1;
			}
				
			if((file_head->file_len - sum) != wrlen)
			{	
				printf("2:write data to new file error\n");				
				fclose(fp_new);
				return -1;				
			}
				
			break;
		}
	}
		
	fclose(fp_new);	
	return 0;
}

/**@brief             升级文件检测主控程序，实现对升级文件进行完整性检测、
 *                    写入判断、解压文件和回应服务器升级结果 
 * @param sock        套接字描述符
 * @param file_buffer 文件数据缓冲区
 * @return   0:成功  -1：失败
 */
int check_file(int sock,char *file_buffer)
{
	FILE *fp_new = NULL;
	FIRMWARE_HEADER pack_head;	
	UPGRADE_FILE_HEADER file_head;
	char dir[FILE_NAME_LEN] = "updata";
	char buffer[BUFF_SIZE];	
	char file_name[FILE_NAME_LEN];
	char *p_sbuf = NULL;
	char *p_dbuf = NULL;
	int length = 0;
	int i = 0;	
	int sum = 0;
	int rdlen = 0;
	int wrlen = 0;
	int updata_result = 0;    // -1:失败，0：成功，1：不需要升级
	
	memset(&pack_head, 0,sizeof(pack_head));
	memcpy(buffer, file_buffer, sizeof(pack_head));
	
	// 解密升级包头
	convert_data(buffer, (char *)&pack_head, sizeof(pack_head));
	
	// 显示升级包头信息
	printf("\n\n\t\t升级包头信息:\n");	
	printf("\t\tmagic_number = 0x%x\n", pack_head.magic_number);
	printf("\t\theader_checksum = %u\n", pack_head.header_checksum);
	printf("\t\theader_length = %u\n", pack_head.header_length);
	printf("\t\tfile_nums = %u\n", pack_head.file_nums);
	printf("\t\tlanguage = %u\n", pack_head.language);
	printf("\t\tdevice_class = %u\n", pack_head.device_class);
	printf("\t\toem_code = %u\n", pack_head.oem_code);	
	
	sleep(1);
	
	length = pack_head.file_nums * sizeof(file_head) + sizeof(pack_head);
	
	p_sbuf = (char *)malloc( length );
	if(p_sbuf == NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	
	p_dbuf = (char *)malloc( length );
	if(p_dbuf == NULL)
	{
		printf("malloc error\n");
		return -1;
	}
	
	memset(p_sbuf, 0,length);	
	memcpy(p_sbuf, file_buffer, length);
	
	//解密
	convert_data(p_sbuf, p_dbuf, length);
	
	//创建解压文件存放目录
	umask(0);
	mkdir(dir, 0777);	
	
	for(i = 0; i < pack_head.file_nums; i++)
	{
		memcpy(&file_head, p_dbuf + sizeof(pack_head) + (i * sizeof(file_head)),
		       sizeof(file_head));
		
		//检测内存中文件的完整性			
		if(check_checksum(file_buffer + file_head.start_offset, 
		                  file_head.file_len,
		                  file_head.checksum)  != 0)
		{		    
		    printf("\n\t\t文件<%20s>: 完整性校验失败，升级文件有误，退出！\n", 
		            file_head.file_name);
		             
		    exit(-1);
		}
		else
		{
			printf("\n\t\t文件<%20s>: 完整性校验成功！\n", file_head.file_name);
		}
		
		//判断能否写入
		bzero(buffer, FILE_NAME_LEN);
		if(check_is_write(file_buffer, &file_head, dir) == 0)
		{
			//文件需要写入	
			if(check_write_file(file_buffer, &file_head, dir) == 0)
			{
				//文件写入成功！				
				sprintf(buffer,"%20s,升级成功[^_^]", file_head.file_name);
				if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
				{
					printf("send result error\n");
				}
				
				updata_result = 0;			
			}
			else
			{
				//文件写入失败！				
				sprintf(buffer,"%20s,升级失败[T_T]", file_head.file_name);
				if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
				{
					printf("send result error\n");
				}
				
				updata_result = -1;				
			}
		}
		else
		{
			//文件不需要写入！			
			sprintf(buffer,"%20s,已经存在[=_=]", file_head.file_name);
			if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
			{
				printf("send result error\n");
			}
			
			updata_result = 1;			
		}
		sleep(1);
					
	}	
	
	// 向服务器发送升级结果	
	if(updata_result == 1)
	{
		if(send(sock,"R版本相同，不需要进行升级!",FILE_NAME_LEN,0) < 0)
		{
			printf("send result error\n");
		}
	}
	else if(updata_result == 0)
	{
		if(send(sock,"R升级成功!",FILE_NAME_LEN,0) < 0)
		{
			printf("send result error\n");
		}
	}
	else
	{
		if(send(sock,"R升级失败!",FILE_NAME_LEN,0) < 0)
		{
			printf("send result error\n");
		}
	}		
	
	close(sock);
	free(p_sbuf);
	p_sbuf = NULL;
	free(p_dbuf);
	p_dbuf = NULL;	
	return 0;
}
