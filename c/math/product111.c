
#include <stdio.h>
#include <math.h>

int main()
{
    int i;
    double a[10] = {11111, 111111, 111111111};
    for (i = 0; i < 3; ++i)
    {
        printf("%lf\n", a[i] * a[i]);
    }
    return 0;
}
