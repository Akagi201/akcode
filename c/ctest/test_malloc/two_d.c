      
/** @file two_d.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ����ά����(m*n)����ռ�,m n ��δ֪�����,��������֪������ڴ治��ȫ�������ģ�ֻ�ǲ���������
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
#include <stdlib.h>

int main(void)
{
    char **a;
	char i;
	char j;
	int m = 3;
	int n = 4;
	// �ȷ���m��ָ�뵥Ԫ,ÿ����Ԫ��С��sizeof(char *)
	a = (char **)malloc(m * sizeof(char *));
	// �ٷ���n���ַ���Ԫ,�����m��ָ�뵥Ԫָ����n���ַ���Ԫ�׵�ַ
	for (i = 0; i < m; ++i)
	{
		a[i] = (char *)malloc(n * sizeof(char));
	}
	for (i = 0; i < m; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			a[i][j] = i + j;
			printf("%2d", a[i][j]);
		}
		printf("\n");
	}
	
	// �ͷ�Ϊ�����
	for (i = 0; i < m; ++i)
	{
		free((void *)a[i]);
	}
	free((void *)a);
    return 0;
}
