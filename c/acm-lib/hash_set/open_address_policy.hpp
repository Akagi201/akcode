      
/** @file open_address_policy.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    开放定址策略哈希表接口
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

#ifndef OPEN_ADDRESS_POLICY_HPP_
#define OPEN_ADDRESS_POLICY_HPP_

//开放定址策略
template <typename Key,  //键值类型
typename Func> //必须是仿函数类
class OpenAddressPolicy
{
public:
	OpenAddressPolicy();
	~OpenAddressPolicy();
	void Init(int size);  //初始化散列表大小
	void Clear();         //清空散列表
	int Count() const;    //计算当前散列表中元素个数
	bool Insert (const Key& k);  //插入键值
	bool Lookup (const Key& k);  //查找是否存在键值
private:
	//用于开放定址策略的数据结构
	Key* m_pData;   //一个存放键值的数组
	bool* m_bUsage; //一个标志对应位置是否使用的数组
	int m_nSize;    //散列表大小
};


#endif // OPEN_ADDRESS_POLICY_HPP_


