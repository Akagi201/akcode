      
/** @file union_hash_set.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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
  #include "union_hash_set.hpp"
	  template <typename Key,typename Func,template< typename, typename> class ManagerPolicy >
	  Hash_Set<Key, Func, ManagerPolicy>::Hash_Set(int size)
	  {
		  ManagerPolicy<Key, Func>::Init(size);
	  }
	  
	  template <typename Key,typename Func,template< typename, typename> class ManagerPolicy >
	  int Hash_Set<Key, Func, ManagerPolicy>::Count() const
	  {
		  return ManagerPolicy<Key, Func>::Count();
	  }
	  
	  template <typename Key,typename Func,template< typename, typename> class ManagerPolicy >
	  void Hash_Set<Key, Func, ManagerPolicy>::Clear()
	  {
		  ManagerPolicy<Key, Func>::Clear();
	  }
	  
	  template <typename Key,typename Func,template< typename, typename> class ManagerPolicy >
	  bool Hash_Set<Key, Func, ManagerPolicy>::Insert(const Key &k)
	  {
		  return ManagerPolicy<Key, Func>::Insert(k);
	  }
	  
	  template <typename Key,typename Func,template< typename, typename> class ManagerPolicy >
	  bool Hash_Set<Key, Func, ManagerPolicy>::Lookup(const Key& k)
	  {
		  return ManagerPolicy<Key, Func>::Lookup(k);
	  }

