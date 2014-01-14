      
/** @file getopt2.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    使用getopt()解析
  * 
  * @author   liuboyf1
  * @date     2012-10-11
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
    int oc; /*选项字符 */
    char *b_opt_arg; /*选项参数字串 */
    while((oc = getopt(argc, argv, "ngl:")) != -1)
    {
        switch(oc)
        {
        case 'n':
            printf("New.\n");
            break;
        case 'g':
            printf("Great.\n");
            break;
        case 'l':
            b_opt_arg = optarg;
            printf("l is %s\n", b_opt_arg);
            break;
        }
    }
    return 0;
}

