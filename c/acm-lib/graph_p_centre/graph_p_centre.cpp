      
/** @file graph_p_centre.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    求图的P个中心算法举例
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
#include <iomanip>

const int MAXN=50;	//邻接矩阵最大容量
float chart[MAXN][MAXN]; //有无图的邻接矩阵
float sheet[MAXN][MAXN];	 //距离表
bool mark[MAXN];		 //标志表true Vi到Vj的最短路径已求出
int s[MAXN], b_s[MAXN];	  //s组合方案b_s目前最好的组合
int way[MAXN], b_way[MAXN]; //way辅助变量b_way最佳方案
//即服务点i选择p中心中最近的服务点b_s[b_way[i]]
int n,p; //n顶点数 p中心点数
float bst;  //目前最大服务距离的最小值

void init ()
{
    int i,j,a,k;
    float c;
    //初始化点数
    do
    {
        std::cout<<"Input the number of vertice: ";
        std::cin>>n;
    }while (n<=0 || n>=MAXN);
    for (i=0; i<n; ++i)
    {
        for (j=0; j<n; ++j)
        {
            chart[i][j]=0;
        }
    }
    //初始化边及权
    std::cout<<"Input the number of edge: ";
    std::cin>>a;
    for (k=0; k<a; ++k)
    {
        std::cin>>i>>j>>c;
        chart[i][j]=c;
        chart[j][i]=c;
    }
    //初始化服务点数
    do
    {
        std::cout<<"Input the number of center point: ";
        std::cin>>p;
    }while (p<=0 || p>=n);
}

//构造距离表sheet中的x行各元素
void makeOneLine (int x)
{
    int i,j, best_j;
    float best;
    //初始化标志标
    for (i=0; i<n; ++i)
    {
        mark[i]=false;
        sheet[x][i]=0;
    }
    mark[x]=true;
    sheet[x][x]=0;
    do
    {
        best=0;
        for (i=0; i<n; ++i)
        {
            if (mark[i])
            {
                for (j=0; j<n; ++j)
                {
                    if (!mark[j] && chart[i][j]>0)
                    {
                        if (best==0 || sheet[x][i]+chart[i][j]<best)
                        {
                            best=sheet[x][i]+chart[i][j];
                            best_j=j;
                        }
                    }
                }
            }
        }
        if (best)
        {
            sheet[x][best_j]=best;
            mark[best_j]=true;
        }
    }
    while (best);
}

//构造距离表
void makeSheet ()
{
    int i;
    for (i=0; i<n; ++i)
    {
        makeOneLine(i);
    }
}

void check ()
{
    int i,j,k;
    float t,v;
    //v为当前方案的最大服务距离
    v=0;
    for (i=0; i<n; ++i) //vi是目的点，
    {
        t=sheet[s[0]][i];
        k=1;
        //在当前p节点中，选择离vi较近的一个结点vk，vi至vk距离最短
        //为t（vk为p组合的第k个元素）
        for (j=2; j<p; ++j)	//扫描选中原点到目的点距离取最小者
        {
            if (sheet[s[j]][i]<t)
            {
                t=sheet[s[j]][i];
                k=j;
            }
        }
        if (t>v)
        {
            v=t; //该方案中这一个原点到目标点最大距离中最大一个
        }
        way[i]=k;
    }
    if (v<bst || bst==-1) //所有方案服务距离最大中最小
    {
        bst=v;
        //存放各顶点最近的服务点		  
        for (i=0; i<p; ++i)
        {
            b_s[i]=s[i];
        }
        for (i=0; i<n; ++i)
        {
            b_way[i]=way[i];
        }
    }
}

void find ()
{
    int lev;
    bst=-1;
    lev=0;
    s[0]=-1; //s中是p个服务点的方案
    //产生排列方案并计算最佳值
    while (lev>-1)
    {
        while (s[lev]<n-1)
        {
            ++s[lev];
            if (lev==p-1)
            {
                //若产生一个p组合，则求当前p组合的最大服务距离，并且得出目前
                //扩展出来的所有p组合最小的最大服务距离方案
                check();
            }
            else
            {
                ++lev;
                s[lev]=s[lev-1];
            }
        }
        --lev;
    }
}

void show ()
{
    int i;
    //输出服务点
    std::cout<<"You should select: ";
    for (i=0; i<p; ++i)
    {
        std::cout<<b_s[i]<<" ";
    }
    std::cout<<std::endl;
    //输出个点与服务点的关系
    for (i=0; i<n; ++i)
    {
        std::cout<<i<<" go to "<<b_s[b_way[i]]<<std::endl;
    }
}

int main(int argc, char* argv[])
{
    init();	   //初始化
    makeSheet(); //构造距离表
    find();	   //计算p中心
    show();	   //输出
    return 0;
}

