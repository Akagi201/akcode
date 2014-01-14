      
/** @file aoe.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    AOE网的算法
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
#include <algorithm>
#include <limits>
#include <queue>
#include <list>

const short MAX=std::numeric_limits<short>::max();
const short NUM_VERTICE=6;

struct Active  //活动信息结构
{
    short beforeEvent; //发生该活动前一个事件
    short afterEvent;  //发生该活动后一个事件
    short eTime;		 //活动发生最早时间
    short lTime;		 //活动发生最晚时间
    short space;		 //活动时间余量
};

static short AOE[NUM_VERTICE][NUM_VERTICE]=
{
    {MAX, 3, 2, MAX, MAX, MAX},
    {MAX, MAX, MAX, 2, 3, MAX},
    {MAX, MAX, MAX, 4, MAX, 3},
    {MAX, MAX, MAX, MAX, MAX, 2},
    {MAX, MAX, MAX, MAX, MAX, 1},
    {MAX, MAX, MAX, MAX, MAX, MAX},
};

static short indegree[NUM_VERTICE]; //顶点的入度用于拓扑排序
static short tpArray[NUM_VERTICE];  //拓扑排序序列内存放顶点值

static short VE[NUM_VERTICE]; //事件的最早发生时间
static short VL[NUM_VERTICE]; //事件的最晚发生时间

static std::list<Active> activelist;

//计算入度用于拓扑排序
void CaculateIndegree ()
{
    int count;
    int i,j;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        count=0;
        for (j=0; j<NUM_VERTICE; ++j)
        {
            if (AOE[j][i]<MAX)
            {
                ++count;
            }
        }
        indegree[i]=count;
    }
}

//拓扑排序
void TpSort ()
{
    std::queue<short> Q;
    int i;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        if (indegree[i]==0)
        {
            Q.push(i);
        }
    }
    int level=-1;
    int v;
    while (!Q.empty())
    {
        v=Q.front();
        Q.pop();
        tpArray[++level]=v;
        for (i=0; i<NUM_VERTICE; ++i)
        {
            if (AOE[v][i]<MAX)
            {
                --indegree[i];
                if (indegree[i]==0)
                {
                    Q.push(i);
                }
            }
        }// end of for
    }// end of while 
}

//正向扫描拓扑序列计算事件的最早发生时间
void GoForward ()
{
    int v=tpArray[0];
    VE[v]=0;
    int i,j;
    int e;
    for (i=1; i<NUM_VERTICE; ++i)
    {
        v=tpArray[i];
        e=0;
        for (j=0; j<NUM_VERTICE; ++j)
        {
            if (AOE[j][v]<MAX)
            {
                if (e==0 || AOE[j][v]+VE[j]>e)
                {
                    e=AOE[j][v]+VE[j];
                }
            }
        }
        VE[v]=e;
    }
}

//逆向扫描拓扑序列计算事件的最晚发生时间
void GoBack ()
{
    int v=tpArray[NUM_VERTICE-1];
    VL[v]=VE[v];
    int i, j;
    int e;
    for (i=NUM_VERTICE-2; i>=0; --i)
    {
        v=tpArray[i];
        e=0;
        for (j=0; j<NUM_VERTICE; ++j)
        {
            if (AOE[v][j]<MAX)
            {
                if (e==0 || VL[j]-AOE[v][j]<e)
                {
                    e=VL[j]-AOE[v][j];
                }
            }
        }
        VL[v]=e;
    }
}

//计算各活动信息
void fillInfo ()
{
    int i,j;
    Active temp;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        for (j=0; j<NUM_VERTICE; ++j)
        {
            if (AOE[i][j]<MAX)
            {
                temp.beforeEvent=i;
                temp.afterEvent=j;
                temp.eTime=VE[i];
                temp.lTime=VL[j]-AOE[i][j];
                temp.space=temp.lTime-temp.eTime;
                activelist.push_back(temp);
            }
        }// enf of for j
    }// end of for i
}

//显示活动记录供for_each调用
void showActive (const Active& t)
{
    std::cout<<"Before event "<<t.beforeEvent
        <<" After event "<<t.afterEvent
        <<" early time "<<t.eTime
        <<" last time "<<t.lTime
        <<" space time "<<t.space<<std::endl;
}

//显示处理信息
void showInfo ()
{
    int i,v;
    for (i=0; i<NUM_VERTICE; ++i)
    {
        v=tpArray[i];
        std::cout<<"Event "<<v<<" et "<<VE[v]<<" lt "<<VL[v]<<std::endl;
    }
    std::for_each(activelist.begin(), activelist.end(), showActive);
}

int main (int argc, char* argv[])
{
    CaculateIndegree();
    TpSort();
    GoForward();
    GoBack();
    fillInfo();
    showInfo();
    return 0;
}


