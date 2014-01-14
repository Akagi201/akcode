      
/** @file bubble_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ð������
  * 
  * @author   liuboyf1
  * @date     2012-9-19
  * @version  V1.0.0
  * 
  * @note ///Description here 
  * @note History:    
  * @note     <author>   <time>    <version >   <desc>
  * @note  
  * @warning  
  */

#include <stdio.h>
#include <algorithm>
#if 0

/** @brief    ð������
  * @param[in]  T : ����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T>
void BubbleSort (T data[], int size)
{
    int i,j;
    for (i=0; i<size-1; ++i)
    {
        for (j=size-1; j>i; --j)
        {
            if (data[j]<data[j-1])
            {
                std::swap(data[j], data[j-1]);
            }
        }
    }
}
#endif


/** @brief    ð������
  * @param[in]  T : Ԫ�����ͣ�����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[in]  f : ��������(C++����)����ָ��(C����)
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T, typename Func>
void BubbleSort (T data[], int size, Func f)
{
    int i,j;
    for (i=0; i<size-1; ++i)
    {
        for (j=size-1; j>i; --j)
        {
            if (f(data[j],data[j-1]))
            {
                std::swap(data[j], data[j-1]);
            }
        }
    }
}

bool cmp(int a, int b)
{
	return a<b;
}


int main(void)
{
    int i;
    int arr[] = {10, 9, 8, 4, 5, 7, 6, 3, 1, 4};
    //BubbleSort(arr, 10);
    BubbleSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}

