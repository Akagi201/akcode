      
/** @file kruskal.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    
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

const short NUM_VERTICE=9;
static short graph[NUM_VERTICE][NUM_VERTICE]=
{
    {0, 3, 4, 8, 0, 0, 0, 0, 0},
    {3, 0, 7, 0, 0, 0, 0, 0, 0},
    {4, 7, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 0, 0, 10, 0, 0, 0, 0},
    {0, 0, 0, 10, 0, 7, 0, 0, 7},
    {0, 0, 0, 0, 7, 0, 1, 2, 0},
    {0, 0, 0, 0, 0, 1, 0, 9, 4},
    {0, 0, 0, 0, 0, 2, 9, 0, 3},
    {0, 0, 0, 0, 7, 0, 4, 3, 0}
};

struct Edge //�߽ṹ��������
{
    short from;
    short to;
    int weight;
};

class Cmp
{
public:
    bool operator() (const Edge& lbs, const Edge& rbs)
    {
        return lbs.weight>rbs.weight;
    }
};

void Kruskal (short G[][NUM_VERTICE], int numOfVertice)
{
    assert(G!=NULL);
    assert(numOfVertice>0);
    std::priority_queue<Edge, std::vector<Edge>, Cmp> PQ; //��С�ѵ����ȶ���
    int i,j;
    Edge temp;
    //��ͼ�еı�ת���ɱ߽ṹ�Ž����ȶ���
    for (i=0; i<numOfVertice; ++i)
    {
        for (j=0; j<numOfVertice; ++j)
        {
            if (G[i][j]>0)
            {
                temp.from=i;
                temp.to=j;
                temp.weight=G[i][j];
                PQ.push(temp);
            }
        }
    }
    //��ÿһ�����㿴��ɭ���Լ�Ϊ��
    short* root=new short[numOfVertice];
    for (i=0; i<numOfVertice; ++i)
    {
        root[i]=i;
    }
    //Kruskal�㷨����
    while (!PQ.empty())
    {
        temp=PQ.top(); 
        PQ.pop();
        //�������ĸ���ͬ
        if (root[temp.from]!=root[temp.to])
        {
            std::cout<<"Edge: "<<temp.from<<" "<<temp.to<<std::endl;
            //���ڶ������ĸ���Ϊ��һ�����ĸ�
            for (j=0; j<numOfVertice; ++j)
            {
                if (root[j]==root[temp.to] && j!=temp.to)
                {
                    root[j]=root[temp.from];
                }
            }
            root[temp.to]=root[temp.from];
        }
    }
    delete[] root;
}

int main (int argc, char* argv[])
{
    Kruskal(graph, NUM_VERTICE);
    return 0;
}

