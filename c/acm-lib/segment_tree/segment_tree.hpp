      
/** @file segment_tree.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    线段树接口
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

//线段树结构
struct  LS
{
    int nl;      //下界
    int nr;      //上界
    int cover;   //当前线段覆盖次数
    int m;       //侧度
    LS* left;    //左孩子
    LS* right;   //右孩子
    //构建线段树
    void Create(int l, int r);
    //插入线段
    void Insert(int l, int r);
    //删除线段
    void Delete(int l, int r);
};

#define SIZE 10000
extern LS tree[SIZE]; //存储空间
extern int total;     //用到的空间

#endif // SEGMENT_TREE_HPP_

