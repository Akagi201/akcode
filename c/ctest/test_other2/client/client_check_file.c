/**@file  client_check_file.c
 * @note  HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief �ͻ����еļ�������ļ��ĸ����������� 
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

/**@brief     ���ü򵥵����������ݱ任�����������ļ�ͷ�Ĵ���ͽ��
* @param[in] char *p_src,  ���ݵ�Դ��ַ
* @param[in] int len��     ���ݵĳ���
* @param[out] char *p_dst, ���ݵ�Ŀ�ĵ�ַ
* @return 0 - �ɹ���-1 - ʧ��
*/
int convert_data(char *p_src, char *p_dst, int len)
{
   // �̶��Ļ������������任
   unsigned char magic[] = {0xba, 0xcd, 0xbc, 0xfe, 0xd6, 0xca, 0xdd, 0xd3,
                            0xba, 0xb9, 0xa3, 0xab, 0xbf, 0xcb, 0xb5, 0xbe};
   int i = 0;
   int j = 0;
   int magic_len = 0;
   int start_magic = 0; 
   
   // �������
   if ((NULL == p_src) || (NULL == p_dst))
   {
        return -1;
   }
   magic_len = sizeof(magic);   
   while(i < len)
   {
        // ��start_magic����ÿ����ѭ��magic����ʼλ��
        for (j = 0; (j < magic_len) && (i < len); j++, i++)
        {
            // �������任
            *p_dst++ = *p_src++ ^ magic[(start_magic+j)%magic_len]; 
        }
        start_magic = (start_magic + 1) % magic_len;
   }

   return 0;
}

/** @brief  ���ļ�����У��ͼ��
 *  @param memStart  ������ʼλ�� 
 *  @param len       ���ݳ���
 *  @param checkSum  У���
 *  @return 0:ƥ��ɹ���-1��ƥ��ʧ��
 */ 
