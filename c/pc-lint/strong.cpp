
// This is a very simple use of the Strong Type mechanism.
// By declaring typedef's strong, FlexeLint can issue reports
// when a mismatch occurs with the nominal type.

// There are several errors reported below in the function combine()
// where integers of the wrong flavor are compared or assigned.

// The following option specifies three strong typedef names
// (to be defined subsequently) that have the characteristic
// AcJcX.  This means report when there is a conflict in Assigning
// to a strong typedef (but not when c-onstants are involved),
// when Joining with a strong typedef (but not when constants are
// involved) and when extracting from a strong typedef.

//lint -strong( AcJcX, Length, H_Position, V_Position )

#define Max(x,y) ((x) >= (y) ? (x) : (y))
#define Min(x,y) ((x) >= (y) ? (y) : (x))

// The following three typedef's create a type hierarchy with
// Length as the root and H_Position and V_Position as the 'derived'
// types
typedef int Length;
typedef Length H_Position;
typedef Length V_Position;

struct Point
{
    H_Position x;       // horizontal position
    V_Position y;       // vertical position
};

struct Rectangle
{
    Point ul;           // upper left boundary
    Point lr;           // lower right boundary
};

// The following function computes without complaint.  Length is
// defined as a generic form of H_Position and V_Position because
// of the typedef's above.  There is no problem when combining a
// generic type with a type lower on the hierarchy

Length perimeter( const Rectangle &r )
{
    Length  len = 0;
    len += 2 * (r.ul.y - r.lr.y);  // combining V_Position with Length
    len += 2 * (r.lr.x - r.ul.x);  // combining H_Position with Length
    return len;
}

// But the following function draws a number of justifiable complaints.
// In this function, the two types H_Position and V_Position need to be
// treated as two separate and distinct types that should not be
// combined.  When they are, warnings are issued.

// combine(r1,r2) is intended to form one large rectangle that will
// just cover the rectangles r1 and r2.

Rectangle combine( const Rectangle &r1, const Rectangle &r2 )
{
    Rectangle r;

    r.ul.x = Min( r1.ul.x, r2.ul.x );   // OK
    r.ul.y = Max( r1.ul.y, r2.ul.y );   // OK
    r.lr.x = Max( r1.lr.y, r2.lr.x );   // Complain
    r.lr.y = Min( r1.lr.x, r2.lr.x );   // Complain
    return r;
}

