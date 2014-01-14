      
/** @file union_hash_set.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    集合哈希表接口
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

  //集合哈希表
  template <typename Key,  //键值类型
  typename Func, //键值类型到位置的映射仿函数类型
  template< typename, typename> class ManagerPolicy > //冲突避免策略
  class Hash_Set : protected ManagerPolicy<Key, Func>
  {
  public:
	  explicit Hash_Set (int size);  //初始化散列表大小
	  int Count() const;  //统计已经散列的元素个数
	  void Clear(); 	  //清空散列表
	  bool Insert(const Key& k);  //插入关键值
	  bool Lookup(const Key& k);  //找特定关键值是否已经映射
  protected:
	  //不可访问函数
	  Hash_Set (const Hash_Set& other);
	  Hash_Set& operator= (const Hash_Set& rbs);
  };


#endif // UNION_HASH_SET_HPP_
