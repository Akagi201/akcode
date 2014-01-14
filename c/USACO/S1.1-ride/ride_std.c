      
/** @file ride_std.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
  * 
  * @author   liuboyf1
  * @date     2012-10-13
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <ctype.h>

int hash(char *s)
{
    int i, h;

    h = 1;
    for(i=0; s[i] && isalpha(s[i]); i++)
    {
        h = ((s[i]-'A'+1)*h) % 47;
    }
    return h;
}

int main(void)
{
    FILE *in, *out;
    char comet[100], group[100];	/* bigger than necessary, room for newline */

    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");

    fgets(comet, sizeof comet, in); /* 至多读sizeof comet -1 个，比gets安全，遇到行尾结束 */
    fgets(group, sizeof group, in);

    if(hash(comet) == hash(group))
    {
        fprintf(out, "GO\n");
    }
    else
    {
        fprintf(out, "STAY\n");
    }
    return 0;
}

