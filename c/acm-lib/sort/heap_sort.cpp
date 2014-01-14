      
/** @file heap_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ������
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

#include <iostream>
#include <stdio.h>
#include <algorithm>

/** @brief    ������������
  * @param[in]  data : �����������, data != NULL
  * @param[in]  size :
  * @param[in]  i :
  * @param[out]  
  * @return  
  */
template <typename T>
void heap_down (T data[], int i, const int& size)
{
    int p=i*2+1;
    while ( p<size )
    {
        if ( p+1<size )
        {
            if ( data[p]<data[p+1] )
            {
                ++p;
            }
        }
        if ( data[i]<data[p] )
        {
            std::swap(data[p], data[i]);
            i=p;
            p=i*2+1;
        }
        else
        {
            break;
        }
    }
}

/** @brief	������
  * @param[in] T : ����֧��С�ڲ���
  * @param[in] data : �����������, data != NULL
  * @param[in] size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */

template <typename T>
void HeapSort (T data[], int size)
{
    int i;
    for (i=(size-1)/2; i>=0; --i)
    {
        heap_down(data, i, size);
    }
    for (i=size-1; i>0; --i)
    {
        std::swap(data[0], data[i]);
        heap_down(data, 0, i);
    }
}

/** @brief	������������
  * @param[in]  data : �����������, data != NULL
  * @param[in]  size :
  * @param[in]  i :
  * @param[out]  
  * @return  
  */

template <typename T, typename Func>
void heap_down (T data[], int i, const int& size, Func& f)
{
    int p=i*2+1;
    while ( p<size )
    {
        if ( p+1<size )
        {
            if ( f(data[p],data[p+1]) )
            {
                ++p;
            }
        }
        if ( f(data[i],data[p]) )
        {
            std::swap(data[p], data[i]);
            i=p;
            p=i*2+1;
        }
        else
        {
            break;
        }
    }
}

/** @brief	������
  * @param[in]  T : Ԫ������
  * @param[in] Func : ��������(����C++)����ָ��(CҲ��)
  * @param[in] data : ���������飬data != NULL
  * @param[in] size : �����������С��size > 0
  * @param[in] f : ��������(C++����)����ָ��(C����)
  * @param[out]  data : ��f����
  * @return  
  */

template <typename T, typename Func>
void HeapSort (T data[], int size, Func f)
{
    int i;
    for (i=(size-1)/2; i>=0; --i)
    {
        heap_down(data, i, size, f);
    }
    for (i=size-1; i>0; --i)
    {
        std::swap(data[0], data[i]);
        heap_down(data, 0, i, f);
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
    //HeapSort(arr, 10);
    HeapSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}



