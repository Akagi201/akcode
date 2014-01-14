      
/** @file floyd.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    有向图的多源最短路径
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

#include <cassert>
#include <limits>
#include <stddef.h>

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

static short result[NUM_VERTICE][NUM_VERTICE];

void Floyd (short dest[][NUM_VERTICE], short src[][NUM_VERTICE], int numOfVertice)
{
    assert(dest!=NULL);
    assert(src!=NULL);
    assert(numOfVertice>0);
    int i,j,k;
    for (i=0; i<numOfVertice; ++i)
    {
        for (j=0; j<numOfVertice; ++j)
        {
            dest[i][j]=src[i][j];
        }
    }
    for (k=0; k<numOfVertice; ++k)
    {
        for (i=0; i<numOfVertice; ++i)
        {
            for (j=0; j<numOfVertice; ++j)
            {
                if (dest[i][k]+dest[k][j]<dest[i][j])
                {
                    dest[i][j]=dest[i][k]+dest[k][j];
                }
            }//end of for j
        }// end of for i
    }//end of for k
}

int main (int argc, char* argv[])
{
    Floyd(result, graph, NUM_VERTICE);

    return 0;
}


