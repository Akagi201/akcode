#include <stdio.h>
#include <stdarg.h>

void print_err(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

void print_usage()
{
    print_err("Usage: client"," ip port filename\n");
}

int main(int argc, char *argv[])
{
    if(1)
    {
        print_usage();
    }
    return 0;
}
