      
/** @file getopt3.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �ı�getopt()�Դ��������в�����Ϣ�������Ϊ
  * 
  * @author   liuboyf1
  * @date     2012-10-12
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */
#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    int oc; /*ѡ���ַ� */
    char *b_opt_arg; /*ѡ������ִ� */
    while((oc = getopt(argc, argv, "ngl:")) != -1)
    {
        switch(oc)
        {
        case 'n':
            printf("new.\n");
            break;
        case 'g':
            printf("great.\n");
            break;
        case 'l':
            b_opt_arg = optarg;
            printf("l is %s\n", b_opt_arg);
            break;
        case '?':
            printf("arguments error!\n");
            break;
        }
    }
    return 0;
}