int check_checksum(char *memStart, int len, int checkSum)
{
    int i = 0;
    int sum = 0;
    
	//�������
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

/**@brief             �ж����ļ��Ƿ����д��
 * @param file_buffer �ļ����ݻ�����
 * @param file_head   �ļ�ͷ
 * @param dir         Ŀ¼��
 * @return   0:����д��  -1��������д��
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
	
	
	// ������Ч�Լ��	
	assert(dir != NULL);
	assert(file_head != NULL);
	assert(file_buffer != NULL);
	
	//ƴ�� Ŀ¼ + �ļ��� 
	if(dir[0] != '/')
	{   
	    //���·��
		sprintf(file_name, "./%s/", dir);
	}
	else
	{   
	    //����·��
		sprintf(file_name, "%s/", dir);
	}	
	strcat(file_name, file_head->file_name);
	
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		printf("\t\t�ļ�<%20s>�������ڣ�����д��!\n", file_head->file_name);					
		ret = 0;		    
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		if(length != file_head->file_len)
		{
			printf("\t\t�ļ�<%20s>�����ڣ�����С��ͬ������д��!\n", file_head->file_name);					
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
					printf("\t\t�ļ�<%20s>�����ڣ������ݲ�ͬ������д��!\n", file_head->file_name);					
					ret =  0;
					break;
				}
			}
						
			if(i == length)
			{
				printf("\t\t�ļ�<%20s>�����ڣ���������ͬ������д��!\n", file_head->file_name);				
				ret = -1;
			}
			free(p_buff);			
		}
	}
	return ret;
}

/**@brief             �������ļ�����������д���豸
 * @param file_buffer �ļ����ݻ�����
 * @param file_head   �ļ�ͷ
 * @param dir         Ŀ¼��
 * @return   0:д��ɹ�  -1��д��ʧ��
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
	
	// ������Ч�Լ��	
	assert(dir != NULL);
	assert(file_head != NULL);
	assert(file_buffer != NULL);
	
	
	//�½��ļ�,�ļ���ȡ
	if(dir[0] != '/')
	{   
	    //���·��
		sprintf(file_name, "./%s/", dir);
	}
	else
	{   
	    //����·��
		sprintf(file_name, "%s/", dir);
	}
	
	strcat(file_name, file_head->file_name);
	fp_new = fopen(file_name, "w+");
	if(fp_new == NULL)
	{
		printf("create file:%s fail\n", file_head->file_name);					
		return -1;		    
	}
		
	/*�����ļ���д����*/
	sum = 0;
	while(sum < file_head->file_len)
	{
		if(file_head->file_len - sum > BUFF_SIZE)
		{		
            //��д���ݳ��ȴ���BUFF_SIZEʱ		
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
            //��д���ݳ���С��BUFF_SIZEʱ		
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

/**@brief             �����ļ�������س���ʵ�ֶ������ļ����������Լ�⡢
 *                    д���жϡ���ѹ�ļ��ͻ�Ӧ������������� 
 * @param sock        �׽���������
 * @param file_buffer �ļ����ݻ�����
 * @return   0:�ɹ�  -1��ʧ��
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
	int updata_result = 0;    // -1:ʧ�ܣ�0���ɹ���1������Ҫ����
	
	memset(&pack_head, 0,sizeof(pack_head));
	memcpy(buffer, file_buffer, sizeof(pack_head));
	
	// ����������ͷ
	convert_data(buffer, (char *)&pack_head, sizeof(pack_head));
	
	// ��ʾ������ͷ��Ϣ
	printf("\n\n\t\t������ͷ��Ϣ:\n");	
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
	
	//����
	convert_data(p_sbuf, p_dbuf, length);
	
	//������ѹ�ļ����Ŀ¼
	umask(0);
	mkdir(dir, 0777);	
	
	for(i = 0; i < pack_head.file_nums; i++)
	{
		memcpy(&file_head, p_dbuf + sizeof(pack_head) + (i * sizeof(file_head)),
		       sizeof(file_head));
		
		//����ڴ����ļ���������			
		if(check_checksum(file_buffer + file_head.start_offset, 
		                  file_head.file_len,
		                  file_head.checksum)  != 0)
		{		    
		    printf("\n\t\t�ļ�<%20s>: ������У��ʧ�ܣ������ļ������˳���\n", 
		            file_head.file_name);
		             
		    exit(-1);
		}
		else
		{
			printf("\n\t\t�ļ�<%20s>: ������У��ɹ���\n", file_head.file_name);
		}
		
		//�ж��ܷ�д��
		bzero(buffer, FILE_NAME_LEN);
		if(check_is_write(file_buffer, &file_head, dir) == 0)
		{
			//�ļ���Ҫд��	
			if(check_write_file(file_buffer, &file_head, dir) == 0)
			{
				//�ļ�д��ɹ���				
				sprintf(buffer,"%20s,�����ɹ�[^_^]", file_head.file_name);
				if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
				{
					printf("send result error\n");
				}
				
				updata_result = 0;			
			}
			else
			{
				//�ļ�д��ʧ�ܣ�				
				sprintf(buffer,"%20s,����ʧ��[T_T]", file_head.file_name);
				if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
				{
					printf("send result error\n");
				}
				
				updata_result = -1;				
			}
		}
		else
		{
			//�ļ�����Ҫд�룡			
			sprintf(buffer,"%20s,�Ѿ�����[=_=]", file_head.file_name);
			if(send(sock, buffer, FILE_NAME_LEN, 0) < 0)
			{
				printf("send result error\n");
			}
			
			updata_result = 1;			
		}
		sleep(1);
					
	}	
	
	// ������������������	
	if(updata_result == 1)
	{
		if(send(sock,"R�汾��ͬ������Ҫ��������!",FILE_NAME_LEN,0) < 0)
		{
			printf("send result error\n");
		}
	}
	else if(updata_result == 0)
	{
		if(send(sock,"R�����ɹ�!",FILE_NAME_LEN,0) < 0)
		{
			printf("send result error\n");
		}
	}
	else
	{
		if(send(sock,"R����ʧ��!",FILE_NAME_LEN,0) < 0)
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
