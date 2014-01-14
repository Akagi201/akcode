      
/** @file segment_tree.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    �߶���ʵ��
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

#include "segment_tree.hpp"

LS tree[SIZE]={0}; //�洢�ռ�
int total; //�õ��Ŀռ�

void LS::Create(int l, int r)
{
    nl=l; nr=r;
    //Ԫ�߶�
    if ( l+1==r )
    {
        left=NULL;
        right=NULL;
        return;
    }
    int mid=(l+r)/2; //�е�
    left=&tree[total++];
    right=&tree[total++];
    //������
    left->Create(l, mid);
    //������
    right->Create(mid, r);
}

void LS::Insert(int l, int r)
{
    if ( l==nl && nr==r )  //����
    {
        ++cover;
    }
    else
    {
        if ( left==NULL && right==NULL )
        {
            return;
        }
        int mid=(nl+nr)/2;

        if ( r<=mid ) //���
        {
            left->Insert(l, r);
        }
        else if ( l>=mid ) //�ұ�
        {
            right->Insert(l, r);
        }
        else
        {
            left->Insert(l, mid);
            right->Insert(mid, r);
        }
    }
}

void LS::Delete(int l, int r)
{
    if ( l==nl && nr==r )  //����
    {
        --cover;
    }
    else
    {
        if ( left==NULL && right==NULL )
        {
            return;
        }
        int mid=(nl+nr)/2;

        if ( r<=mid ) //���
        {
            left->Delete(l, r);
        }
        else if ( l>=mid ) //�ұ�
        {
            right->Delete(l, r);
        }
        else
        {
            left->Delete(l, mid);
            right->Delete(mid, r);
        }
    }
}

