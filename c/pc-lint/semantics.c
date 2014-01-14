
// The following example illustrates the use of the semantic option.
//lint -sem( fill, 1p >= N )  fill's argument points to an area that
//                            must be at least N in length.
//lint -sem( copy, @p == 1p ) copy's return value points to an area
//                            equal in size to that of the area
//                            pointed to by copy's first argument.
//
// Note:
// ----
//   "1p" is the length of a region to which the first argument
//        of the given function is pointing.
//   "@p" is the length of a region to which the return value
//        of the given function is pointing.

#include <stdlib.h>
#include <string.h>
void fill( char * );
char *copy( char * );

#define N 100

void f()
{
    char buffer[N];
    char *p;
    char *q;

    p = (char *) malloc( N );
    p++;
    fill( buffer );
    fill( p );
    q = copy( p );
    strcpy( q, buffer );
    free( buffer );
    free( p );
}

