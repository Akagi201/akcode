      
/** @file binary_search_tree.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    普通二叉搜素树接口
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

#ifndef BINARY_SEARCH_TREE_HPP_
#define BINARY_SEARCH_TREE_HPP_

#include <stddef.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

//---------- begin class BinarySearchTree definition ----------//
template <typename Key, typename Value>
class BinarySearchTree
{
private:
    //二叉搜索树节点类型
    struct BSTnode
    {
        const Key m_key;      //比较关键字
        Value m_value;         //映射类型
        BSTnode* m_left;      //左子树
        BSTnode* m_right;     //右子树
        BSTnode* m_parent;    //双亲节点
        BSTnode (const Key& k, const Value& v);
        //撤销结构
        void Destroy ();
    private:
        ~BSTnode ();  //保证只能在堆上创建对象
    }; //end of struct BSTnode
public:
    //二叉搜索树访问引用类型
    class Node
    {
    public:
        explicit Node (typename BinarySearchTree<Key, Value>::BSTnode* target=NULL);
        bool IsNull () const;   //判断节点是否为空
        bool IsRoot () const;   //判断节点是否为根
        Node GetLeft () const;  //返回左子树
        Node GetRight () const; //返回右子树
        Node GetParent () const;//返回父节点
        void MoveLeft ();       //移动到左子树
        void MoveRight ();      //移动到右子树
        void MoveParent ();     //移动到双亲
        const Key& GetKey () const;      //返回当前节点的键值
        void SetValue (const Value& v);  //设置当前节点数据
        Value& GetValue () const;         //返回当前节点数据
        BSTnode* getSelf () const        //算法实现内部调用不要操作此函数
        {
            return m_subTree;
        }
    private:
        //引用的子树指针
        typename BinarySearchTree<Key, Value>::BSTnode* m_subTree;
    }; //end of class Node
public:
    //构建操作
    BinarySearchTree ();
    BinarySearchTree (const BinarySearchTree<Key, Value>& other);
    BinarySearchTree<Key, Value>& operator= (const BinarySearchTree<Key, Value>& other);
    //拆除操作
    ~BinarySearchTree ();
    //变异性操作
    void InsertItem (const Key& k, const Value& v); //插入新的元素
    void DeleteItem (const Key& k);                 //删除特定的元素
    bool InsertSubTree (typename BinarySearchTree<Key, Value>::Node& subTree); //插入子树
    void DeleteSubTree (typename BinarySearchTree<Key, Value>::Node& subTree); //删除子树
    //复制一颗新子树
    BinarySearchTree<Key, Value> CopySubTree (typename BinarySearchTree<Key, Value>::Node& subTree);
    //访问性操作
    Node GetRoot () const;    //获得根访问对象
    Node Find (const Key& k); //查找特定元素
    //返回树的高度
    unsigned int GetDepth (typename BinarySearchTree<Key, Value>::Node& subTree);
    //返回叶子节点个数
    void CountLeaf (typename BinarySearchTree<Key, Value>::Node& subTree, int& outSum);
    //遍历操作前、中、后序遍历
    template <typename Visit>
        void PreOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
    template <typename Visit>
        void InOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
    template <typename Visit>
        void PostOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
private:
    //内部辅助实现
    static typename BinarySearchTree<Key, Value>::BSTnode* CreateNode (const Key& k, const Value& v);
    static void DeleteNode (typename BinarySearchTree<Key, Value>::BSTnode* n);
    typename BinarySearchTree<Key, Value>::BSTnode* copySubTree (typename BinarySearchTree<Key, Value>::BSTnode* subtree);
    void deleteSubTree (typename BinarySearchTree<Key, Value>::BSTnode* subtree);
    //travese
    template <typename Visit>
        void InOrder (BSTnode* subtree, Visit& visitor);
    template <typename Visit>
        void PreOrder (BSTnode* subtree, Visit& visitor);
    template <typename Visit>
        void PostOrder (BSTnode* subtree, Visit& visitor);
    //depth
    unsigned int GetDepth (typename BinarySearchTree<Key, Value>::BSTnode* subtree);
    //leaf
    void CountLeaf (typename BinarySearchTree<Key, Value>::BSTnode* subtree, int& outSum);
private:
    typename BinarySearchTree<Key, Value>::BSTnode* m_root;  //树根指针
}; //end of class BinarySearchTree
//----------end class BinarySearchTree definition ----------//

#endif // BINARY_SEARCH_TREE_HPP_

