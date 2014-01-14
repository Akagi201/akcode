
/* General Test Example */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define Extract(ch) (ch) & 0xFf
#define Value(ch) ((ch) > '0' && \
    (ch) <= '9' ? (ch) - '0' : 0)
#define Abs(x) ( (x) < 0 ? -x : (x) )

void readline( char *fn )
{
    FILE *f;
    char buf[100];

    if( !fn ) printf( "bad file\n" );
    f = fopen( fn, "r" );
    (void) fgets( buf, 101, f );
    fclose( f );
}

int compute( char *s )
{
    int sum = 0;
    while( *s )
        sum = sum + Value(Extract(*s++));
    return Abs( sum - 100 );
}

class String
{
private:
    char *a;
    unsigned len;
public:
    String( char *s = 0 )
    {
        if( s )
        {
            len = strlen(s);
            a = new char[len];
            strcpy( a, s );
        }
    }
    ~String() { len = 0; }
    String( const String & );
    String & operator=( const String &s )
    {
        len = s.len;
        a = new char[len];
        memcpy( a, s.a, len );
        return s;
    }
};

