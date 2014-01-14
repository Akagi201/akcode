      
/** @file main.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ²âÊÔ¶þ²æËÑË÷Ê÷¹¦ÄÜ
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

void print(const int& t);

int main()
{

    srand(time(0));
    BinarySearchTree<int, int> trees;
    int i, t;
    for (i=0; i<100; ++i)
    {
        t=rand()%1000;
        trees.InsertItem(t, t);
    }
    BinarySearchTree<int,int>::Node root=trees.GetRoot();
    std::cout<<"InOrder: ";
    trees.InOrder(root, print);
    std::cout<<std::endl;
    int del;
    std::cout<<"Delete: ";
    std::cin>>del;
    trees.DeleteItem(del);
    std::cout<<"InOrder: ";
    trees.InOrder(root, print);
    std::cout<<std::endl;
    trees.CountLeaf(root, t);
    std::cout<<"Leaf: "<<t<<std::endl;
    std::cout<<"Depth: "<<trees.GetDepth(root)<<std::endl;
    return 0;
}

void print(const int& t)
{
    std::cout<<t<<" ";
}


