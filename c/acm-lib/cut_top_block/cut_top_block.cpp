      
/** @file cut_top_block.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    割顶和块的算法举例
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

#include<iostream>
#include <algorithm>
#include <fstream>

#define maxn 30

struct node
{
    int k; //顶点类型，dfn编码
    int l; //low编码
};

typedef int ghtype[maxn][maxn]; //邻接矩阵类型
typedef node ltype[maxn]; //点类型

ghtype g; //邻接矩阵
int n; //顶点数
int p; //栈指针
int x; //根的子树个数
int i; //辅助变量

std::ifstream f("input.txt"); //文件对象
ltype l; //顶点序列

int st[maxn]; //栈
bool k[maxn]; //割顶集合

void push (int a)
{
    ++p;
    st[p]=a;
}

int pop()
{
    int t=st[p];
    --p;
    return t;
}

void read_graph()
{
    f>>n;
    for (int i=0; i<n; ++i)
    {
        for (int j=0; j<n; ++j)
        {
            f>>g[i][j];
        }
    }
    f.close();
    memset(l, 0, sizeof(l));
    p=0;
    push(0); //顶点入栈
    //顶点首次被访问 dfn(0)=low(0)=1
    i=1;
    l[0].k=i;
    l[0].l=i;
    //割顶集合为空
    memset(k, false, sizeof(k));
    x=0;
}

void search(int v)
{
    int u;
    for (u=0; u<n; ++u)
    {
        if (g[v][u]>0) //<v,u>边存在
        {
            if (l[u].k==0) //u首次被访问
            {
                //设置u的dfn和low值
                ++i;
                l[u].k=i;
                l[u].l=i;
                push(u);
                //从u出发递归求解
                search(u);
                //v的儿子u完全扫描完毕,设置v的low值
                l[v].l=std::min(l[v].l, l[u].l);
                //从u及u的后代不会追溯到比v更早的祖先点
                if (l[u].l>=l[v].k)
                {
                    //若v不是根且不属于割顶集合,则打印割顶v，v进割顶集合
                    if (v!=0 && !k[v])
                    {
                        std::cout<<"["<<v<<"]"<<"\n";
                        k[v]=true;
                    }
                    //若v是根，计算其子树个数
                    if (v==0)
                        ++x;
                    //从栈中依次取出至v的个顶点,产生一个连通分支
                    while (st[p]!=v)
                        std::cout<<pop()<<" ";
                    std::cout<<std::endl;
                }//
            }//
            else
                l[v].l=std::min(l[v].l, l[u].k); //<v,u>是后向边u重复访问, 则设置v的low值
        }
    }
}

int main(int argc, char* argv[])
{
    read_graph();
    search(0);
    if (x>1)
        std::cout<<"[0]\n";
    return 0;
} 


