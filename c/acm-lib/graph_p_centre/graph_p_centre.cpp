      
/** @file graph_p_centre.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ͼ��P�������㷨����
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

const int MAXN=50;	//�ڽӾ����������
float chart[MAXN][MAXN]; //����ͼ���ڽӾ���
float sheet[MAXN][MAXN];	 //�����
bool mark[MAXN];		 //��־��true Vi��Vj�����·�������
int s[MAXN], b_s[MAXN];	  //s��Ϸ���b_sĿǰ��õ����
int way[MAXN], b_way[MAXN]; //way��������b_way��ѷ���
//�������iѡ��p����������ķ����b_s[b_way[i]]
int n,p; //n������ p���ĵ���
float bst;  //Ŀǰ������������Сֵ

void init ()
{
    int i,j,a,k;
    float c;
    //��ʼ������
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
    //��ʼ���߼�Ȩ
    std::cout<<"Input the number of edge: ";
    std::cin>>a;
    for (k=0; k<a; ++k)
    {
        std::cin>>i>>j>>c;
        chart[i][j]=c;
        chart[j][i]=c;
    }
    //��ʼ���������
    do
    {
        std::cout<<"Input the number of center point: ";
        std::cin>>p;
    }while (p<=0 || p>=n);
}

//��������sheet�е�x�и�Ԫ��
void makeOneLine (int x)
{
    int i,j, best_j;
    float best;
    //��ʼ����־��
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

//��������
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
    //vΪ��ǰ���������������
    v=0;
    for (i=0; i<n; ++i) //vi��Ŀ�ĵ㣬
    {
        t=sheet[s[0]][i];
        k=1;
        //�ڵ�ǰp�ڵ��У�ѡ����vi�Ͻ���һ�����vk��vi��vk�������
        //Ϊt��vkΪp��ϵĵ�k��Ԫ�أ�
        for (j=2; j<p; ++j)	//ɨ��ѡ��ԭ�㵽Ŀ�ĵ����ȡ��С��
        {
            if (sheet[s[j]][i]<t)
            {
                t=sheet[s[j]][i];
                k=j;
            }
        }
        if (t>v)
        {
            v=t; //�÷�������һ��ԭ�㵽Ŀ��������������һ��
        }
        way[i]=k;
    }
    if (v<bst || bst==-1) //���з�����������������С
    {
        bst=v;
        //��Ÿ���������ķ����		  
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
    s[0]=-1; //s����p�������ķ���
    //�������з������������ֵ
    while (lev>-1)
    {
        while (s[lev]<n-1)
        {
            ++s[lev];
            if (lev==p-1)
            {
                //������һ��p��ϣ�����ǰp��ϵ���������룬���ҵó�Ŀǰ
                //��չ����������p�����С����������뷽��
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
    //��������
    std::cout<<"You should select: ";
    for (i=0; i<p; ++i)
    {
        std::cout<<b_s[i]<<" ";
    }
    std::cout<<std::endl;
    //�������������Ĺ�ϵ
    for (i=0; i<n; ++i)
    {
        std::cout<<i<<" go to "<<b_s[b_way[i]]<<std::endl;
    }
}

int main(int argc, char* argv[])
{
    init();	   //��ʼ��
    makeSheet(); //��������
    find();	   //����p����
    show();	   //���
    return 0;
}

