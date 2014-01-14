      
/** @file merge_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �鲢����
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

/** @brief    �鲢����
  * @param[in]  T : ����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T>
void MergeSort (T data[], int size)
{
    if ( size>1 )
    {
        //Ԥ����
        int mid=size/2;
        int numOfleft=mid;
        int numOfright=size-mid;
        T* left=new T[numOfleft];
        T* right=new T[numOfright];
        //��
        std::copy(data, data+numOfleft, left);
        std::copy(data+numOfleft, data+size, right);
        MergeSort(left, numOfleft);
        MergeSort(right, numOfright);
        //��
        std::merge(left, left+numOfleft, right, right+numOfright, data);
        //����
        delete[] left;
        delete[] right;
    }
}

/** @brief    �鲢����
  * @param[in]  T : Ԫ������
  * @param[in] Func : ��������(����C++)����ָ��(CҲ��)
  * @param[in] data : ���������飬data != NULL
  * @param[in] size : �����������С��size > 0
  * @param[in] f : ��������(C++����)����ָ��(C����)
  * @param[out]  data : ��f����
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

