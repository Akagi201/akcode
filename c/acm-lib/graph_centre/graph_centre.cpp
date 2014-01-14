      
/** @file graph_centre.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ��ͼ��һ�������㷨����
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

const int MAXN=100;	 //�ڽӾ����������
float chart[MAXN][MAXN]; //����ͼ���ڽӾ���
bool mark[MAXN];		 //��־��true Vi��Vj�����·�������
float b[MAXN];		   //Vi��Vj�����·������
int n;				   //������

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
        chart[i][j]=c; //��������������Ϊ����ͼ
        chart[j][i]=c; 
    }
}

//���ض���x����������뼴�ڶ���x������������������ֵ
float findFarest (int x)
{
    int best_j; //����x�붥��j���·���յ�
    int i,j;
    float best, farest;//���·��  ���������
    //��ʼ����־���������·������
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
        //����������ѱ�ţ��յ�Ϊ��ŵĻ������У�ѡһ����
        //x best_jʹ����x��best_j���
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
            //ɸѡ�������·��
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
    //��������������������е���Сֵ
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


