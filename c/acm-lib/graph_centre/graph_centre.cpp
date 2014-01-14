      
/** @file graph_centre.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    求图的一个中心算法举例
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

const int MAXN=100;	 //邻接矩阵最大容量
float chart[MAXN][MAXN]; //有无图的邻接矩阵
bool mark[MAXN];		 //标志表true Vi到Vj的最短路径已求出
float b[MAXN];		   //Vi到Vj的最短路径长度
int n;				   //顶点数

void init ()
{
    int i,j,a,k;
    float c;
    do
    {
        std::cout<<"Please input the number of vertice: ";
        std::cin>>n;	
    } while (n<=0 || n>=MAXN);
    for (i=0; i<n; ++i)
    {
        for (j=0; j<n; ++j)
        {
            chart[i][j]=0;
        }
    }
    std::cout<<"Please input the number of edge: ";
    std::cin>>a;
    std::cout<<"Please input the weight of the edge: "<<std::endl;
    for (k=0; k<a; ++k)
    {
        std::cin>>i>>j>>c;
        chart[i][j]=c; //这两个赋数体现为无向图
        chart[j][i]=c; 
    }
}

//返回顶点x的最大服务距离即在顶点x与其它顶点距离中最大值
float findFarest (int x)
{
    int best_j; //顶点x与顶点j最短路径终点
    int i,j;
    float best, farest;//最短路径  最大服务距离
    //初始化标志数组与最短路径数组
    for (i=0; i<n; ++i)
    {
        b[i]=0;
        mark[i]=false;
    }
    mark[x]=true;
    farest=0;
    do
    {
        best=0;
        //从所有起点已编号，终点为编号的弧集合中，选一条弧
        //x best_j使顶点x到best_j最短
        for (i=0; i<n; ++i)
        {
            if (mark[i])
            {
                for (j=0; j<n; ++j)
                {
                    if (!mark[j] && chart[i][j]>0)
                    {
                        if (best==0 || b[i]+chart[i][j]<best)
                        {
                            best=b[i]+chart[i][j];
                            best_j=j;
                        }
                    }
                }
            }
        }
        if (best)
        {
            b[best_j]=best;
            mark[best_j]=true;
            //筛选最大的最短路径
            if (best>farest)
            {
                farest=best;
            }
        }
    }while (best);
    return farest;
}

void find ()
{
    int i,best_i;
    float e,best;
    best=0;
    //求出各顶点的最大服务距离中的最小值
    for (i=0; i<n; ++i)
    {
        e=findFarest(i);
        if (e<best || best==0)
        {
            best=e;
            best_i=i;
        }
    }
    std::cout<<"Best vertice is "<<best_i<<" and length is "<<best<<std::endl;
}

int main(int argc, char* argv[])
{
    init();
    find();
    return 0;
}


