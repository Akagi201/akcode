
/** @file insertion_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��������
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
#if 0
/** @brief    ��������
  * @param[in] T : ����֧��С�ڲ���
  * @param[in] data : �����������, data != NULL
  * @param[in] size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T>
void InsertionSort (T data[], int size)
{
    int i,j;
	T temp;
    for (i=1; i<size; ++i)
    {
        temp=data[i];
        for (j=i; j>0 && temp<data[j-1]; --j)
        {
            data[j]=data[j-1];
        }
        data[j]=temp;
    }
}
#endif

/** @brief    ��������
  * @param[in]  T : Ԫ������
  * @param[in] Func : ��������(����C++)����ָ��(CҲ��)
  * @param[in] data : ���������飬data != NULL
  * @param[in] size : �����������С��size > 0
  * @param[in] f : ��������(C++����)����ָ��(C����)
  * @param[out]  data : ��f����
  * @return  
  */
template <typename T, typename Func >
void InsertionSort (T data[], int size, Func f)
{
    int i,j;
	T temp;
    for (i=1; i<size; ++i)
    {
        temp=data[i];
        for (j=i; j>0 && f(temp,data[j-1]); --j)
        {
            data[j]=data[j-1];
        }
        data[j]=temp;
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
    //InsertionSort(arr, 10);
    InsertionSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}
