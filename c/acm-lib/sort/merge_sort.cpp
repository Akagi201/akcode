      
/** @file merge_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    归并排序
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

/** @brief    归并排序
  * @param[in]  T : 必须支持小于操作
  * @param[in]  data : 待排序数组, data != NULL
  * @param[in]  size : 待排序数组大小, size > 0
  * @param[out]  data按非降序排列
  * @return  
  */
template <typename T>
void MergeSort (T data[], int size)
{
    if ( size>1 )
    {
        //预处理
        int mid=size/2;
        int numOfleft=mid;
        int numOfright=size-mid;
        T* left=new T[numOfleft];
        T* right=new T[numOfright];
        //分
        std::copy(data, data+numOfleft, left);
        std::copy(data+numOfleft, data+size, right);
        MergeSort(left, numOfleft);
        MergeSort(right, numOfright);
        //合
        std::merge(left, left+numOfleft, right, right+numOfright, data);
        //清理
        delete[] left;
        delete[] right;
    }
}

/** @brief    归并排序
  * @param[in]  T : 元素类型
  * @param[in] Func : 函数对象(属于C++)或函数指针(C也有)
  * @param[in] data : 待排序数组，data != NULL
  * @param[in] size : 待排序数组大小，size > 0
  * @param[in] f : 函数对象(C++才有)或函数指针(C就有)
  * @param[out]  data : 按f排序
  * @return  
  */

template <typename T, typename Func>
void MergeSort (T data[], int size, Func f)
{
    if ( size>1 )
    {
        int mid=size/2;
        int numOfleft=mid;
        int numOfright=size-mid;
        T* left=new T[numOfleft];
        T* right=new T[numOfright];
        std::copy(data, data+numOfleft, left);
        std::copy(data+numOfleft, data+size, right);
        MergeSort(left, numOfleft, f);
        MergeSort(right, numOfright, f);
        std::merge(left, left+numOfleft, right, right+numOfright, data, f);
        delete[] left;
        delete[] right;
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
    //MergeSort(arr, 10);
    MergeSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}

