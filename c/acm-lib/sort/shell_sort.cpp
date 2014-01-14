      
/** @file shell_sort.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ϣ������
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

/** @brief	 ϣ������
  * @param[in]  T : ����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]	data���ǽ�������
  * @return	
  */

template <typename T>
void ShellSort (T data[], int size)
{
    int i, j, hCnt, h;
    int array[20], k;
    T temp;
    for (h=1, i=0; h<size; ++i)
    {
        array[i]=h;
        h=3*h+1;
    }
    for (i--; i>=0; --i)
    {
        h=array[i];
        for (hCnt=h; hCnt<2*h; ++hCnt) 
        {
            for (j=hCnt; j<size; )
            {
                temp=data[j];
                k=j;
                while (k-h>=0 && temp<data[k-h])
                {
                    data[k]=data[k-h];
                    k-=h;
                }
                data[k]=temp;
                j+=h;
            }
        }
    }
}

/** @brief    ϣ������
  * @param[in]  T : Ԫ�����ͣ�����֧��С�ڲ���
  * @param[in]  data : ����������, data != NULL
  * @param[in]  size : �����������С, size > 0
  * @param[out]  data���ǽ�������
  * @return  
  */
template <typename T, typename Func>
void ShellSort (T data[], int size, Func f)
{
    int i, j, hCnt, h;
    int array[20], k;
    T temp;
    for (h=1, i=0; h<size; ++i)
    {
        array[i]=h;
        h=3*h+1;
    }
    for (i--; i>=0; --i)
    {
        h=array[i];
        for (hCnt=h; hCnt<2*h; ++hCnt) 
        {
            for (j=hCnt; j<size; )
            {
                temp=data[j];
                k=j;
                while (k-h>=0 && f(temp,data[k-h]))
                {
                    data[k]=data[k-h];
                    k-=h;
                }
                data[k]=temp;
                j+=h;
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
    //ShellSort(arr, 10);
    ShellSort(arr, 10, cmp);
    for (i = 0; i < 10; ++i)
    {
        printf("%d ", *(arr+i));
    }
    printf("\n");
    return 0;
}

