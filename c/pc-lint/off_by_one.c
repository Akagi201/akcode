
/** @file off_by_one.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    This illustrates what FlexeLint/PC-lint does with an off-by-one error.
  *
  * @author   liuboyf1
  * @date     2012-10-27
  * @version  V1.0.0
  *
  * @note ///Description here
  * @note History:
  * @note     <author>   <time>    <version >   <desc>
  * @note
  * @warning
  */

/* Off-By-One Example */
#include <stdio.h>
int main()
{
    int i;
    int a[] = {1,2,3};
    int n = sizeof(a)/sizeof(int);
    for(i=0;i<=n;i++)
        printf("a[%d]=%d\n",i,a[i]);
    return 0;
}


