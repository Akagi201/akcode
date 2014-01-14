      
/** @file overflow_chain_policy.hpp
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

#ifndef OVERFLOW_CHAIN_POLICY_HPP_
#define OVERFLOW_CHAIN_POLICY_HPP_

//溢出链策略
template <typename Key,    //键值类型
typename Func>   //必须是仿函数类
class OverflowChainPolicy
{
protected:
	//内部数据结构
	struct Node  //桶
	{
		Key m_k;      //键值
		Node* m_next; //下一个桶
	};
	struct Chain //链
	{
		Chain (bool use=false, Node* p=NULL ):m_bUsage(use), m_list(p) {}
		bool m_bUsage;  //该位是否使用标志
		Node* m_list;   //桶链
	};
	void DestroyAllNode(); //删除所有桶节点
public:
	OverflowChainPolicy();   
	~OverflowChainPolicy(); 
	void Init(int size);       //初始化散列表大小
	void Clear();              //清空散列表
	int Count() const;         //计算当前散列表中元素个数
	bool Insert (const Key& k);  //插入键值
	bool Lookup (const Key& k);  //查找是否存在键值
private:
	//用于溢出链策略的数据结构
	Chain* m_pData; //链
	int m_nSize;    //散列表大小
};



#endif // OVERFLOW_CHAIN_POLICY_HPP_

