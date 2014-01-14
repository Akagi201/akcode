      
/** @file selection_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ѡ������
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
/** @brief    ѡ������
  * @param[in]  T : ����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T>
void SelectionSort (T data[], int size)
{
    int i,j,k;  
    for (i=0; i<size-1; ++i)
    {
        k=i;
        for (j=i+1; j<size; ++j)
        {
            if (data[j]<data[k])
            {
                k=j;
            }
        }
        std::swap(data[i], data[k]);
    }
}
#endif


/** @brief    ѡ������
  * @param[in]  T : Ԫ�����ͣ�����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */

template <typename T, typename Func>
void SelectionSort (T data[], int size, Func f)
{
    int i,j,k;  
    for (i=0; i<size-1; ++i)
    {
        k=i;
        for (j=i+1; j<size; ++j)
        {
            if (f(data[j],data[k]))
            {
                k=j;
            }
        }
        std::swap(data[i], data[k]);
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
    //SelectionSort(arr, 10);
    SelectionSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}


