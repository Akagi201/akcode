
//lint +source

// This example demonstrates dimensional analysis
// We will report whenever a variable (such as v) that is typed
// as a Velocity is assigned anything other than a Velocity or
// a Met/Sec

// This example also demonstrates source echoing (triggered by
// the +source option)

//lint -e838 Suppress "Previously assigned value not used"

//lint -strong( AcJcX, Met, Sec, Velocity = Met/Sec )
typedef double Met, Sec, Velocity;

Velocity speed( Met d, Sec t )
{
    Velocity v;
    v = d / t;          // ok
    v = 1 / t;          // warning
    v = (3.5/t) * d;    // ok
    v = (1/(t*t)) * d * t;  // ok
    return v;           // ok
}

