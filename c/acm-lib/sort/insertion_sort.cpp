
/** @file insertion_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    插入排序
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
/** @brief    插入排序
  * @param[in] T : 必须支持小于操作
  * @param[in] data : 待排序的数组, data != NULL
  * @param[in] size : 待排序数组大小, size > 0
  * @param[out]  data按非降序排列
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

/** @brief    插入排序
  * @param[in]  T : 元素类型
  * @param[in] Func : 函数对象(属于C++)或函数指针(C也有)
  * @param[in] data : 待排序数组，data != NULL
  * @param[in] size : 待排序数组大小，size > 0
  * @param[in] f : 函数对象(C++才有)或函数指针(C就有)
  * @param[out]  data : 按f排序
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
