      
/** @file getopt_long2.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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
#include <getopt.h>

int do_name, do_gf_name;
char *l_opt_arg;

struct option longopts[] = {
    { "name", no_argument, NULL, 'n' },
    { "gf_name", no_argument, NULL, 'g' },
    { "love", required_argument, NULL, 'l' },
    { 0, 0, 0, 0},
};

int main(int argc, char *argv[])
{
    int c;
    while((c = getopt_long(argc, argv, ":l:", longopts, NULL)) != -1){
        switch (c){
        case 'n':
            printf("My name is LYR.\n");
            break;
        case 'g':
            printf("Her name is BX.\n");
            break;
        case 'l':
            l_opt_arg = optarg;
            printf("Our love is %s!\n", l_opt_arg);
            break;
        }
    }
    return 0;
}

