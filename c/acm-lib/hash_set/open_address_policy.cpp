      
/** @file open_address_policy.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ϣ���ʵ��
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
		  //��ʹ�ñ�־дΪ��
		  for ( int i=0; i<m_nSize; ++i )
			  m_bUsage[i]=false;
	  }
	  
	  template <typename Key, typename Func>
	  int OpenAddressPolicy<Key, Func>::Count() const
	  {
		  int count=0;
		  //ͳ��ʹ��λ�ø���
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
		  int pos=Func()(k); //�õ�ɢ��λ��
		  if ( m_bUsage[pos] ) //������ͻ
		  {
			  int i=pos+1; //��ͻ��һ��λ��
			  //����ҵ�һ��û��ʹ�õ�λ��
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
			  //�ڳ�ͻλ��ǰ�ҵ�һ��û��ʹ�õ�λ��
			  for ( i=0; i<pos; ++i )
			  {
				  if ( !m_bUsage[i] )
				  {
					  m_bUsage[i]=true;
					  m_pData[i]=k;
					  return true;
				  }
			  }
			  //�޷��ҵ���λ��
			  return false;
		  }
		  else //û�г�ͻ
		  {
			  m_bUsage[pos]=true;
			  m_pData[pos]=k;
		  }
		  return true;
	  }
	  
	  template <typename Key, typename Func>
	  bool OpenAddressPolicy<Key, Func>::Lookup(const Key &k)
	  {
		  int pos=Func()(k); //�õ�ɢ��λ��
		  if ( m_bUsage[pos] )	//���ܴ��ڻ��ͻ
		  {
			  if ( m_pData[pos]==k ) //ֱ��ӳ��ɹ�
				  return true;
			  else //���ܳ�ͻ
			  {
				  int i;
				  //������ͻλ���Ժ�
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
				  //������ͻλ��֮ǰ
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

