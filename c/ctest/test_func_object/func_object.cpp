      
/** @file func_object.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ���Ժ�������
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

enum Relations {equal,not_equal,less,greater,less_equal,greater_equal};//��������޼��ϳ���,�ο���һ��"  enum���͵ı���".
class FunctionObject
{
    int value;
    Relations rel;
public:
    FunctionObject(int v,Relations r):value(v),rel(r)
    {}
    bool operator()(int n)
    {
        switch(rel)
        {
        case equal:
            return n==value;

        case not_equal:
            return n!=value;

        case less:
            return n<value;

        case greater:
            return n>value;

        case less_equal:
            return n<=value;

        case greater_equal:
            return n>=value;

        default:
            return 1;
        }
    }
};

void print_if(const int *arr,int len,FunctionObject con)
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

int main()
{
    int a[10]={0,1,2,35,46,4,8,0,6,0};
    print_if(a,10,FunctionObject(5,less));
    print_if(a,10,FunctionObject(10,greater));
    print_if(a,10,FunctionObject(0,not_equal));

    return 0;
}

