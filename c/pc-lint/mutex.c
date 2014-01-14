

// This is an example of the detection of the misuse of functions
// that have been identified as mutex locking and unlocking functions.

//lint -sem( lock, thread_lock )
//lint -sem( unlock, thread_unlock )

//lint +source  interleave messages amidst the source code
//lint -u (unit checkout)

extern int g();
void lock(void), unlock(void);

int data[20];

void f( int i, int n )
{
    lock();
    unlock();       // great
    //-------------
    lock();
    if( g() )
    {
        unlock();
        return;     // ok
    }
    unlock();       // still ok
    //-------------
    lock();
    if( g() )
        return;     // Warning 454
    unlock();
    //-------------
    if( g() )
    {
        lock();
        unlock();
        unlock();   // Warning 455
        return;
    }
    //-------------
    if( g() )
        lock();
    data[i] = n;    // Warning 456
    if( g() )
        unlock();
}

