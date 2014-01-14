      
/** @file heap.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    堆的接口文件
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

#ifndef HEAP_HPP_
#define HEAP_HPP_

#include <algorithm>
#include <functional>

template <typename T, typename Compare=std::less<T> >
class Heap
{
public:
	explicit Heap (int size);
	~Heap ();
	const T& operator[] (int pos)
	{ return m_pdata[pos]; }
	int getSize () const
	{ return curr_size; }
	bool isEmpty() const
	{ return curr_size==0; }
	bool isFull () const
	{ return curr_size==max_size; }
	void insertItem (const T& item);
	T deleteItem ();
	void clear ()
	{ curr_size=0; }
private:
	T* m_pdata;
	Compare m_f;
	int max_size;
	int curr_size;
	void FilterDown (int i);
	void FilterUp (int i);
};

#endif // HEAP_HPP_

