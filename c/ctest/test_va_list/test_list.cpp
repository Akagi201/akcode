#include <iostream>

using namespace std;

void fun(int a, ...)
{
    int *temp = &a;
    ++temp;
    for (int i = 0; i < a; ++i)
    {
        cout << *temp << endl;
        ++temp;
    }
}

int main()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    fun(4, a, b, c, d);
   // system("pause");
    return 0;
}
