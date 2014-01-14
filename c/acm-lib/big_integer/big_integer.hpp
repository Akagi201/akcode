      
/** @file big_integer.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ����������ӿ�
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

#ifndef BIG_INTEGER_HPP_
#define BIG_INTEGER_HPP_

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <algorithm>

//��������
class BigInteger
{
public:
    //Ĭ�Ϲ���
    explicit BigInteger(const std::string& str="0") : m_str(str) {Normal();}
    //����˫���ȸ��㹹��
    explicit BigInteger(double d)
    {
        m_str=ToString(d);
        Normal();
    }
    //���ܵ����ȸ��㹹��
    explicit BigInteger(float f)
    {
        m_str=ToString(f);
        Normal();
    }
    //������������
    explicit BigInteger(int i)
    {
        m_str=ToString(i);
        Normal();
    }
    //����������
    friend std::ostream& operator<< (std::ostream& out, const BigInteger& big);
    //���������
    friend std::istream& operator>> (std::istream& in, BigInteger& big);
    //��ֵ
    BigInteger& operator= (const BigInteger& rbs);
    BigInteger& operator= (double d);
    BigInteger& operator= (float f);
    BigInteger& operator= (int i);
    //�߾��ȳ˷�
    BigInteger operator* (const BigInteger& other)
    { return signMultiply(*this, other);}
    //�߾��ȼӷ�
    BigInteger operator +(const BigInteger& other)
    { return signAdd(*this, other);}
    //�߾��ȼ���
    BigInteger operator- (const BigInteger& other)
    { return signMinuse(*this, other);}
    //�߾��ȳ˷�
    BigInteger operator ^(int n)
    { return BigInteger(pow(m_str, n));}
    //�߾�������ȡģ
    BigInteger operator% (const BigInteger& other);
    //�Ƚ�
    bool operator< (const BigInteger& rbs)
    { return signCompare(*this, rbs)==-1; }
    bool operator== (const BigInteger& rbs)
    { return signCompare(*this, rbs)==0; }
    //ת�����ַ���
    std::string ToString();
private:
    //�淶��
    void Normal();
    void Unnormal();
    //ת��
    std::string ToString(int n);
    std::string ToString(double n);
    std::string ToString(float n);
    //�з��Ÿ߾������㼰��Ƚ�
    BigInteger signMultiply(const BigInteger& l, const BigInteger& r);
    BigInteger signAdd(const BigInteger& l, const BigInteger& r);
    BigInteger signMinuse(const BigInteger& l, const BigInteger& r);
    BigInteger signPow(const BigInteger& x, int n);
    int signCompare(const BigInteger& a, const BigInteger& b);
    //�޷��űȽ�
    int Compare(const std::string& a, const std::string& b);
    //�޷��Ÿ߾�������
    std::string MultiplyEx (std::string s, std::string t);
    std::string Multiply (std::string lbs, std::string rbs);
    std::string AddEx(std::string a, std::string b);
    std::string Add(std::string lbs, std::string rbs);
    std::string MinusEx(std::string a, std::string b, bool& sign);
    std::string Minuss (std::string lbs, std::string rbs, bool& sign);
    std::string pow(const std::string& b, int n);
    std::string mod(std::string s, const std::string& t);
    //�ж�s�Ƿ�ȫΪ��
    bool isZero(const std::string& s);
private:
    bool m_sign;       //����
    std::string m_str; //�ڲ��ַ���
};

#endif // BIG_INTEGER_HPP_

