
#include <string.h>

class X
{
    int *p;
public:
    X()
    { p = new int[20]; }
    void init()
    { memset( p, 20, 'a'  ); }
    ~X()
    { delete p; }
};
