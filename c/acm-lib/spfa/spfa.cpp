      
/** @file spfa.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    SPFA�㷨����
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

#include <limits>
#include <iostream>
#include <queue>
#include <string.h>

using namespace std;

#define SIZE 26
#define MAX std::numeric_limits<int>::max()

int graph[SIZE][SIZE]={0}; //�ڽӾ���
int l; //������Ŀ
bool flag[SIZE]; //��ӱ�־false�Ѿ�����ӣ�����û�л�������չ
int times[SIZE]; //��չ����������ڶ������Ͳ�����չ
int distances[SIZE]; //���㵽��ʼ�����

int spfa(int start, int end)
{
    memset(times, 0, sizeof(times));
    memset(flag, true, sizeof(flag));

    int i, curr;
    bool f=true;
    i=l+1;
    while (i--)
        distances[i]=MAX;

    distances[start]=0;
    flag[start]=false;

    std::queue<int> q;
    q.push(start);
    while (!q.empty())
    {
        curr=q.front();
        q.pop();
        for (int i=1; i<=l; ++i)
        {
            if (graph[curr][i]!=MAX) //��
            {
                //������չ
                if (distances[i]>distances[curr]+graph[curr][i])
                {
                    distances[i]=distances[curr]+graph[curr][i];
                    //������չ����
                    times[i]=times[curr]+1;
                    if (times[i]>l)
                    {
                        f=true;
                        break;
                    }
                    //������չ
                    if (flag[i])
                    {
                        q.push(i);
                        flag[i]=false;
                    }
                }
            }
        }
        if (!f)
        {
            f=false;
            flag[curr]=true;
        }
        else
            f=false;
    }
    return distances[end];
}

int main()
{
    int t;
    std::cin>>t;
    int n, b;
    int s, e, k, m;
    while (t--)
    {
        std::cin>>n>>b>>l;
        for (int i=1; i<=l; ++i)
            for (int j=1; j<=l; ++j)
                graph[i][j]=MAX;

        for (int i=0; i<n; ++i)
        {
            std::cin>>s>>e>>k>>m;
            if (k==0)
                graph[s][e]=-m;
            else
                graph[s][e]=m;
        }
        std::cout<<spfa(1, b)<<'\n';
    }
    return 0;
} 


