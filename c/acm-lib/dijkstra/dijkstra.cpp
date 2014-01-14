      
/** @file dijkstra.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    有向图的单源最短路径
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
const short MAX=std::numeric_limits<short>::max();

static short graph[NUM_VERTICE][NUM_VERTICE]=
{
    {MAX, 3, 4, 8, MAX, MAX, MAX, MAX, MAX},
    {3, MAX, 2, MAX, MAX, MAX, MAX, MAX, MAX},
    {4, 2, MAX, MAX, MAX, MAX, MAX, MAX, MAX},
    {8, MAX, MAX, MAX, 10, MAX, MAX, MAX, MAX},
    {MAX, MAX, MAX, 10, MAX, 7, MAX, MAX, 7},
    {MAX, MAX, MAX, MAX, 7, MAX, 1, 2, MAX},
    {MAX, MAX, MAX, MAX, MAX, 1, MAX, 9, 4},
    {MAX, MAX, MAX, MAX, MAX, 2, 9, MAX, 3},
    {MAX, MAX, MAX, 7, MAX, 4, 3, MAX, MAX}
};

static int cost[NUM_VERTICE]={0};
static bool visited[NUM_VERTICE]={false};

void Dijkstra (short G[][NUM_VERTICE], int numOfVertice, int start)
{
    assert(G!=NULL);
    assert(numOfVertice>0);
    assert(start>=0 && start<numOfVertice);
    cost[start]=0;  //起点花费为
    visited[start]=true; //起点被访问
    int i,j;
    int best_j; //当前最小花费端点
    int best;  //当前最小花费
    do //Dijkstra算法核心
    {
        best=0;
        for (i=0; i<NUM_VERTICE; ++i)
        {
            //在已访问的点中搜索
            if (visited[i])
            {
                for (j=0; j<NUM_VERTICE; ++j)
                {
                    //端点未访问且与i关联
                    if (!visited[j] && graph[i][j]<MAX)
                    {
                        //当前花费最小
                        if (best==0 || graph[i][j]+cost[i]<best)
                        {
                            best=graph[i][j]+cost[i];
                            best_j=j;
                        }// end of if
                    }// end of if
                }//end of for
            }// end of if
        }// end of for
        if (best>0) //找到最小花费更新花费
        {
            cost[best_j]=best;
            visited[best_j]=true;
        }
    }while (best!=0);
}

int main (int argc, char* argv[])
{
    Dijkstra(graph, NUM_VERTICE, 0);
    for (int i=0; i<NUM_VERTICE; ++i)
    {
        std::cout<<cost[i]<<" ";
    }
    std::cout<<std::endl;
    return 0;
}


