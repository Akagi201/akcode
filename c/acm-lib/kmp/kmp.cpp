      
/** @file kmp.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    KMP算法
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
#include <cstdio> 
#include <string> 

//计算next数组
static int* __getNextofString(const std::string& p) 
{ 
    int size=p.size(); 
    int* next=new int[size+1]; 
    int i, j; 
    i=1; next[1]=0; j=0; 
    while ( i<size ) 
    { 
        if ( j==0 || p[i-1]==p[j-1] ) 
        { 
            ++i; ++j; 
            next[i]=j; 
        } 
        else 
        {
            j=next[j]; 
        }
    } 
    return next; 
}
//查找
static int __findSubString(const std::string& t, const std::string& p, int* next, int index=1) 
{ 
    int i, j; 
    int tlen, plen; 
    tlen=t.size(); plen=p.size(); 
    i=index; j=1; 
    while ( i<=tlen && j<=plen ) 
    { 
        if ( j==0 || t[i-1]==p[j-1] ) 
        { ++i; ++j; } 
        else 
        {
            j=next[j]; 
        }
    } 
    if ( j>plen ) 
    {
        return i-plen; 
    }
    else 
    {
        return 0; 
    }
} 
//主函数
int IndexSubString(const std::string& text, const std::string& pattern, int* next=NULL, int pos=0) 
{ 
    bool bAlloc=false;
    if (next==NULL)
    {
        bAlloc=true;
        next=__getNextofString(pattern);
    }
    int ret=__findSubString(text, pattern, next, pos+1)-1; 
    if (bAlloc)
    {
        delete[] next;
    }
    return ret;
} 
//使用方法
int main (int argc, char* argv[]) 
{ 
    std::string text, pattern; 
    while ( std::cin>>text>>pattern )
    {
        std::cout<<IndexSubString(text, pattern)<<std::endl; 
    }
    return 0; 
}  


