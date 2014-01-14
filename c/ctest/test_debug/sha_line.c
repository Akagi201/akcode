#include <stdio.h>

#line 200 //通过#line重新设定__LINE__的值, 指定下一行的__LINE__为200

int main()
{
    printf("%d\n", __LINE__);
    printf("%d\n", __LINE__);
    printf("%d\n", __LINE__);
    return 0;
}
