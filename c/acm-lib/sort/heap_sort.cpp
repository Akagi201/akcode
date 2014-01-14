      
/** @file heap_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    堆排序
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

/** @brief    堆排序辅助过程
  * @param[in]  data : 待排序的数组, data != NULL
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

/** @brief	堆排序
  * @param[in] T : 必须支持小于操作
  * @param[in] data : 待排序的数组, data != NULL
  * @param[in] size : 待排序数组大小, size > 0
  * @param[out]  data按非降序排列
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

/** @brief	堆排序辅助过程
  * @param[in]  data : 待排序的数组, data != NULL
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

/** @brief	堆排序
  * @param[in]  T : 元素类型
  * @param[in] Func : 函数对象(属于C++)或函数指针(C也有)
  * @param[in] data : 待排序数组，data != NULL
  * @param[in] size : 待排序数组大小，size > 0
  * @param[in] f : 函数对象(C++才有)或函数指针(C就有)
  * @param[out]  data : 按f排序
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



