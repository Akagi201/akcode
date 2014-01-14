      
/** @file binary_search_tree.hpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ͨ�����������ӿ�
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
    //�����������ڵ�����
    struct BSTnode
    {
        const Key m_key;      //�ȽϹؼ���
        Value m_value;         //ӳ������
        BSTnode* m_left;      //������
        BSTnode* m_right;     //������
        BSTnode* m_parent;    //˫�׽ڵ�
        BSTnode (const Key& k, const Value& v);
        //�����ṹ
        void Destroy ();
    private:
        ~BSTnode ();  //��ֻ֤���ڶ��ϴ�������
    }; //end of struct BSTnode
public:
    //����������������������
    class Node
    {
    public:
        explicit Node (typename BinarySearchTree<Key, Value>::BSTnode* target=NULL);
        bool IsNull () const;   //�жϽڵ��Ƿ�Ϊ��
        bool IsRoot () const;   //�жϽڵ��Ƿ�Ϊ��
        Node GetLeft () const;  //����������
        Node GetRight () const; //����������
        Node GetParent () const;//���ظ��ڵ�
        void MoveLeft ();       //�ƶ���������
        void MoveRight ();      //�ƶ���������
        void MoveParent ();     //�ƶ���˫��
        const Key& GetKey () const;      //���ص�ǰ�ڵ�ļ�ֵ
        void SetValue (const Value& v);  //���õ�ǰ�ڵ�����
        Value& GetValue () const;         //���ص�ǰ�ڵ�����
        BSTnode* getSelf () const        //�㷨ʵ���ڲ����ò�Ҫ�����˺���
        {
            return m_subTree;
        }
    private:
        //���õ�����ָ��
        typename BinarySearchTree<Key, Value>::BSTnode* m_subTree;
    }; //end of class Node
public:
    //��������
    BinarySearchTree ();
    BinarySearchTree (const BinarySearchTree<Key, Value>& other);
    BinarySearchTree<Key, Value>& operator= (const BinarySearchTree<Key, Value>& other);
    //�������
    ~BinarySearchTree ();
    //�����Բ���
    void InsertItem (const Key& k, const Value& v); //�����µ�Ԫ��
    void DeleteItem (const Key& k);                 //ɾ���ض���Ԫ��
    bool InsertSubTree (typename BinarySearchTree<Key, Value>::Node& subTree); //��������
    void DeleteSubTree (typename BinarySearchTree<Key, Value>::Node& subTree); //ɾ������
    //����һ��������
    BinarySearchTree<Key, Value> CopySubTree (typename BinarySearchTree<Key, Value>::Node& subTree);
    //�����Բ���
    Node GetRoot () const;    //��ø����ʶ���
    Node Find (const Key& k); //�����ض�Ԫ��
    //�������ĸ߶�
    unsigned int GetDepth (typename BinarySearchTree<Key, Value>::Node& subTree);
    //����Ҷ�ӽڵ����
    void CountLeaf (typename BinarySearchTree<Key, Value>::Node& subTree, int& outSum);
    //��������ǰ���С��������
    template <typename Visit>
        void PreOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
    template <typename Visit>
        void InOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
    template <typename Visit>
        void PostOrder (typename BinarySearchTree<Key, Value>::Node& subTree, Visit visitor);
private:
    //�ڲ�����ʵ��
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
    typename BinarySearchTree<Key, Value>::BSTnode* m_root;  //����ָ��
}; //end of class BinarySearchTree
//----------end class BinarySearchTree definition ----------//

#endif // BINARY_SEARCH_TREE_HPP_

