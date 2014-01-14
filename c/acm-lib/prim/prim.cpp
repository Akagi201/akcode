      
/** @file prim.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    无向图最小生成树
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
#include <limits>

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

//在已标记的点集合中寻找与未标记点关联最小权的边
void min (short G[][NUM_VERTICE], int numOfVertice, bool* visited, int& a, int& b)
{
    int i,j;
    short MIN=std::numeric_limits<short>::max();
    a=b=-1;
    for (i=0; i<numOfVertice; ++i)
    {
        if (visited[i])  //确定在已标记点集合中
        {
            for (j=0; j<numOfVertice; ++j)
            {
                //源点与目标点不同并且目标点未被标记过与源点关联且小于最小值
                if (j!=i && !visited[j] && G[i][j]>0 && G[i][j]<MIN)
                {
                    a=i;
                    b=j;
                    MIN=G[i][j];
                }//end of if
            }//end of for
        }//end of if
    }// end of for
}

void Prim (short G[][NUM_VERTICE], int numOfVertice, int start)
{
    assert(G!=NULL);
    assert(numOfVertice>0);
    assert(start>=0 && start<numOfVertice);
    bool* visited=new bool[numOfVertice];
    assert(visited!=NULL);
    int i;
    for (i=0; i<numOfVertice; ++i)
    {
        visited[i]=false;
    }
    int a,b;
    visited[start]=true; //放入起点到标记集合
    int count=numOfVertice-1; //找numOfVertice-1条边
    while (count)
    {
        min(G, numOfVertice, visited, a, b);
        std::cout<<"Edge: "<<a<<"  "<<b<<std::endl;
        visited[b]=true; //标记选过的点添加到标记点集合
        --count;
    }
    delete[] visited;
}

int main (int argc, char* argv[])
{
    Prim(graph, NUM_VERTICE, 0);
    return 0;
}

