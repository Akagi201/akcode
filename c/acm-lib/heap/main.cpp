      
/** @file main.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ²âÊÔ¶Ñ¹¦ÄÜº¯Êý
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
#include "heap.hpp"

class cmp
{
public:
	bool operator()(const int& a, const int& b)
	{ return a<b;}
};


int main()
{
	Heap<int, cmp> heap(100);
	int i;
	for (i=100; i>0; --i)
		{
		heap.insertItem(i);
		}
	while (!heap.isEmpty())
		{
		std::cout<<heap.deleteItem()<<" ";
		}

	return 0;
}

