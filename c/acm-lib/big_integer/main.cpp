      
/** @file main.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试大整数处理函数
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

#include "big_integer.hpp"

int main()
{
    std::cout<<"Input any number:\n";
    BigInteger a, b;
    std::cin>>a>>b;
    std::cout<<"a*b="<<a*b<<std::endl;
    std::cout<<"a+b="<<a+b<<std::endl;
    std::cout<<"a-b="<<a-b<<std::endl;
    std::cout<<"Input one of normal positive integer number:\n";
    int n;
    std::cin>>a>>n;
    std::cout<<"a^n="<<(a^n)<<std::endl;
    std::cout<<"All integer number: \n";
    std::cin>>a>>b;
    std::cout<<"a%b="<<(a%b)<<std::endl;
    if (a<b)
    {
        std::cout<<"a less b\n";
    }
    else
    {
        std::cout<<"a not less b\n";
    }
    if (a==b)
    {
        std::cout<<"a equal b\n";
    }
    else
    {
        std::cout<<"a not equal b\n";
    }

    return 0;
}



