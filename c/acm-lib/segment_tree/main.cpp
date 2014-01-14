      
/** @file main.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    测试线段树功能函数
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

#include "segment_tree.hpp"

int main(int argc, char* argv[])
{
    total=1;
    tree[0].Create(0, 10);
    tree[0].Insert(4, 6);
    tree[0].Delete(4, 6);
    for ( int i=0; i<total; ++i )
    {
        std::cout<<tree[i].nl<<' '<<tree[i].nr<<' '<<tree[i].cover<<'\n';
    }
    return 0;
}


