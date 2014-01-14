      
/** @file quick_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ���������
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
#include <time.h>

/** @brief	 ��������������
  * @param[in]  T : ����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  first : ��������������
  * @param[in]  lsat : ������������յ����һ���˵�
  * @param[out]	data���ǽ�������
  * @return	
  */

template <typename T>
void quick_sort (T data[], int frist, int last)
{
    int lower=frist+1;
    int upper=last;
    int t=rand()%(last-frist)+frist;
    std::swap(data[frist], data[t]);
    T& bound=data[frist];
    while (lower<=upper)
    {
        while (data[lower]<bound)
        {
            ++lower;
        }
        while (bound<data[upper])
        {
            --upper;
        }
        if (lower<upper)
        {
            std::swap(data[lower], data[upper]);
            ++lower;
            --upper;
        }
        else
            ++lower;
    }
    std::swap(data[upper], data[frist]);
    if (frist<upper-1)
        quick_sort(data, frist, upper-1);
    if (upper+1<last)
        quick_sort(data, upper+1, last);
}


/** @brief	  ��������
  * @param[in]	T : ����֧��С�ڲ���
  * @param[in]	data : ����������, data != NULL
  * @param[in]	size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */

template <typename T>
void QuickSort (T data[], int size)
{
    int i, max;
    if (size<2)
        return;
    for (i=1, max=0; i<size; ++i) 
    {
        if (data[max]<data[i])
            max=i;
    }
    std::swap(data[size-1], data[max]);
    srand(time(0));
    quick_sort(data, 0, size-2);
}


/** @brief    ��������������
  * @param[in]  T : Ԫ�����ͣ�����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[in]  f : ��������(C++����)����ָ��(C����)
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T, typename Func>
void quick_sort (T data[], int frist, int last, Func& f)
{
    int lower=frist+1;
    int upper=last;
    int t=rand()%(last-frist)+frist;
    std::swap(data[frist], data[t]);
    T& bound=data[frist];
    while (lower<=upper)
    {
        while (f(data[lower],bound))
            ++lower;
        while (f(bound,data[upper]))
            --upper;
        if (lower<upper)
        {
            std::swap(data[lower], data[upper]);
            ++lower;
            --upper;
        }
        else
            ++lower;
    }
    std::swap(data[upper], data[frist]);
    if (frist<upper-1)
        quick_sort(data, frist, upper-1, f);
    if (upper+1<last)
        quick_sort(data, upper+1, last, f);
}

/** @brief	  ��������
  * @param[in]	T : Ԫ�����ͣ�����֧��С�ڲ���
  * @param[in]	data : ����������, data != NULL
  * @param[in]	size : �����������С, size > 0
  * @param[in]	f : ��������(C++����)����ָ��(C����)
  * @param[out]  data���ǽ�������
  * @return  
  */

template <typename T, typename Func>
void QuickSort (T data[], int size, Func f)
{
    int i, max;
    if (size<2)
        return;
    for (i=1, max=0; i<size; ++i) 
    {
        if (f(data[max],data[i]))
            max=i;
    }
    std::swap(data[size-1], data[max]);
    srand(time(0));
    quick_sort(data, 0, size-2, f);
}

bool cmp(int a, int b)
{
    return a<b;
}


int main(void)
{
    int i;
    int arr[] = {10, 9, 8, 4, 5, 7, 6, 3, 1, 4};
	
    //QuickSort(arr, 10);
	QuickSort(arr, 10, cmp);
	
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}

