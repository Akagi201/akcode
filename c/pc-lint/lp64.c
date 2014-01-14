
//              64-bit mode
//
// The machine on which this demo takes place is a 32-bit machine
// and hence a full 64 bit analysis is impossible.  If 64-bit
// analysis is required then we recommend compiling FlexeLint directly
// on the target machine.
//
// Nonetheless, even on a 32-bit machine, a limited amount of
// checking is available to determine if a program is consistent
// with a particular 64-bit system.  We are assuming in this example
// that the compiler is using what is called LP64.  That is, long's
// and pointers are 64 bits but integers remain at 32.
// Accordingly we employed the following options (in an options file
// preceding this one):
//
// -sl8  ==  sizeof(long) == 8
// -sp8  ==  sizeof(a pointer) == 8
// +fzl  ==  the type of sizeof() is long
// +fdl  ==  pointer differences are long.

// Probably the biggest source of error in porting from the 32 bit
// model to the 64 bit model is a hidden assumption that an int is
// the same size as a long or a pointer.  The following contains
// examples of such bugs.

#include <stdio.h>
#include <memory.h>

// this function uses integer arithmetic to align a pointer to a 4
// byte boundary
int *allign( int *p )
{
    // the programmer should have used size_t here.
    unsigned int n = (unsigned) p;
    if( n % 4 ) n = n + (4 - (n % 4));
    return (int *) n;
}

// this innocent looking function now has a serious problem
// if offsets can approach the maximum integer.

unsigned long addbase( unsigned long base, int offset )
{
    base += offset * 32;
    return base;
}

// printf-scanf offer a number of interesting bug-inducing
// situations.

void f( long x )
{
    long y;
    scanf( "%d", &y );
    printf( "%d %d\n", x, y );
}

// pointer differences now require a new type.

int diff( const int *p, const int *q )
{
    int n = p - q;
    return n;
}

struct A { long n, m; };

// There may be hidden assumptions on the size of struct's

char *stringize( const struct A *p )
{
    static char buf[ 8 ];      // should have used sizeof
    memcpy( buf, p, sizeof( struct A ) );
    return buf;
}

