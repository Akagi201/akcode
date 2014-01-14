      
/** @file heap.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    堆实现文件
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
#include "heap.hpp"

template <typename T, typename Compare >
Heap<T, Compare>::Heap(int size)
{
	max_size=size;
	curr_size=0;
	m_pdata=new T[size];
}

template <typename T, typename Compare >
Heap<T, Compare>::~Heap()
{
	delete[] m_pdata;
}

template <typename T, typename Compare >
void Heap<T, Compare>::FilterUp (int i)  
{ 
	int currentpos, parentpos;
	T target;
	currentpos=i;
	parentpos=(i-1)/2;
	target=m_pdata[i];
	while (currentpos)
	{
		if (!(m_f(m_pdata[currentpos],m_pdata[parentpos])))
			{
			break;
			}
		else
		{
			std::swap(m_pdata[parentpos], m_pdata[currentpos]);
			currentpos=parentpos;
			parentpos=(currentpos-1)/2;
		}
	}
	m_pdata[currentpos]=target;
}

template <typename T, typename Compare >
void Heap<T, Compare>::insertItem (const T& item)
{
	if (isFull())
		{
		;
		}
	else
	{
		m_pdata[curr_size]=item;
		FilterUp(curr_size);
		++curr_size;
	}
}

template <typename T, typename Compare >
void Heap<T, Compare>::FilterDown (int i)
{
	int currentpos, childpos;
	T target;
	currentpos=i;
	childpos=2*i+1;
	target=m_pdata[i];
	while (childpos<curr_size)
	{
		if ( (childpos+1<curr_size) && m_f(m_pdata[childpos+1],m_pdata[childpos]))
			{
			++childpos;
			}
		if (m_f(target,m_pdata[childpos]))
			{
			break;
			}
		else
		{
			std::swap(m_pdata[currentpos], m_pdata[childpos]);
			currentpos=childpos;
			childpos=2*currentpos+1;
		}
	}
	m_pdata[currentpos]=target;
}

template <typename T, typename Compare >
T Heap<T, Compare>::deleteItem ()
{
	T temp;
	if (isEmpty())
		{
		;
		}
	else
	{
		temp=m_pdata[0];
		m_pdata[0]=m_pdata[curr_size-1];
		--curr_size;
		FilterDown(0);
	}
	return temp;
}



