      
/** @file topological.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��������
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

#include <iostream>
#include <cassert>
#include <queue>

const short NUM_VERTICE=10;

struct Node  //�ڽ�����Ԫ�ؽڵ�
{
    short m_name;
    Node* m_next;
    Node (short name, Node* next=NULL): m_name(name), m_next(next) {}
};

Node* graph[NUM_VERTICE]; //ͼ
short indegree[NUM_VERTICE]={0}; //�����ص����

void preinit ()
{
    for (short i=0; i<NUM_VERTICE; ++i)
    {
        graph[i]=new Node(i);
    }
}

void init ()
{
    graph[0]->m_next=new Node(9);
    graph[3]->m_next=new Node(1);
    graph[3]->m_next->m_next=new Node(2);
    graph[3]->m_next->m_next->m_next=new Node(4);
    graph[4]->m_next=new Node(1);
    graph[4]->m_next->m_next=new Node(2);
    graph[5]->m_next=new Node(4);
    graph[5]->m_next->m_next=new Node(6);
    graph[7]->m_next=new Node(0);
    graph[8]->m_next=new Node(5);
    graph[8]->m_next->m_next=new Node(7);
}

void caculateIndegree ()
{
    short i;
    Node* p=NULL;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        p=graph[i]->m_next;
        while (p!=NULL)
        {
            ++indegree[p->m_name];
            p=p->m_next;
        }
    }
}

void sort ()
{
    std::queue<short> Q;
    short i;
    //���Ϊ�ĵ����
    for (i=0; i<NUM_VERTICE; ++i)
    {
        if (indegree[i]==0)
        {
            Q.push(i);
        }
    }
    int count=0;
    int v;
    Node* p=NULL;
    //�������Ϊ�ĵ�ʹ���������ĵ���ȼ�
    while (!Q.empty())
    {
        v=Q.front();
        Q.pop();
        std::cout<<v<<" ";
        p=graph[v]->m_next;
        ++count;
        while (p!=NULL)
        {
            --indegree[p->m_name];
            if (indegree[p->m_name]==0)
            {
                Q.push(p->m_name);
            }
            p=p->m_next;
        }
    }
    std::cout<<std::endl;
    if (count<NUM_VERTICE)
    {
        std::cout<<"There is circle in the digraph."<<std::endl;
    }
}

void free ()
{
    Node* p=NULL;
    for (int i=0; i<NUM_VERTICE; ++i)
    {
        while (graph[i])
        {
            p=graph[i]->m_next;
            delete graph[i];
            graph[i]=p;
        }
    }
}

int main (int argc, char* argv[])
{
    preinit();
    init();
    caculateIndegree();
    sort();
    free();
    return 0;
}


