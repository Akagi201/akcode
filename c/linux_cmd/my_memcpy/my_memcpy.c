      
/**@file my_memcpy.c
 * @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief  my_memcpy()实现函数
 * 
 * @author   liuboyf1
 * @date     2012-8-27
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#include <stdint.h>  //uint32_t
#include "my_memcpy.h"
//#include <assert.h>


/**@brief		  
 * @param[in]  目标地址，源地址，复制长度
 * @param[out] 得到复制后目标内容
 * @return	  -1 出错，0成功
 */
int my_memcpy(void *dst, void *src, int size)
{
    // 检查入口参数有效性
    if(!dst || !src || !size )
    {
        return -1;
    }

    if(dst == src)
    {
        return 0;
    }

    uint32_t *p_dst = (uint32_t *) dst;
    uint32_t *p_src = (uint32_t *) src;

    //int *p_dst = (int *) dst;
    size = size/sizeof(uint32_t) + 1; //不足4位的补0当4位使用
    //assert(size == 3);

    // 从高地址向低地址拷贝,从头向尾巴赋值
    if (dst < src)
    {
        while (size--)
        {
            *(p_dst++) = *(p_src++);
        }
    }
    // 从低地址向高地址拷贝,从尾向头赋值
    else
    {
        while (size--)
        {
            *(p_dst + size) = *(p_src + size);
        }
    }

    return 0;
}
