      
/** @file base64.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   xmq <xiemq@hikvision.com>
  *           songshilong <songshilong@hikvision.com.cn>
  * @date     2012-9-21
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note     xmq       2006/12/28  V1.0.0     create the file
  * @note  osngshilong  2011/11/03  V1.0.1     按照代码规范修改代码，并添加注释
  * @warning  
  */

#include <stdio.h>

#include "base64.h"

const char  *_base64_encode_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const signed char _base64_decode_chars[] = 
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};



/** @fn int base64_encode(const char *in_str, int in_len, char *out_str)
 *	@brief	BASE64加密算法实现  
 *	@param const char *in_str    原始输入数据
 *	@param int in_len            输入数据长度 
 *	@param char *out_str         加密结果输出


 *	@return index  加密结果长度 
 */
int base64_encode(const char *in_str, int in_len, char *out_str)
{
    unsigned char c1, c2, c3;
    int     i = 0;
    int     index = 0;

    if ((NULL == in_str) || (NULL == out_str))
    {
        return (-1);
    }

    while (i < in_len)
    {
        /* read the first byte */
        c1 = in_str[i++];
        if (i == in_len)         /* pad with "="*/
        {
            out_str[index++] = _base64_encode_chars[ c1>>2 ];
            out_str[index++] = _base64_encode_chars[ (c1&0x3)<<4 ];
            out_str[index++] = '=';
            out_str[index++] = '=';
            break;
        }

        /* read the second byte */
        c2 = in_str[i++];
        if (i == in_len)        /* pad with "=" */
        {
            out_str[index++] = _base64_encode_chars[ c1>>2 ];
            out_str[index++] = _base64_encode_chars[ ((c1&0x3)<<4) | ((c2&0xF0)>>4) ];
            out_str[index++] = _base64_encode_chars[ (c2&0xF)<<2 ];
            out_str[index++] = '=';
            break;
        }

        /* read the third byte */
        c3 = in_str[i++];

        /* convert into four bytes string */
        out_str[index++] = _base64_encode_chars[c1 >> 2];
        out_str[index++] = _base64_encode_chars[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
        out_str[index++] = _base64_encode_chars[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
        out_str[index++] = _base64_encode_chars[c3 & 0x3F];
    }

    return index;
}


/** @fn int base64_decode(const char *in_str, int in_len, char *out_str)
 *	@brief	BASE64加密算法实现  
 *	@param const char *in_str    原始输入数据
 *	@param int in_len            输入数据长度 
 *	@param char *out_str         解密结果输出


 *	@return index  解密输出数据长度
 */ 
int base64_decode(const char *in_str, int in_len, char *out_str)
{
    signed char c1, c2, c3, c4;
    int     i = 0;
    int     index = 0;

    while (i < in_len)
    {
        /* read the first byte */
        do
        {
            c1 = _base64_decode_chars[(int)in_str[i++]];
        } while((i < in_len) && (c1 == -1));

        if (c1 == -1)
        {
            break;
        }

        /* read the second byte */
        do
        {
            c2 = _base64_decode_chars[(int)in_str[i++]];
        } while ((i < in_len) && (c2 == -1));

        if (c2 == -1)
        {
            break;
        }

        /* assamble the first byte */
        out_str[index++] = (char)((c1 << 2) | ((c2 & 0x30) >> 4));

        /* read the third byte */
        do
        {
            c3 = in_str[i++];
            if (c3 == 61)        /* meet with "=", break */
            {
                return index;
            }

            c3 = _base64_decode_chars[(int)c3];
        } while ((i < in_len) && (c3 == -1));

        if(c3 == -1)
        {
            break;
        }

        /* assabmel the second byte */
        out_str[index++] = (char)(((c2 & 0XF) << 4) | ((c3 & 0x3C) >> 2));

        /* read the fourth byte */
        do
        {
            c4 = in_str[i++];
            if (c4 == 61)        /* meet with "=", break */
            {
                return index;
            }

            c4 = _base64_decode_chars[(int)c4];
        } while ((i < in_len) && (c4 == -1));

        if (c4 == -1)
        {
            break;
        }

        /* assamble the third byte */
        out_str[index++] = (char)(((c3 & 0x03) << 6) | c4);
    }

    return index;
}


