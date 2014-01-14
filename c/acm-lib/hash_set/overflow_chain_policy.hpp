      
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

//���������
template <typename Key,    //��ֵ����
typename Func>   //�����Ƿº�����
class OverflowChainPolicy
{
protected:
	//�ڲ����ݽṹ
	struct Node  //Ͱ
	{
		Key m_k;      //��ֵ
		Node* m_next; //��һ��Ͱ
	};
	struct Chain //��
	{
		Chain (bool use=false, Node* p=NULL ):m_bUsage(use), m_list(p) {}
		bool m_bUsage;  //��λ�Ƿ�ʹ�ñ�־
		Node* m_list;   //Ͱ��
	};
	void DestroyAllNode(); //ɾ������Ͱ�ڵ�
public:
	OverflowChainPolicy();   
	~OverflowChainPolicy(); 
	void Init(int size);       //��ʼ��ɢ�б��С
	void Clear();              //���ɢ�б�
	int Count() const;         //���㵱ǰɢ�б���Ԫ�ظ���
	bool Insert (const Key& k);  //�����ֵ
	bool Lookup (const Key& k);  //�����Ƿ���ڼ�ֵ
private:
	//������������Ե����ݽṹ
	Chain* m_pData; //��
	int m_nSize;    //ɢ�б��С
};



#endif // OVERFLOW_CHAIN_POLICY_HPP_

