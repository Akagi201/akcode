      
/**@file mem_proc.c
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  proc�ӿڵ�ʵ���ļ�
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */
 
#include <linux/proc_fs.h>
#include "mem_proc.h"
#include "chrdev.h"

extern PMyCharDevice g_chrdev_devices;   // �豸�ṹ��ָ�룬��chrdev_init_module() �з���ռ�
extern INT32 g_chrdvr_nums;              // �ַ��豸����

/** @fn
  * @brief	    proc�Ķ������ӿڣ���ȡchrdev�ַ��豸�����еĵ�ǰ�ַ�������дָ��
  * @param[in]  page : ����д�����ݵĻ����� start : δʹ�� off : ƫ���� count : δʹ�� eof : ���ض�������־
  * @param[out]
  * @return	   �ܹ����ص��ַ���(���豸ʱ�ۼ�) or ��Ӧʧ�ܴ���
  */

ssize_t chrdev_read_proc(char *page, char **start, off_t off,
                   int count, int *eof, void *data)
{
    int ret = 0;
    int len = 0;
    int tmp_len = 0;
    int i = 0;

    if(NULL == page)
    {
        return -EFAULT;
    }

    if(off >0)// ����ƫ�Ʒ���
    {
        *eof = 1;
        ret = 0;
    }
    else
    {
        for(i = 0; i<g_chrdvr_nums; i++)
        {
            // ��ʽ�����ص��ַ�
            tmp_len = sprintf(page + len, "\n devices[%d] cur_size: %d p_read :%0X p_write: %0X \n"
                              , i, g_chrdev_devices[i].cur_size, (UINT32)g_chrdev_devices[i].p_read
                              , (UINT32)g_chrdev_devices[i].p_write);
            DBG_PRINT("mem content is: \n %s \n", g_chrdev_devices[i].mem);
            len += tmp_len;
        }
        ret = len;
    }
    return ret;
}


/** @fn
 * @brief	   proc��д�����ӿڣ����û���������������ں˲��ӡ����
 * @param[in]  file : δʹ�� buffer : �û��㴫������ݻ��� count : buffer��С data : δʹ��
 * @param[out]
 * @return	   �ɹ�д����ַ��� or ��Ӧʧ�ܴ���
 */
ssize_t chrdev_write_proc(struct file *file, const char __user *buffer,
               unsigned long count, void *data)
{
    char k_buf[BUF_SIZE] = {0};
    INT32 ret = 0;
    INT32 len = min(BUF_SIZE, count);

    if(NULL == buffer)
    {
        return -EFAULT; 
    }
    // �û��ռ�--> �ں˿ռ�
    if(copy_from_user(k_buf, buffer, len ))
    {
        ret = -EFAULT;
    }
    else
    {
        // ����û���proc�ӿ����������
        DBG_PRINT("you have input: %s\n", k_buf);
        ret = len;
    }
    
    return ret;
}

/** @fn
  * @brief	    ����/proc�µ�chrdev_proc�ڵ�
  * @param[in]  
  * @param[out]
  * @return
  */

void chrdev_create_proc(void)
{
    struct proc_dir_entry  *entry = NULL;

    // ��/procĿ¼�´���һ����Ϊchrdev_proc���ļ�����Ĭ�Ͼ���ȫ�ֿɶ�Ȩ��
    entry = create_proc_entry("chrdev_proc", 666, NULL);
    if (entry)
    {
        entry->read_proc = chrdev_read_proc;
        entry->write_proc = chrdev_write_proc;
    }
    return;
}

/** @fn
  * @brief	    ɾ��/proc�µ�chrdev_proc�ڵ�
  * @param[in]  
  * @param[out]
  * @return
  */
void chrdev_remove_proc(void)
{
    // ɾ�������
    remove_proc_entry("chrdev_proc", NULL);
    return;
}

