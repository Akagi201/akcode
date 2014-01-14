      
/** @file disjoint_set.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    基本并查集
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

#define  SIZE 1000
int v[SIZE];
int rank[SIZE];

void init(int n)
{
    for (int i=1; i<=n; ++i)
    {
        v[i]=i;
        rank[i]=0;
    }
}

int find(int p)
{
    if (v[p]==p)
    {
        return p;
    }
    else
    {
        v[p]=find(v[p]);
        return v[p];
    }
}

void judge(int a, int b)
{
    int fa, fb;
    fa=find(a);
    fb=find(b);
    if (fa==fb)
    {
        return;
    }
    if (rank[fa]>rank[fb])
    {
        v[fb]=fa;
    }
    else
    {
        v[fa]=v[fb];
        if (rank[fa]=rank[fb])
        {
            ++rank[fb];
        }
    }
}


