      
/** @file test_func_pointer.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试函数指针的情况
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

#include<iostream>

using std::cout;
using std::endl;

void print_if(const int * arr, int len, bool (*con)(int))
{
    for(int i=0;i<len;++i)
    {
        if(con(arr[i]))
        {
            cout<<arr[i]<<" ";
        }
    }
    cout<<endl;
}

bool less_5(int n)
{
    return n < 5;
}

bool greater_10(int n)
{
    return n > 10;
}

bool not_equal_0(int n)
{
    return n != 0;
}

int main()
{
    int a[10] = {0, 1, 2, 35, 46, 4, 8, 0, 6, 0};
    print_if(a, 10, less_5);  //print_if(a, 10, &less_5); 也能通过
    print_if(a, 10, greater_10);
    print_if(a, 10, not_equal_0);
    return 0;
}

