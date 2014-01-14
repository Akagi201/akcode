      
/** @file union_hash_set.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ���Ϲ�ϣ��ӿ�
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

#ifndef UNION_HASH_SET_HPP_
#define UNION_HASH_SET_HPP_
#include "open_address_policy.hpp"
#include "overflow_chain_policy.hpp"

  //���Ϲ�ϣ��
  template <typename Key,  //��ֵ����
  typename Func, //��ֵ���͵�λ�õ�ӳ��º�������
  template< typename, typename> class ManagerPolicy > //��ͻ�������
  class Hash_Set : protected ManagerPolicy<Key, Func>
  {
  public:
	  explicit Hash_Set (int size);  //��ʼ��ɢ�б��С
	  int Count() const;  //ͳ���Ѿ�ɢ�е�Ԫ�ظ���
	  void Clear(); 	  //���ɢ�б�
	  bool Insert(const Key& k);  //����ؼ�ֵ
	  bool Lookup(const Key& k);  //���ض��ؼ�ֵ�Ƿ��Ѿ�ӳ��
  protected:
	  //���ɷ��ʺ���
	  Hash_Set (const Hash_Set& other);
	  Hash_Set& operator= (const Hash_Set& rbs);
  };


#endif // UNION_HASH_SET_HPP_
