// externc.cpp

#include <iostream>

extern "C" double sqrt(double);

int main()
{
    using std::cout;
    using std::endl;
    double result = sqrt(25);
    cout<<"result = "<<result<<endl;
    return 0;
}
