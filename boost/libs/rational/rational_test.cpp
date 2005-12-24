/*
 *  A test program for boost/rational.hpp.
 *  Change the typedef at the beginning of run_tests() to try out different
 *  integer types.  (These tests are designed only for signed integer
 *  types.  They should work for short, int and long.)
 *
 *  (C) Copyright Stephen Silver, 2001. Permission to copy, use, modify, sell
 *  and distribute this software is granted provided this copyright notice
 *  appears in all copies. This software is provided "as is" without express or
 *  implied warranty, and with no claim as to its suitability for any purpose.
 *
 *  Incorporated into the boost rational number library, and modified and
 *  extended, by Paul Moore, with permission.
 */

// Revision History
// 23 Dec 05  Change code to use Boost.Test (Daryle Walker)
// 04 Mar 01  Patches for Intel C++ and GCC (David Abrahams)

#define BOOST_TEST_MAIN  "Boost::Rational unit tests"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/stringize.hpp>
#include "boost/rational.hpp"
#include "boost/operators.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <sstream>

// We can override this on the compile, as -DINT_TYPE=short or whatever.
// The default test is against rational<long>.
#ifndef INT_TYPE
#define INT_TYPE long
#endif

namespace {

// This is a trivial user-defined wrapper around the built in int type.
// It can be used as a test type for rational<>
class MyInt : boost::operators<MyInt>
{
    int val;
public:
    MyInt(int n = 0) : val(n) {}
    friend MyInt operator+ (const MyInt&);
    friend MyInt operator- (const MyInt&);
    MyInt& operator+= (const MyInt& rhs) { val += rhs.val; return *this; }
    MyInt& operator-= (const MyInt& rhs) { val -= rhs.val; return *this; }
    MyInt& operator*= (const MyInt& rhs) { val *= rhs.val; return *this; }
    MyInt& operator/= (const MyInt& rhs) { val /= rhs.val; return *this; }
    MyInt& operator%= (const MyInt& rhs) { val %= rhs.val; return *this; }
    MyInt& operator|= (const MyInt& rhs) { val |= rhs.val; return *this; }
    MyInt& operator&= (const MyInt& rhs) { val &= rhs.val; return *this; }
    MyInt& operator^= (const MyInt& rhs) { val ^= rhs.val; return *this; }
    const MyInt& operator++() { ++val; return *this; }
    const MyInt& operator--() { --val; return *this; }
    bool operator< (const MyInt& rhs) const { return val < rhs.val; }
    bool operator== (const MyInt& rhs) const { return val == rhs.val; }
    bool operator! () const { return !val; }
    friend std::istream& operator>>(std::istream&, MyInt&);
    friend std::ostream& operator<<(std::ostream&, const MyInt&);
};

inline MyInt operator+(const MyInt& rhs) { return rhs; }
inline MyInt operator-(const MyInt& rhs) { return MyInt(-rhs.val); }
inline std::istream& operator>>(std::istream& is, MyInt& i) { is >> i.val; return is; }
inline std::ostream& operator<<(std::ostream& os, const MyInt& i) { os << i.val; return os; }
inline MyInt abs(MyInt rhs) { if (rhs < MyInt()) rhs = -rhs; return rhs; }

#define CHECK(x)  BOOST_CHECK( x )

class rational_size_check
{
    typedef INT_TYPE                          int_type;
    typedef ::boost::rational<int_type>  rational_type;

public:
    rational_size_check()
    {
        using ::std::cout;

        char const * const  int_name = BOOST_PP_STRINGIZE( INT_TYPE );

        cout << "Running tests for boost::rational<" << int_name << ">\n\n";

        cout << "Implementation issue: the minimal size for a rational\n"
             << "is twice the size of the underlying integer type.\n\n";

        cout << "Checking to see if space is being wasted.\n"
             << "\tsizeof(" << int_name << ") == " << sizeof( int_type )
             << "\n";
        cout <<  "\tsizeof(boost::rational<" << int_name << ">) == "
             << sizeof( rational_type ) << "\n\n";

        cout << "Implementation has "
             << ( 
                  (sizeof( rational_type ) > 2u * sizeof( int_type ))
                  ? "included padding bytes"
                  : "minimal size"
                )
             << "\n\n";
    }
};

// The basic test suite
BOOST_AUTO_TEST_CASE( rational_test )
{
    typedef INT_TYPE IntType;
    typedef boost::rational<IntType> rat;

    /* gcd tests */
    CHECK(( boost::gcd<IntType>(1,-1) == 1     ));
    CHECK(( boost::gcd<IntType>(-1,1) == 1     ));
    CHECK(( boost::gcd<IntType>(1,1) == 1      ));
    CHECK(( boost::gcd<IntType>(-1,-1) == 1    ));
    CHECK(( boost::gcd<IntType>(0,0) == 0      ));
    CHECK(( boost::gcd<IntType>(7,0) == 7      ));
    CHECK(( boost::gcd<IntType>(0,9) == 9      ));
    CHECK(( boost::gcd<IntType>(-7,0) == 7     ));
    CHECK(( boost::gcd<IntType>(0,-9) == 9     ));
    CHECK(( boost::gcd<IntType>(42,30) == 6    ));
    CHECK(( boost::gcd<IntType>(6,-9) == 3     ));
    CHECK(( boost::gcd<IntType>(-10,-10) == 10 ));
    CHECK(( boost::gcd<IntType>(-25,-10) == 5  ));

    /* lcm tests */
    CHECK(( boost::lcm<IntType>(1,-1) == 1     ));
    CHECK(( boost::lcm<IntType>(-1,1) == 1     ));
    CHECK(( boost::lcm<IntType>(1,1) == 1      ));
    CHECK(( boost::lcm<IntType>(-1,-1) == 1    ));
    CHECK(( boost::lcm<IntType>(0,0) == 0      ));
    CHECK(( boost::lcm<IntType>(6,0) == 0      ));
    CHECK(( boost::lcm<IntType>(0,7) == 0      ));
    CHECK(( boost::lcm<IntType>(-5,0) == 0     ));
    CHECK(( boost::lcm<IntType>(0,-4) == 0     ));
    CHECK(( boost::lcm<IntType>(18,30) == 90   ));
    CHECK(( boost::lcm<IntType>(-6,9) == 18    ));
    CHECK(( boost::lcm<IntType>(-10,-10) == 10 ));
    CHECK(( boost::lcm<IntType>(25,-10) == 50  ));

    /* initialization tests */
    rat r1, r2(0), r3(1), r4(-3), r5(7,2), r6(5,15), r7(14,-21),
        r8(-4,6), r9(-14,-70);

    CHECK(( r1.numerator() == 0  ));
    CHECK(( r2.numerator() == 0  ));
    CHECK(( r3.numerator() == 1  ));
    CHECK(( r4.numerator() == -3 ));
    CHECK(( r5.numerator() == 7  ));
    CHECK(( r6.numerator() == 1  ));
    CHECK(( r7.numerator() == -2 ));
    CHECK(( r8.numerator() == -2 ));
    CHECK(( r9.numerator() == 1  ));

    CHECK(( r1.denominator() == 1 ));
    CHECK(( r2.denominator() == 1 ));
    CHECK(( r3.denominator() == 1 ));
    CHECK(( r4.denominator() == 1 ));
    CHECK(( r5.denominator() == 2 ));
    CHECK(( r6.denominator() == 3 ));
    CHECK(( r7.denominator() == 3 ));
    CHECK(( r8.denominator() == 3 ));
    CHECK(( r9.denominator() == 5 ));

    /* assign() tests */
    r1.assign(6,8);
    CHECK(( r1.numerator()   == 3   ));
    CHECK(( r1.denominator() == 4   ));
    r1.assign(0,-7);
    CHECK(( r1.numerator()   == 0   ));
    CHECK(( r1.denominator() == 1   ));

    /* comparison tests */
    CHECK(( r1 == r2 ));
    CHECK(( r2 != r3 ));
    CHECK(( r4 <  r3 ));
    CHECK(( r4 <= r5 ));
    CHECK(( r1 <= r2 ));
    CHECK(( r5 >  r6 ));
    CHECK(( r5 >= r6 ));
    CHECK(( r7 >= r8 ));

    CHECK(( !(r3 == r2) ));
    CHECK(( !(r1 != r2) ));
    CHECK(( !(r1 <  r2) ));
    CHECK(( !(r5 <  r6) ));
    CHECK(( !(r9 <= r2) ));
    CHECK(( !(r8 >  r7) ));
    CHECK(( !(r8 >  r2) ));
    CHECK(( !(r4 >= r6) ));

    CHECK(( r1 == 0  ));
    CHECK(( r2 != -1 ));
    CHECK(( r3 < 2   ));
    CHECK(( r4 <= -3 ));
    CHECK(( r5 > 3   ));
    CHECK(( r6 >= 0  ));

    CHECK(( 0 == r2  ));
    CHECK(( 0 != r7  ));
    CHECK(( -1 < r8  ));
    CHECK(( -2 <= r9 ));
    CHECK(( 1 > r1   ));
    CHECK(( 1 >= r3  ));

    /* increment/decrement tests */
    CHECK(( r1++ == r2 ));
    CHECK(( r1 != r2   ));
    CHECK(( r1 == r3   ));
    CHECK(( --r1 == r2 ));
    CHECK(( r8-- == r7 ));
    CHECK(( r8 != r7   ));
    CHECK(( ++r8 == r7 ));

    /* abs tests */
#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
// This is a nasty hack, required because some compilers do not implement
// "Koenig Lookup". Basically, if I call abs(r), the C++ standard says that
// the compiler should look for a definition of abs in the namespace which
// contains r's class (in this case boost) - among other places.

using boost::abs;
#endif

    CHECK(( abs(r2) == r2       ));
    CHECK(( abs(r5) == r5       ));
    CHECK(( abs(r8) == rat(2,3) ));

    /* addition/subtraction tests */
    CHECK(( rat(1,2) + rat(1,2)  == 1                   ));
    CHECK(( rat(11,3) + rat(1,2) == rat(25,6)           ));
    CHECK(( rat(-8,3) + rat(1,5) == rat(-37,15)         ));
    CHECK(( rat(-7,6) + rat(1,7) == rat(1,7) - rat(7,6) ));
    CHECK(( rat(13,5) - rat(1,2) == rat(21,10)          ));
    CHECK(( rat(22,3) + 1        == rat(25,3)           ));
    CHECK(( rat(12,7) - 2        == rat(-2,7)           ));
    CHECK(( 3 + rat(4,5)         == rat(19,5)           ));
    CHECK(( 4 - rat(9,2)         == rat(-1,2)           ));
    rat r0(11);
    r0 -= rat(20,3);
    CHECK(( r0 == rat(13,3)  ));
    r0 += rat(1,2);
    CHECK(( r0 == rat(29,6)  ));
    r0 -= 5;
    CHECK(( r0 == rat(1,-6)  ));
    r0 += rat(1,5);
    CHECK(( r0 == rat(1,30)  ));
    r0 += 2;
    CHECK(( r0 == rat(61,30) ));

    /* assignment tests */
    r0 = rat(1,10);
    CHECK(( r0 == rat(1,10) ));
    r0 = -9;
    CHECK(( r0 == rat(-9,1) ));

    /* unary operator tests */
    CHECK(( +r5 == r5    ));
    CHECK(( -r3 != r3    ));
    CHECK(( -(-r3) == r3 ));
    CHECK(( -r4 == 3     ));
    CHECK(( !r2          ));
    CHECK(( !!r3         ));

    /* multiplication tests */
    CHECK(( rat(1,3) * rat(-3,4) == rat(-1,4) ));
    CHECK(( rat(2,5) * 7         == rat(14,5) ));
    CHECK(( -2 * rat(1,6)        == rat(-1,3) ));
    r0 = rat(3,7);
    r0 *= 14;
    CHECK(( r0 == 6 ));
    r0 *= rat(3,8);
    CHECK(( r0 == rat(9,4) ));

    /* division tests */
    CHECK(( rat(-1,20) / rat(4,5) == rat(-1,16) ));
    CHECK(( rat(5,6) / 7          == rat(5,42)  ));
    CHECK(( 8 / rat(2,7)          == 28         ));
    r0 = rat(4,3);
    r0 /= rat(5,4);
    CHECK(( r0 == rat(16,15) ));
    r0 /= 4;
    CHECK(( r0 == rat(4,15) ));
    CHECK(( rat(-1)/rat(-3) == rat(1,3) ));

    /* tests for operations on self */
    r0 = rat(4,3);
    r0 += r0;
    CHECK(( r0 == rat(8,3) ));
    r0 *= r0;
    CHECK(( r0 == rat(64,9) ));
    r0 /= r0;
    CHECK(( r0 == rat(1,1) ));
    r0 -= r0;
    CHECK(( r0 == rat(0,1) ));

    /* operator<< and operator>> tests */
    std::ostringstream oss;
    
    oss << rat(44,14);
    CHECK(( oss.str() == "22/7" ));
    typedef std::istringstream input_string_stream;
    {
        input_string_stream iss("");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("42");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("57A");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("20-20");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("1/");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("1/ 2");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        input_string_stream iss("1 /2");
        iss >> r0;
        CHECK(( !iss ));
    }
    {
        int n;
        input_string_stream iss("1/2 12");
        CHECK(( iss >> r0 >> n ));
        CHECK(( r0 == rat(1,2) ));
        CHECK(( n == 12 ));
    }
    {
        input_string_stream iss("34/67");
        CHECK(( iss >> r0 ));
        CHECK(( r0 == rat(34,67) ));
    }
    {
        input_string_stream iss("-3/-6");
        CHECK(( iss >> r0 ));
        CHECK(( r0 == rat(1,2) ));
    }

    /* rational cast tests */
    /* Note that these are not generic. The problem is that rational_cast<T>
     * requires a conversion from IntType to T. However, for a user-defined
     * IntType, it is not possible to define such a conversion except as
     * an operator T(). This causes problems with overloading resolution.
     */
    {
        boost::rational<int> half(1,2);
        CHECK(( boost::rational_cast<double>(half) == 0.5 ));
        CHECK(( boost::rational_cast<int>(half) == 0 ));
    }

    /* End of main tests.  */

    /* mean number of times a fair 6-sided die must be thrown
       until each side has appeared at least once */
    r0 = IntType(0);
    for (int i=1; i<=6; ++i)
        r0 += rat(1,i);
    r0 *= 6;
    CHECK(( r0 == rat(147,10) ));
}

} // namespace

BOOST_GLOBAL_FIXTURE( rational_size_check );
