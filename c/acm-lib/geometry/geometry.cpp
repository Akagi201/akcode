      
/** @file geometry.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    ���㼸��
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
#include <math.h>

using namespace std;

#define PRECISION 1e-8

struct Point
{
    double x, y;
};//ƽ���ϵĵ�

int dbcmp(double d)
{
    if (fabs(d) < PRECISION)
    {
        return 0;
    }
    return d > 0 ? 1 : -1;
}//����ں���,���⾫�����
// ����ģ
double length(double x, double y)
{
    return sqrt(x*x + y*y);
}//������ģ
// �������
double dotdet(double x1, double y1, double x2, double y2)
{
    return x1*x2 + y1*y2;
}//�������
// �������
double det(double x1, double y1, double x2, double y2)
{
    return x1*y2 - x2*y1;
}//�������

int cross(Point a, Point c, Point d)
{
    return dbcmp( det(a.x-c.x, a.y-c.y, d.x-c.x, d.y-c.y) );
}//������������,a��cd�Ҳ෵��,��෵��-1,���߷���
// �����ж�
bool between(Point a, Point c, Point d)
{
    return dbcmp( dotdet(c.x-a.x, c.y-a.y, d.x-a.x, d.y-a.y) ) != 1;
}//��cross(a, c, d)==0��ǰ����,��a���߶�cd�ڲ�����true
// �ཻ�ж�
int segIntersect(Point a, Point b, Point c, Point d)
{
    int a_cd = cross(a, c, d);
    if (a_cd == 0 && between(a, c, d))
    {
        return 2;
    }

    int b_cd = cross(b, c, d);
    if (b_cd == 0 && between(b, c, d))
    {
        return 2;
    }

    int c_ab = cross(c, a, b);
    if (c_ab == 0 && between(c, a, b))
    {
        return 2;
    }

    int d_ab = cross(d, a, b);
    if (d_ab == 0 && between(d, a, b))
    {
        return 2;
    }

    if ((a_cd ^ b_cd) == -2 && (c_ab ^ d_ab) == -2)
    {
        return 1;
    }

    return 0;
}//���߶��ཻ���,0--���ཻ,1--�淶�ཻ,2--���淶�ཻ(���ڶ˵���غ�)
// �����ཻ����
void intersectPoint(Point a, Point b, Point c, Point d, Point &e)
{
    double sc, sd;

    sc = fabs( det(b.x-a.x, b.y-a.y, c.x-a.x, c.y-a.y) );
    sd = fabs( det(b.x-a.x, b.y-a.y, d.x-a.x, d.y-a.y) );
    e.x = (sc * d.x + sd * c.x) / (sc + sd);
    e.y = (sc * d.y + sd * c.y) / (sc + sd);
}//���߶ι淶�ཻʱ,�õ�e���ؽ���
// �ж϶����͹
bool convex(const std::vector<Point>& v)
{
    Point c, d;
    c=v[0];
    for (int i=1; i<v.size(); ++i)
    {
        d=v[i];
        for (int j=0; j<v.size(); ++j)
            if (!(v[j]==c || v[j]==d))
            {
                if (cross(v[j], c, d)!=-1)
                {
                    return false;
                }
            }
        c=d;
    }
    return true;
}
// �����������
double linesqr(double x1,double y1,double x2,double y2)
{ 
    return (x2-x1)*(y1+y2)/2.0;
}

//clockwise +
double anyS(std::vector<Point>& v)
{
    double fx,fy,x1,y1,x2,y2;
    double s=0.0;
    int n,i;
    n=v.size();
    x1=v[0].m_x;
    y1=v[0].m_y;
    fx=x1;fy=y1;
    x2=v[1].m_x;
    y2=v[1].m_y;
    s=linesqr(x1,y1,x2,y2);
    for(i=2;i<n;++i)
    {
        x1=x2;y1=y2;
        x2=v[i].m_x;
        y2=v[i].m_y;
        s+=linesqr(x1,y1,x2,y2);
    }
    s+=linesqr(x2,y2,fx,fy);//��β����

    return s;
}

