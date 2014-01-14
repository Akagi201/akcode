      
/** @file binary_search_tree.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    普通二叉搜索树实现
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

#include "binary_search_tree.hpp"

//----------begin struct BSTnode----------//
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::BSTnode::BSTnode (const Key& k, const Value& v)
: m_key(k), m_value(v), m_left(NULL), m_right(NULL), m_parent(NULL)
{
    //do nothing else
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::BSTnode::Destroy ()
{
    delete this;
}

template <typename Key, typename Value>
inline BinarySearchTree<Key, Value>::BSTnode::~BSTnode ()
{
    //do nothing else
}
//----------end struct BSTnode----------//

//----------begin class Node----------//
template <typename Key, typename Value>
BinarySearchTree<Key, Value>::Node::Node (typename BinarySearchTree<Key, Value>::BSTnode* target)
: m_subTree(target)
{
    //do nothing else
}

template <typename Key, typename Value>
inline bool BinarySearchTree<Key, Value>::Node::IsNull () const
{
    return m_subTree==NULL;
}

template <typename Key, typename Value>
inline bool BinarySearchTree<Key, Value>::Node::IsRoot () const
{
    return m_subTree==m_root->m_left;
}

template <typename Key, typename Value>
inline typename BinarySearchTree<Key, Value>::Node BinarySearchTree<Key, Value>::Node::GetLeft () const
{
    return Node(m_subTree->m_left);
}

template <typename Key, typename Value>
inline typename BinarySearchTree<Key, Value>::Node BinarySearchTree<Key, Value>::Node::GetRight () const
{
    return Node(m_subTree->m_right);
}

template <typename Key, typename Value>
inline typename BinarySearchTree<Key, Value>::Node BinarySearchTree<Key, Value>::Node::GetParent () const
{
    return Node(m_subTree->m_parent);
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::Node::MoveLeft ()
{
    m_subTree=m_subTree->m_left;
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::Node::MoveRight ()
{
    m_subTree=m_subTree->m_right;
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::Node::MoveParent ()
{
    m_subTree=m_subTree->m_parent;
}

template <typename Key, typename Value>
inline const Key& BinarySearchTree<Key, Value>::Node::GetKey () const
{
    return m_subTree->m_key;
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::Node::SetValue (const Value& v)
{
    m_subTree->m_value=v;
}

template <typename Key, typename Value>
inline Value& BinarySearchTree<Key, Value>::Node::GetValue () const
{
    return m_subTree->m_value;
}
//----------end class Node----------//

//----------begin class BinarySearchTree ----------//
template <typename Key, typename Value>
inline typename BinarySearchTree<Key, Value>::BSTnode* BinarySearchTree<Key, Value>::CreateNode (const Key& k, const Value& v)
{
    return new BSTnode(k, v);
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::DeleteNode (typename BinarySearchTree<Key, Value>::BSTnode* n)
{
    n->Destroy();
}
// 删树
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::deleteSubTree (typename BinarySearchTree<Key, Value>::BSTnode* subtree)
{
    if (subtree!=NULL)
    {
        deleteSubTree(subtree->m_left);
        deleteSubTree(subtree->m_right);
        DeleteNode(subtree);
    }
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree ()
{
    m_root=CreateNode(Key(), Value());
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree ()
{
    deleteSubTree(m_root);
}
// 插入元素到树
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::InsertItem (const Key& k, const Value& v)
{
    BSTnode* tmp=CreateNode(k, v);
    if (m_root->m_left==NULL) 
    {
        tmp->m_parent=m_root;
        m_root->m_left=tmp;
        return;
    }
    BSTnode* root=m_root->m_left;
    BSTnode* parent;
    while (root!=NULL)
    {
        parent=root;
        if (root->m_key==k) 
        {
            root->m_value=v;
            DeleteNode(tmp);
            return;
        }
        else if (root->m_key<k) 
        {
            root=root->m_right;
        }
        else 
        {
            root=root->m_left;
        }
    }
    if (parent->m_key<k)
    {
        parent->m_right=tmp;
    }
    else
    {
        parent->m_left=tmp;
    }
    tmp->m_parent=parent;
}

template <typename Key, typename Value>
inline typename BinarySearchTree<Key,Value>::Node BinarySearchTree<Key, Value>::GetRoot () const
{
    return Node(m_root->m_left);
}

template <typename Key, typename Value>
template <typename Visit>
inline void BinarySearchTree<Key, Value>::InOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor)
{
    InOrder(subTree.getSelf(), visitor);
}

template <typename Key, typename Value>
template <typename Visit>
void BinarySearchTree<Key, Value>::InOrder (BSTnode* subtree, Visit& visitor)
{
    if (subtree!=NULL)
    {
        InOrder(subtree->m_left, visitor);
        visitor(subtree->m_value);
        InOrder(subtree->m_right, visitor);
    }
}

template <typename Key, typename Value>
template <typename Visit>
inline void BinarySearchTree<Key, Value>::PreOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor)
{
    PreOrder(subTree.getSelf(), visitor);
}

template <typename Key, typename Value>
template <typename Visit>
void BinarySearchTree<Key, Value>::PreOrder (BSTnode* subtree, Visit& visitor)
{
    if (subtree!=NULL)
    {
        visitor(subtree->m_value);
        PreOrder(subtree->m_left, visitor);
        PreOrder(subtree->m_right, visitor);
    }
}

template <typename Key, typename Value>
template <typename Visit>
inline void BinarySearchTree<Key, Value>::PostOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor)
{
    PostOrder(subTree.getSelf(), visitor);
}

template <typename Key, typename Value>
template <typename Visit>
void BinarySearchTree<Key, Value>::PostOrder (BSTnode* subtree, Visit& visitor)
{
    if (subtree!=NULL)
    {
        PostOrder(subtree->m_left, visitor);
        PostOrder(subtree->m_right, visitor);
        visitor(subtree->m_value);
    }
}
// 复制树
template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::BSTnode* BinarySearchTree<Key, Value>::copySubTree (typename BinarySearchTree<Key, Value>::BSTnode* subtree)
{
    BSTnode* left=NULL, *right=NULL, *parent=NULL;
    if (subtree==NULL) 
    {
        return NULL;
    }
    if (subtree->m_left==NULL)
    {
        left=NULL;
    }
    else
    {
        left=copySubTree(subtree->m_left);
    }
    if (subtree->m_right==NULL)
    {
        right=NULL;
    }
    else
    {
        right=copySubTree(subtree->m_right);
    }
    parent=CreateNode(subtree->m_key, subtree->m_value);
    parent->m_left=left;
    parent->m_right=right;
    if (left!=NULL)
    {
        left->m_parent=parent;
    }
    if (right!=NULL)
    {
        right->m_parent=parent;
    }
    return parent;
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree (const BinarySearchTree<Key, Value>& other)	
{
    m_root=CreateNode(Key(), Value());
    if (other.m_root->m_left!=NULL)
    {
        m_root->m_left=copySubTree(other.m_root->m_left);
    }
}

template <typename Key, typename Value>
BinarySearchTree<Key, Value>& BinarySearchTree<Key, Value>::operator= (const BinarySearchTree<Key, Value>& other)
{
    if (this!=&other)
    {
        deleteSubTree(m_root->m_left);
        m_root->m_left=copySubTree(other.m_root->m_left);
    }
    return *this;
}

// k 不确定啊!!!
template <typename Key, typename Value>
bool BinarySearchTree<Key, Value>::InsertSubTree (typename BinarySearchTree<Key, Value>::Node& subTree)
{
    BSTnode* subtree=subTree.getSelf();
    bool bleft=true; 
    if (subtree->m_parent!=NULL) 
    {
        if (subtree->m_parent->m_left==subtree)
        {
            subtree->m_parent->m_left=NULL;
        }
        else
        {
            bleft=false;
            subtree->m_parent->m_right=NULL;
        }
    }
    if (m_root->m_left==NULL) //对于树为空时
    {
        subtree->m_parent=m_root;
        m_root->m_left=subtree;
        return true;
    }
    BSTnode* root=m_root->m_left;
    BSTnode* parent;
    while (root!=NULL)
    {
        parent=root;
        if (root->m_key==k) 
        {
            if (subtree->m_parent!=NULL)
            {
                if (bleft)
                {
                    subtree->m_parent->m_left=subtree;
                }
                else
                {
                    subtree->m_parent->m_right=subtree;
                }
            }
            return false;
        }
        else if (root->m_key<k) 
        {
            root=root->m_right;
        }
        else 
        {
            root=root->m_left;
        }
    }
    if (parent->m_key<k)
    {
        parent->m_right=subtree;
    }
    else
    {
        parent->m_left=subtree;
    }
    subtree->m_parent=parent;
    
    return true;
}


template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::DeleteSubTree (typename BinarySearchTree<Key, Value>::Node& subTree)
{
    BSTnode* subtree=subTree.getSelf();
    if (subtree->m_parent!=NULL)
    {
        if (subtree->m_parent->m_left==subtree)
        {
            subtree->m_parent->m_left=NULL;
        }
        else
        {
            subtree->m_parent->m_right=NULL;
        }
    }
    deleteSubTree(subtree);
    subTree=Node(NULL);
}

template <typename Key, typename Value>
inline BinarySearchTree<Key, Value> BinarySearchTree<Key, Value>::CopySubTree (typename BinarySearchTree<Key, Value>::Node& subTree)
{
    BinarySearchTree<Key, Value> tmp;
    tmp.m_root->m_left=copySubTree(subTree->getSelf());
    return tmp;
}

template <typename Key, typename Value>
typename BinarySearchTree<Key, Value>::Node BinarySearchTree<Key, Value>::Find (const Key& k)
{
    BSTnode* root=m_root->m_left;
    while (root!=NULL)
    {
        if (root->m_key==k)
        {
            return Node(root);
        }
        else if (root->m_key<k)
        {
            root=root->m_right;
        }
        else
        {
            root=root->m_left;
        }
    }
    return Node(NULL);
}

template <typename Key, typename Value>
inline unsigned int BinarySearchTree<Key, Value>::GetDepth (typename BinarySearchTree<Key, Value>::Node& subTree)
{
    return GetDepth(subTree.getSelf());
}
// 求树的高度
template <typename Key, typename Value>
unsigned int BinarySearchTree<Key, Value>::GetDepth (typename BinarySearchTree<Key, Value>::BSTnode* subtree)
{
    unsigned int left=0, right=0, root=0;
    if (subtree==NULL)
    {
        return 0;
    }
    left=GetDepth(subtree->m_left);
    right=GetDepth(subtree->m_right);
    root=( left>right ? left : right )+1;
    return root;
}

template <typename Key, typename Value>
inline void BinarySearchTree<Key, Value>::CountLeaf (typename BinarySearchTree<Key, Value>::Node& subTree, int& outSum)
{
    outSum=0;
    CountLeaf(subTree.getSelf(), outSum);
}
// 求叶子的个数
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::CountLeaf (typename BinarySearchTree<Key, Value>::BSTnode* subtree, int& outSum)
{
    if (subtree->m_left==NULL && subtree->m_right==NULL)
    {
        ++outSum;
        return;
    }
    if (subtree->m_left!=NULL)
    {
        CountLeaf(subtree->m_left, outSum);
    }
    if (subtree->m_right!=NULL)
    {
        CountLeaf(subtree->m_right, outSum);
    }
}
// 删除元素
template <typename Key, typename Value>
void BinarySearchTree<Key, Value>::DeleteItem (const Key& k)
{
    if (m_root->m_left==NULL)
    {
        return;
    }
    BSTnode* root=m_root->m_left; //待删除的节点
    BSTnode* parent=m_root; //待删除节点的父节点
    while ( root!=NULL )
    {
        if ( root->m_key==k )
        {
            break;
        }
        else if ( root->m_key<k )
        {
            root=root->m_right;
        }
        else
        {
            root=root->m_left;
        }
    }
    if ( root==NULL )
    {
        return;
    }
    else
    {
        parent=root->m_parent;
    }
    if ( root->m_left==NULL && root->m_right==NULL )
    {
        if ( parent->m_left==root )
        {
            parent->m_left=NULL;
        }
        else
        {
            parent->m_right=NULL;
        }
    }
    else if ( root->m_left!=NULL && root->m_right!=NULL )
    {
        BSTnode* left=root->m_left; //待删除节点左子树根
        BSTnode* pleft=left;  //左子树最大值节点 新根
        BSTnode* ppleft=NULL; //左子树最大值节点的父节点
        while ( pleft->m_right!=NULL )
        {
            pleft=pleft->m_right;
        }
        ppleft=pleft->m_parent;
        pleft->m_right=root->m_right;
        if ( root->m_right!=NULL )
        {
            root->m_right->m_parent=pleft;
        }
        if ( pleft==left )
        {
            if ( parent!=root )
            {
                if ( parent->m_left==root )
                {
                    parent->m_left=left;
                }
                else
                {
                    parent->m_right=left;
                }
                left->m_parent=parent;
            }
            else
            {
                m_root->m_left=left;
                left->m_parent=m_root;
            }
        }
        else
        {
            ppleft->m_right=pleft->m_left;
            if ( pleft->m_left!=NULL )
            {
                pleft->m_left->m_parent=ppleft;
            }
            pleft->m_left=left;
            left->m_parent=pleft;
            if ( parent!=root )
            {
                if ( parent->m_left==root )
                {
                    parent->m_left=pleft;
                }
                else
                {
                    parent->m_right=pleft;
                }
                pleft->m_parent=parent;
            }
            else 
            {
                m_root->m_left=pleft;
            }
            pleft->m_parent=m_root;
        }
    }
    else if ( root->m_left!=NULL )
    {
        if ( parent->m_left==root )
        {
            parent->m_left=root->m_left;
        }
        else
        {
            parent->m_right=root->m_left;
        }
        root->m_left->m_parent=parent;
    }
    else
    {
        if ( parent->m_left==root )
        {
            parent->m_left=root->m_right;
        }
        else
        {
            parent->m_right=root->m_right;
        }
        root->m_right->m_parent=parent;
    }
    DeleteNode(root);
}


