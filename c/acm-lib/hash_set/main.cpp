      
/** @file main.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ¹þÏ£±í²âÊÔº¯Êý
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
#include <iostream>   //cout, cin
#include <iterator>   //ostream_iterator
#include <stdlib.h>   //srand rand
#include <time.h>     //time

#include "union_hash_set.hpp"

class Cmp1
{
public:
	int operator() (int k)
	{
		return k%101;
	}
};


int main()
{
	Hash_Set<int, Cmp1, OpenAddressPolicy> hs(101);
	int x;
	bool ret;
	for ( int i=0; i<101; ++i )
	{
		x=rand()%RANGE;
		ret=hs.Insert(x);
	}
	std::cout<<hs.Count()<<"\n";

	
	return 0;
}
