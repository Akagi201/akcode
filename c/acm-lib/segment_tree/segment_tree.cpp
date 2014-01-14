      
/** @file segment_tree.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    线段树实现
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

LS tree[SIZE]={0}; //存储空间
int total; //用到的空间

void LS::Create(int l, int r)
{
    nl=l; nr=r;
    //元线段
    if ( l+1==r )
    {
        left=NULL;
        right=NULL;
        return;
    }
    int mid=(l+r)/2; //中点
    left=&tree[total++];
    right=&tree[total++];
    //左子树
    left->Create(l, mid);
    //右子树
    right->Create(mid, r);
}

void LS::Insert(int l, int r)
{
    if ( l==nl && nr==r )  //包含
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

        if ( r<=mid ) //左边
        {
            left->Insert(l, r);
        }
        else if ( l>=mid ) //右边
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
    if ( l==nl && nr==r )  //包含
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

        if ( r<=mid ) //左边
        {
            left->Delete(l, r);
        }
        else if ( l>=mid ) //右边
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

