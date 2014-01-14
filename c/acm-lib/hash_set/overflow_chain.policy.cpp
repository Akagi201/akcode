      
/** @file overflow_chain_policy.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    溢出链策略的哈希表实现
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

#include "overflow_chain_policy.hpp"

template <typename Key, typename Func>
OverflowChainPolicy<Key, Func>::OverflowChainPolicy()
{
	m_pData=NULL;
	m_nSize=0;
}

template <typename Key, typename Func>
OverflowChainPolicy<Key, Func>::~OverflowChainPolicy()
{
	if ( m_pData!=NULL )
	{
		DestroyAllNode();  //先删除桶节点
		delete[] m_pData;
		m_pData=NULL;
		m_nSize=0;
	}
}

template <typename Key, typename Func>
void OverflowChainPolicy<Key, Func>::DestroyAllNode()
{
	Node* p=NULL;
	Node* q=NULL;
	for ( int i=0; i<m_nSize; ++i )
	{
		if ( m_pData[i].m_bUsage ) //存在映射
		{
			p=m_pData[i].m_list;
			//删除所有关联桶
			while ( p!=NULL && p->m_next!=NULL )
			{
				q=p->m_next;
				p->m_next=q->m_next;
				delete q;
			}
			if ( p!=NULL )
			{
				delete p;
				m_pData[i].m_list=NULL;
				m_pData[i].m_bUsage=false;
			}
		}
	}
}

template <typename Key, typename Func>
void OverflowChainPolicy<Key, Func>::Init(int size)
{
	m_pData=new Chain[size];
	m_nSize=size;
}

template <typename Key, typename Func>
void OverflowChainPolicy<Key, Func>::Clear()
{
	DestroyAllNode();
}

template <typename Key, typename Func>
int OverflowChainPolicy<Key, Func>::Count() const
{
	int count=0;
	Node* p=NULL;
	for ( int i=0; i<m_nSize; ++i )
	{
		if ( m_pData[i].m_bUsage ) //存在映射
		{
			p=m_pData[i].m_list;
			//统计桶数，即映射后元素个数
			while ( p!=NULL )
			{
				++count;
				p=p->m_next;
			}
		}
	}
	return count;
}

template <typename Key, typename Func>
bool OverflowChainPolicy<Key, Func>::Insert(const Key& k)
{
	int pos=Func()(k);  //获得映射地址
	if ( m_pData[pos].m_bUsage ) //冲突
	{
		Node* p=new Node;
		p->m_k=k;
		p->m_next=m_pData[pos].m_list;
		m_pData[pos].m_list=p;
	}
	else //没有冲突
	{
		m_pData[pos].m_bUsage=true;
		Node* p=new Node;
		p->m_k=k;
		p->m_next=NULL;
		m_pData[pos].m_list=p;
	}
	return true; //总会成功
}

template <typename Key, typename Func>
bool OverflowChainPolicy<Key, Func>::Lookup(const Key &k)
{
	int pos=Func()(k); //获得映射位置
	if ( m_pData[pos].m_bUsage )  //存在映射
	{
		Node* p=m_pData[pos].m_list;
		//在关联桶链表中找特定键值
		while ( p!=NULL )
		{
			if ( p->m_k==k )
			{
				return true;
			}
			p=p->m_next;
		}
	}
	return false;
}

