      
/** @file segment_tree.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �߶����ӿ�
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

#ifndef SEGMENT_TREE_HPP_
#define SEGMENT_TREE_HPP_

#include <iostream>

//�߶����ṹ
struct  LS
{
    int nl;      //�½�
    int nr;      //�Ͻ�
    int cover;   //��ǰ�߶θ��Ǵ���
    int m;       //���
    LS* left;    //����
    LS* right;   //�Һ���
    //�����߶���
    void Create(int l, int r);
    //�����߶�
    void Insert(int l, int r);
    //ɾ���߶�
    void Delete(int l, int r);
};

#define SIZE 10000
extern LS tree[SIZE]; //�洢�ռ�
extern int total;     //�õ��Ŀռ�

#endif // SEGMENT_TREE_HPP_

