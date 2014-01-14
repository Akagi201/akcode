      
/** @file quick_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    随机化快排
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

/** @brief	 快速排序辅助函数
  * @param[in]  T : 必须支持小于操作
  * @param[in]  data : 待排序数组, data != NULL
  * @param[in]  first : 待排序数组的起点
  * @param[in]  lsat : 待排序数组的终点的下一个端点
  * @param[out]	data按非降序排列
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


/** @brief	  快速排序
  * @param[in]	T : 必须支持小于操作
  * @param[in]	data : 待排序数组, data != NULL
  * @param[in]	size : 待排序数组大小, size > 0
  * @param[out]  data按非降序排列
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


/** @brief    快速排序辅助函数
  * @param[in]  T : 元素类型，必须支持小于操作
  * @param[in]  data : 待排序数组, data != NULL
  * @param[in]  size : 待排序数组大小, size > 0
  * @param[in]  f : 函数对象(C++才有)或函数指针(C就有)
  * @param[out]  data按非降序排列
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

/** @brief	  快速排序
  * @param[in]	T : 元素类型，必须支持小于操作
  * @param[in]	data : 待排序数组, data != NULL
  * @param[in]	size : 待排序数组大小, size > 0
  * @param[in]	f : 函数对象(C++才有)或函数指针(C就有)
  * @param[out]  data按非降序排列
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

