      
/** @file dfs_bfs.cpp
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
#include <stack>
#include <queue>

const short NUM_VERTICE=9;
static short graph[NUM_VERTICE][NUM_VERTICE]=
{
    {0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 1, 0}
};

static bool old[9]={false};

//深度优先
void DFS (short G[][NUM_VERTICE], int numOfVertice, bool visited[], short start, void Func(short& vertice))
{
    std::stack<short> S;
    S.push(start);
    int i;
    while (!S.empty())
    {
        start=S.top();
        S.pop();
        Func(start);
        for (i=0; i<numOfVertice; ++i)
        {
            if (G[start][i]!=0 && !visited[i])
            {
                visited[i]=true;
                S.push(i);
            }
        }
    }
}

//广度优先
void BFS (short G[][NUM_VERTICE], int numOfVertice, bool visited[], short start, void Func(short& vertice))
{
    std::queue<short> Q;
    Q.push(start);
    short i;
    while (!Q.empty())
    {
        start=Q.front();
        Q.pop();
        for (i=0; i<numOfVertice; ++i)
        {
            if (G[start][i]!=0 && !visited[i])
            {
                visited[i]=true;
                Func(i);
                Q.push(i);
            }
        }
    }
}

void show (short& target)
{
    std::cout<<target<<" ";
}

int main (int argc, char* argv[])
{
    short i;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        if (!old[i])
        {
            old[i]=true;
            DFS(graph, NUM_VERTICE, old, i, show);
        }
    }
    std::cout<<std::endl;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        old[i]=false;
    }
    for (i=0; i<NUM_VERTICE; ++i)
    {
        if (!old[i])
        {
            old[i]=true;
            show(i);
            BFS(graph, NUM_VERTICE, old, i, show);
        }
    }
    std::cout<<std::endl;
    return 0;
}

