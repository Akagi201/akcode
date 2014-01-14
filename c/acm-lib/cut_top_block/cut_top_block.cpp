      
/** @file cut_top_block.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��Ϳ���㷨����
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
    int k; //�������ͣ�dfn����
    int l; //low����
};

typedef int ghtype[maxn][maxn]; //�ڽӾ�������
typedef node ltype[maxn]; //������

ghtype g; //�ڽӾ���
int n; //������
int p; //ջָ��
int x; //������������
int i; //��������

std::ifstream f("input.txt"); //�ļ�����
ltype l; //��������

int st[maxn]; //ջ
bool k[maxn]; //�����

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
    push(0); //������ջ
    //�����״α����� dfn(0)=low(0)=1
    i=1;
    l[0].k=i;
    l[0].l=i;
    //�����Ϊ��
    memset(k, false, sizeof(k));
    x=0;
}

void search(int v)
{
    int u;
    for (u=0; u<n; ++u)
    {
        if (g[v][u]>0) //<v,u>�ߴ���
        {
            if (l[u].k==0) //u�״α�����
            {
                //����u��dfn��lowֵ
                ++i;
                l[u].k=i;
                l[u].l=i;
                push(u);
                //��u�����ݹ����
                search(u);
                //v�Ķ���u��ȫɨ�����,����v��lowֵ
                l[v].l=std::min(l[v].l, l[u].l);
                //��u��u�ĺ������׷�ݵ���v��������ȵ�
                if (l[u].l>=l[v].k)
                {
                    //��v���Ǹ��Ҳ����ڸ����,���ӡ�v��v�������
                    if (v!=0 && !k[v])
                    {
                        std::cout<<"["<<v<<"]"<<"\n";
                        k[v]=true;
                    }
                    //��v�Ǹ�����������������
                    if (v==0)
                        ++x;
                    //��ջ������ȡ����v�ĸ�����,����һ����ͨ��֧
                    while (st[p]!=v)
                        std::cout<<pop()<<" ";
                    std::cout<<std::endl;
                }//
            }//
            else
                l[v].l=std::min(l[v].l, l[u].k); //<v,u>�Ǻ����u�ظ�����, ������v��lowֵ
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


