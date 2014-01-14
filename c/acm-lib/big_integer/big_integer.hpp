      
/** @file big_integer.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    大整数处理接口
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

//大整数类
class BigInteger
{
public:
    //默认构造
    explicit BigInteger(const std::string& str="0") : m_str(str) {Normal();}
    //接受双精度浮点构造
    explicit BigInteger(double d)
    {
        m_str=ToString(d);
        Normal();
    }
    //接受单精度浮点构造
    explicit BigInteger(float f)
    {
        m_str=ToString(f);
        Normal();
    }
    //接受整数构造
    explicit BigInteger(int i)
    {
        m_str=ToString(i);
        Normal();
    }
    //输入流运算
    friend std::ostream& operator<< (std::ostream& out, const BigInteger& big);
    //输出流运算
    friend std::istream& operator>> (std::istream& in, BigInteger& big);
    //赋值
    BigInteger& operator= (const BigInteger& rbs);
    BigInteger& operator= (double d);
    BigInteger& operator= (float f);
    BigInteger& operator= (int i);
    //高精度乘法
    BigInteger operator* (const BigInteger& other)
    { return signMultiply(*this, other);}
    //高精度加法
    BigInteger operator +(const BigInteger& other)
    { return signAdd(*this, other);}
    //高精度减法
    BigInteger operator- (const BigInteger& other)
    { return signMinuse(*this, other);}
    //高精度乘方
    BigInteger operator ^(int n)
    { return BigInteger(pow(m_str, n));}
    //高精度正数取模
    BigInteger operator% (const BigInteger& other);
    //比较
    bool operator< (const BigInteger& rbs)
    { return signCompare(*this, rbs)==-1; }
    bool operator== (const BigInteger& rbs)
    { return signCompare(*this, rbs)==0; }
    //转换成字符串
    std::string ToString();
private:
    //规范化
    void Normal();
    void Unnormal();
    //转换
    std::string ToString(int n);
    std::string ToString(double n);
    std::string ToString(float n);
    //有符号高精度运算及其比较
    BigInteger signMultiply(const BigInteger& l, const BigInteger& r);
    BigInteger signAdd(const BigInteger& l, const BigInteger& r);
    BigInteger signMinuse(const BigInteger& l, const BigInteger& r);
    BigInteger signPow(const BigInteger& x, int n);
    int signCompare(const BigInteger& a, const BigInteger& b);
    //无符号比较
    int Compare(const std::string& a, const std::string& b);
    //无符号高精度运算
    std::string MultiplyEx (std::string s, std::string t);
    std::string Multiply (std::string lbs, std::string rbs);
    std::string AddEx(std::string a, std::string b);
    std::string Add(std::string lbs, std::string rbs);
    std::string MinusEx(std::string a, std::string b, bool& sign);
    std::string Minuss (std::string lbs, std::string rbs, bool& sign);
    std::string pow(const std::string& b, int n);
    std::string mod(std::string s, const std::string& t);
    //判断s是否全为零
    bool isZero(const std::string& s);
private:
    bool m_sign;       //符号
    std::string m_str; //内部字符串
};

#endif // BIG_INTEGER_HPP_

