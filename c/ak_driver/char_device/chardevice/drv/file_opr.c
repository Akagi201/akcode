      
/**@file file_opr.h
 * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
 * @brief  文件读写函数的实现
 * 
 * @author   liuboyf1
 * @date     2012-8-30
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History: 修改之前的接口，使其可以配合虚拟字符设备驱动工作       
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

 
#include "file_opr.h"
#include <linux/fs.h>     
#include <asm/uaccess.h>

/** @fn
  * @brief	    驱动程序来读取用户层文件的内容
  * @param[in]  filename : 要读取的文件名  read_max_len : 请求读取的长度
  * @param[out] data : 读取到的数据
  * @return	    0 : 成功 else : 失败
  */
INT32 file_read(const char * filename, UINT32 read_max_len, char * data) 
{ 
    struct file *filp = NULL; 
    struct inode *inode = NULL;
    UINT32 read_len = 0;
    
    mm_segment_t original_fs = 0;

    off_t fsize = 0; 
    unsigned long magic = 0; 

    // 检验参数的有效性
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

    read_len = (fsize < read_max_len) ? fsize : read_max_len; // 实际可读取的长度
	
    original_fs = get_fs();     // 获取当前的进程地址空间上限

    set_fs(KERNEL_DS);     // 将进程的地址空间上限置为一个固定值
    filp->f_op->read(filp, data, read_len, &(filp->f_pos)); 
    DBG_PRINT("file_read() : read %d bytes from file\n", read_len);
    DBG_PRINT("file_read() : content is :%s\n", data);

    set_fs(original_fs);     // 还原原先的进程地址空间上限
    filp_close(filp, NULL); 
    DBG_PRINT("Leave file_read()!\n");
    return 0;
} 

/** @fn
  * @brief	    驱动程序来写入用户层文件的内容
  * @param[in]  filename : 要读取的文件名 data : 请求写入的数据的指针 data_len : 准备写入的数据长度
  * @param[out]
  * @return	    0 : 成功 else : 失败
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

    // 获取当前的进程地址空间上限
    original_fs = get_fs();
    set_fs(KERNEL_DS);
    filp->f_op->write(filp, data, data_len,&filp->f_pos);// 只写入有效的长度

    //还原原先的进程地址空间上限
    set_fs(original_fs);
    DBG_PRINT("test *****write datalen  is :%d\n", data_len); 

    filp_close(filp,NULL);
    return 0;
}


