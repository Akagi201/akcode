      
/** @file open_address_policy.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ���Ŷ�ַ���Թ�ϣ��ӿ�
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

//���Ŷ�ַ����
template <typename Key,  //��ֵ����
typename Func> //�����Ƿº�����
class OpenAddressPolicy
{
public:
	OpenAddressPolicy();
	~OpenAddressPolicy();
	void Init(int size);  //��ʼ��ɢ�б��С
	void Clear();         //���ɢ�б�
	int Count() const;    //���㵱ǰɢ�б���Ԫ�ظ���
	bool Insert (const Key& k);  //�����ֵ
	bool Lookup (const Key& k);  //�����Ƿ���ڼ�ֵ
private:
	//���ڿ��Ŷ�ַ���Ե����ݽṹ
	Key* m_pData;   //һ����ż�ֵ������
	bool* m_bUsage; //һ����־��Ӧλ���Ƿ�ʹ�õ�����
	int m_nSize;    //ɢ�б��С
};


#endif // OPEN_ADDRESS_POLICY_HPP_


