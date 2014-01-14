      
/**@file file_opr.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  �ļ���д������ʵ��
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History: �޸�֮ǰ�Ľӿڣ�ʹ�������������ַ��豸��������       
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

 
#include "file_opr.h"
#include <linux/fs.h>     
#include <asm/uaccess.h>

/** @fn
  * @brief	    ������������ȡ�û����ļ�������
  * @param[in]  filename : Ҫ��ȡ���ļ���  read_max_len : �����ȡ�ĳ���
  * @param[out] data : ��ȡ��������
  * @return	    0 : �ɹ� else : ʧ��
  */
INT32 file_read(const char * filename, UINT32 read_max_len, char * data) 
{ 
    struct file *filp = NULL; 
    struct inode *inode = NULL;
    UINT32 read_len = 0;
    
    mm_segment_t original_fs = 0;

    off_t fsize = 0; 
    unsigned long magic = 0; 

    // �����������Ч��
    if((NULL == filename) || (NULL == data))
    {
        return PTR_ERR(NULL);
    }

    DBG_PRINT("Enter file_read()!\n"); 
    filp = filp_open(filename,O_RDONLY,0);
    if(IS_ERR(filp) || (NULL == filp))
    {
      DBG_PRINT("[Fail]filp_open() error!\n"); 
      return PTR_ERR(filp) ;
    } 

    inode = filp->f_dentry->d_inode;  
    magic = inode->i_sb->s_magic; 
    fsize = inode->i_size;

    DBG_PRINT("file system magic:%li \n", magic); 
    DBG_PRINT("super blocksize:%li \n", inode->i_sb->s_blocksize); 
    DBG_PRINT("inode %li \n", inode->i_ino); 
    DBG_PRINT("file size:%i \n", (int)fsize);
    DBG_PRINT("max read_len :%d\n", read_max_len);

    read_len = (fsize < read_max_len) ? fsize : read_max_len; // ʵ�ʿɶ�ȡ�ĳ���
	
    original_fs = get_fs();     // ��ȡ��ǰ�Ľ��̵�ַ�ռ�����

    set_fs(KERNEL_DS);     // �����̵ĵ�ַ�ռ�������Ϊһ���̶�ֵ
    filp->f_op->read(filp, data, read_len, &(filp->f_pos)); 
    DBG_PRINT("file_read() : read %d bytes from file\n", read_len);
    DBG_PRINT("file_read() : content is :%s\n", data);

    set_fs(original_fs);     // ��ԭԭ�ȵĽ��̵�ַ�ռ�����
    filp_close(filp, NULL); 
    DBG_PRINT("Leave file_read()!\n");
    return 0;
} 

/** @fn
  * @brief	    ����������д���û����ļ�������
  * @param[in]  filename : Ҫ��ȡ���ļ��� data : ����д������ݵ�ָ�� data_len : ׼��д������ݳ���
  * @param[out]
  * @return	    0 : �ɹ� else : ʧ��
  */
INT32 file_write(char* filename, char* data, UINT32 data_len)
{
    struct file *filp; 
    mm_segment_t original_fs = 0;
    if((NULL == filename) ||(NULL == data))
    {
        return PTR_ERR(NULL);
    }
    filp = filp_open(filename, O_RDWR|O_CREAT, 0644); //|O_APPEND
    if(IS_ERR(filp) || (NULL == filp))
    {
      DBG_PRINT("[Fail]open error!\n"); 
      return PTR_ERR(filp) ;
    }   

    // ��ȡ��ǰ�Ľ��̵�ַ�ռ�����
    original_fs = get_fs();
    set_fs(KERNEL_DS);
    filp->f_op->write(filp, data, data_len,&filp->f_pos);// ֻд����Ч�ĳ���

    //��ԭԭ�ȵĽ��̵�ַ�ռ�����
    set_fs(original_fs);
    DBG_PRINT("test *****write datalen  is :%d\n", data_len); 

    filp_close(filp,NULL);
    return 0;
}


