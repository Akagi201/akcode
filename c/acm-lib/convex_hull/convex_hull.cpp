      
/** @file convex_hull.cpp
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    凸包问题的快包实现
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

#include <iostream>   //cout, cin
#include <algorithm>  //copy sort
#include <iterator>   //ostream_iterator
#include <vector>     //vector
#include <queue>      //queue
#include <stdlib.h>   //srand rand
#include <time.h>     //time
#include <limits>     //numeric_limits
#include <math.h>     //sqrt
#include <assert.h>   //assert

class Point
{
public:
    Point( int x=0, int y=0 ) : m_x(x), m_y(y) {}
public:
    int m_x;
    int m_y;
};

std::istream& operator>> (std::istream& in, Point& p)
{
    return (in>>p.m_x>>p.m_y);
}

std::ostream& operator<< (std::ostream& out, const Point& p)
{
    return (out<<"x="<<p.m_x<<" y="<<p.m_y);
}

class CmpXY
{
public:
    bool operator() (const Point& l, const Point& r)
    {
        bool ret;
        if ( l.m_x<r.m_x )
        {
            ret=true;
        }
        else if ( l.m_x==r.m_x )
        {
            if ( l.m_y<r.m_y )
            {
                ret=true;
            }
            else
            {
                ret=false;
            }
        }
        else
        {
            ret=false;
        }
        return ret;
    }
};

void findup(std::vector<Point>& in, const Point& p1, const Point& p2, std::vector<Point>& out)
{
    std::vector<Point> up;  //新上包点
    Point upp; //极点
    int uph=0; //极高
    int h;
    std::vector<Point>::iterator iter=in.begin();
    while ( iter!=in.end() )
    {
        h=p1.m_x*p2.m_y+iter->m_x*p1.m_y+p2.m_x*iter->m_y-iter->m_x*p2.m_y-p2.m_x*p1.m_y-p1.m_x*iter->m_y;
        if ( h>0 )  //p3在直线p1p2左边在上包中
        {
            up.push_back(*iter);
            if ( h>uph )
            {
                uph=h;
                upp=*iter;
            }
        }
        ++iter;
    }
    if ( uph>0 )
    {
        out.push_back(upp);
    }
    //递归构造新上包
    if ( !up.empty() )
    {
        findup(up, p1, upp, out);
        findup(up, upp, p2, out);
    }
}

void finddown(std::vector<Point>& in, const Point& p1, const Point& p2, std::vector<Point>& out)
{
    std::vector<Point> down; //新下包点
    Point downp;	//极点
    int downh=0;	//极高
    int h;
    std::vector<Point>::iterator iter=in.begin();
    while ( iter!=in.end() )
    {
        h=p1.m_x*p2.m_y+iter->m_x*p1.m_y+p2.m_x*iter->m_y-iter->m_x*p2.m_y-p2.m_x*p1.m_y-p1.m_x*iter->m_y;
        if ( h<0 ) //p3在直线p1p2右边在下包中
        {
            down.push_back(*iter);
            if ( h<downh )
            {
                downh=h;
                downp=*iter;
            }
        }
        ++iter;
    }
    if ( downh<0 )
        out.push_back(downp);
    //递归构造新下包
    if ( !down.empty() )
    {
        finddown(down, p1, downp, out);
        finddown(down, downp, p2, out);
    }
}

void FindPoint(std::vector<Point>& in, std::vector<Point>& out)
{
    //排序
    std::sort(in.begin(), in.end(), CmpXY());
    Point p1=in.front(); //凸包一点
    Point p2=in.back();  //凸包一点
    out.push_back(p1);
    out.push_back(p2);
    //迭代变量
    std::vector<Point>::iterator iter=in.begin()+1;
    std::vector<Point> up; //上凸包点
    std::vector<Point> down; //下凸包点
    Point upp, downp;  //凸包极点
    int uph=0, downh=0;	 //极点离基线高度
    int h;
    while ( iter!=in.end()-1 )
    {
        h=p1.m_x*p2.m_y+iter->m_x*p1.m_y+p2.m_x*iter->m_y-iter->m_x*p2.m_y-p2.m_x*p1.m_y-p1.m_x*iter->m_y;
        if ( h>0 ) //p3在直线p1p2左边在上包中
        {
            up.push_back(*iter);
            if ( h>uph )
            {
                uph=h;
                upp=*iter;
            }
        }
        if ( h<0 ) //p3在直线p1p2右边在下包中
        {
            down.push_back(*iter);
            if ( h<downh )
            {
                downh=h;
                downp=*iter;
            }
        }
        ++iter;
    }
    if ( uph>0 )
    {
        out.push_back(upp);
    }
    if ( downh<0 )
    {
        out.push_back(downp);
    }
    //构造上包
    if ( !up.empty() )
    {
        findup(up, p1, upp, out);
        findup(up, upp, p2, out);
    }
    //构造下包
    if ( !down.empty() )
    {
        finddown(down, p1, downp, out);
        finddown(down, downp, p2, out);
    }
}

#define SIZE 100000

int main(int argc, char* argv[])
{
    srand(time(0));
    std::vector<Point> in;
    std::vector<Point> out;
    for ( int i=0; i<SIZE; ++i )
    {
        Point p;
        p.m_x=rand()%SIZE;
        p.m_y=rand()%SIZE;
        in.push_back(p);
    }
    //std::copy(in.begin(), in.end(), std::ostream_iterator<Point>(std::cout, " "));
    //system("pause");
    FindPoint(in, out);
    std::copy(out.begin(), out.end(), std::ostream_iterator<Point>(std::cout, " "));
    return 0;
}

