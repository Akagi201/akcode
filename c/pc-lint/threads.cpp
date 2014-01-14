
// In this multi-threading example, the programmer has specified that
// reader() is the head of a thread;  Also the constructor and destructor
// of Lock are given the properties of mutex locking and unlocking
// respectively.

//lint -sem( reader, thread )
//lint -sem( Lock::Lock, thread_lock )
//lint -sem( Lock::~Lock, thread_unlock )

// When reader() invokes h() it is made from a critical section within g().
// When main() invokes h() it does so outside of any critical section
// and for this reason warnings are issued.

//lint -esym( 1502, Lock )  Lock is memberless
//lint -esym( 1788, Lock )  Lock's are invoked only by constructors
//                          and destructors

int x;
int y;
void create(...);
struct Lock
{
    Lock();
    ~Lock();
};

void g(void);
void h(void);

void reader() { g(); }

void g(void)
{
    Lock lock;
    y = 1;
    h();
}

void h(void) { x = y; }

int main()
{
    create( reader );
    h();
    return 0;
}

