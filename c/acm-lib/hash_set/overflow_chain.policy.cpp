      
/** @file overflow_chain_policy.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��������ԵĹ�ϣ��ʵ��
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
		DestroyAllNode();  //��ɾ��Ͱ�ڵ�
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
		if ( m_pData[i].m_bUsage ) //����ӳ��
		{
			p=m_pData[i].m_list;
			//ɾ�����й���Ͱ
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
		if ( m_pData[i].m_bUsage ) //����ӳ��
		{
			p=m_pData[i].m_list;
			//ͳ��Ͱ������ӳ���Ԫ�ظ���
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
	int pos=Func()(k);  //���ӳ���ַ
	if ( m_pData[pos].m_bUsage ) //��ͻ
	{
		Node* p=new Node;
		p->m_k=k;
		p->m_next=m_pData[pos].m_list;
		m_pData[pos].m_list=p;
	}
	else //û�г�ͻ
	{
		m_pData[pos].m_bUsage=true;
		Node* p=new Node;
		p->m_k=k;
		p->m_next=NULL;
		m_pData[pos].m_list=p;
	}
	return true; //�ܻ�ɹ�
}

template <typename Key, typename Func>
bool OverflowChainPolicy<Key, Func>::Lookup(const Key &k)
{
	int pos=Func()(k); //���ӳ��λ��
	if ( m_pData[pos].m_bUsage )  //����ӳ��
	{
		Node* p=m_pData[pos].m_list;
		//�ڹ���Ͱ���������ض���ֵ
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

