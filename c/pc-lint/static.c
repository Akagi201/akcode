
// What did the programmer forget to do in this example?
// It requires 3 passes and a static depth of 3 to find out.

//lint -passes(3) -static_depth(3)

int *p;

int f()
{
    return g();
}

int star_p()
{
    return *p;
}

int g()
{
    return star_p();
}

int main()
{
    return f();
}

