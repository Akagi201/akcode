      
/** @file open_address_policy.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    哈希表的实现
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
	  template <typename Key, typename Func>
	  OpenAddressPolicy<Key, Func>::OpenAddressPolicy()
	  {
		  m_pData=NULL;
		  m_bUsage=NULL;
		  m_nSize=0;
	  }
	  
	  template <typename Key, typename Func>
	  OpenAddressPolicy<Key, Func>::~OpenAddressPolicy()
	  {
		  if ( m_pData!=NULL )
		  {
			  delete[] m_pData;
			  m_pData=NULL;
		  }
		  if ( m_bUsage!=NULL )
		  {
			  delete[] m_bUsage;
			  m_bUsage=NULL;
		  }
		  m_nSize=0;
	  }
	  
	  template <typename Key, typename Func>
	  void OpenAddressPolicy<Key, Func>::Init(int size)
	  {
		  m_pData=new Key[size];
		  m_bUsage=new bool[size];
		  m_nSize=size;
		  Clear();
	  }
	  
	  template <typename Key, typename Func>
	  void OpenAddressPolicy<Key, Func>::Clear()
	  {   
		  //将使用标志写为假
		  for ( int i=0; i<m_nSize; ++i )
			  m_bUsage[i]=false;
	  }
	  
	  template <typename Key, typename Func>
	  int OpenAddressPolicy<Key, Func>::Count() const
	  {
		  int count=0;
		  //统计使用位置个数
		  for ( int i=0; i<m_nSize; ++i )
		  {
			  if ( m_bUsage[i] )
				  ++count;
		  }
		  return count;
	  }
	  
	  template <typename Key, typename Func>
	  bool OpenAddressPolicy<Key, Func>::Insert(const Key &k)
	  {
		  int pos=Func()(k); //得到散列位置
		  if ( m_bUsage[pos] ) //发生冲突
		  {
			  int i=pos+1; //冲突后一个位置
			  //向后找第一个没有使用的位置
			  while ( i<m_nSize )
			  {
				  if ( !m_bUsage[i] )
				  {
					  m_bUsage[i]=true;
					  m_pData[i]=k;
					  return true;
				  }
				  ++i;
			  };
			  //在冲突位置前找第一个没有使用的位置
			  for ( i=0; i<pos; ++i )
			  {
				  if ( !m_bUsage[i] )
				  {
					  m_bUsage[i]=true;
					  m_pData[i]=k;
					  return true;
				  }
			  }
			  //无法找到空位置
			  return false;
		  }
		  else //没有冲突
		  {
			  m_bUsage[pos]=true;
			  m_pData[pos]=k;
		  }
		  return true;
	  }
	  
	  template <typename Key, typename Func>
	  bool OpenAddressPolicy<Key, Func>::Lookup(const Key &k)
	  {
		  int pos=Func()(k); //得到散列位置
		  if ( m_bUsage[pos] )	//可能存在或冲突
		  {
			  if ( m_pData[pos]==k ) //直接映射成功
				  return true;
			  else //可能冲突
			  {
				  int i;
				  //遍历冲突位置以后
				  for ( i=pos+1; i<m_nSize; ++i )
				  {
					  if ( m_bUsage[i] )
					  {
						  if ( m_pData[i]==k )
							  return true;
					  }
					  else
						  return false;
				  }
				  //遍历冲突位置之前
				  for ( i=0; i<pos; ++i )
				  {
					  if ( m_bUsage[i] )
					  {
						  if ( m_pData[i]==k )
							  return true;
					  }
					  else
						  return false;
				  }
			  }
		  }
		  return false;
	  }

