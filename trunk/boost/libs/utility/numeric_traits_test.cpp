//  (C) Copyright David Abrahams 2001. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  21 Jan 2001 Initial version (David Abrahams)

#include <boost/detail/numeric_traits.hpp>
#include <cassert>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/utility.hpp>
#include <boost/lexical_cast.hpp>
#include <climits>
#include <typeinfo>
#include <iostream>
#include <string>
#ifndef BOOST_NO_LIMITS
# include <limits>
#endif

// A macro for declaring class compile-time constants.
#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
# define DECLARE_CLASS_CONST(type, init) static const type init
#else
# define DECLARE_CLASS_CONST(type, init) enum { init }
#endif

// template class complement_traits<Number> --
//
//    statically computes the max and min for 1s and 2s-complement binary
//    numbers. This helps on platforms without <limits> support. It also shows
//    an example of a recursive template that works with MSVC!
//

template <unsigned size> struct complement; // forward

// The template complement, below, does all the real work, using "poor man's
// partial specialization". We need complement_traits_aux<> so that MSVC doesn't
// complain about undefined min/max as we're trying to recursively define them. 
template <class Number, unsigned size>
struct complement_traits_aux
{
    DECLARE_CLASS_CONST(Number, max = complement<size>::template traits<Number>::max);
    DECLARE_CLASS_CONST(Number, min = complement<size>::template traits<Number>::min);
};

template <unsigned size>
struct complement
{
    template <class Number>
    struct traits
    {
     private:
        // indirection through complement_traits_aux neccessary to keep MSVC happy
        typedef complement_traits_aux<Number, size - 1> prev;
     public:
        DECLARE_CLASS_CONST(Number, max =
                            Number(Number(prev::max) << CHAR_BIT)
                            + Number(UCHAR_MAX));
        
        DECLARE_CLASS_CONST(Number, min = Number(Number(prev::min) << CHAR_BIT));
    };
};

// Template class complement_base<> -- defines values for min and max for
// complement<1>, at the deepest level of recursion. Uses "poor man's partial
// specialization" again.
template <bool is_signed> struct complement_base;

template <> struct complement_base<false>
{
    template <class Number>
    struct values
    {
        DECLARE_CLASS_CONST(Number, min = 0);
        DECLARE_CLASS_CONST(Number, max = UCHAR_MAX);
    };
};

template <> struct complement_base<true>
{
    template <class Number>
    struct values
    {
        DECLARE_CLASS_CONST(Number, min = SCHAR_MIN);
        DECLARE_CLASS_CONST(Number, max = SCHAR_MAX);
    };
};

// Base specialization of complement, puts an end to the recursion.
template <>
struct complement<1>
{
    template <class Number>
    struct traits
    {
        DECLARE_CLASS_CONST(bool, is_signed = boost::detail::is_signed<Number>::value);
        DECLARE_CLASS_CONST(Number, min =
                            complement_base<is_signed>::template values<Number>::min);
        DECLARE_CLASS_CONST(Number, max =
                            complement_base<is_signed>::template values<Number>::max);
    };
};

// Now here's the "pretty" template you're intended to actually use.
//   complement_traits<Number>::min, complement_traits<Number>::max are the
//   minimum and maximum values of Number if Number is a built-in integer type.
template <class Number>
struct complement_traits
{
    DECLARE_CLASS_CONST(Number, max = (complement_traits_aux<Number, sizeof(Number)>::max));
    DECLARE_CLASS_CONST(Number, min = (complement_traits_aux<Number, sizeof(Number)>::min));
};

// Support for streaming various numeric types in exactly the format I want. I
// needed this in addition to all the assertions so that I could see exactly
// what was going on.
//
// Numbers go through a 2-stage conversion process (by default, though, no real
// conversion).
//
template <class T> struct stream_as {
    typedef T t1;
    typedef T t2;
};

// char types first get converted to unsigned char, then to unsigned.
template <> struct stream_as<char> {
    typedef unsigned char t1;
    typedef unsigned t2;
};
template <> struct stream_as<unsigned char> {
    typedef unsigned char t1; typedef unsigned t2;
};
template <> struct stream_as<signed char>  {
    typedef unsigned char t1; typedef unsigned t2;
};

#if defined(BOOST_MSVC) // No intmax streaming built-in

// On this platform, __int64 and __uint64 get streamed as strings
template <> struct stream_as<boost::uintmax_t> {
    typedef std::string t1;
    typedef std::string t2;
};

template <> struct stream_as<boost::intmax_t>  {
    typedef std::string t1;
    typedef std::string t2;
};
#endif

// Standard promotion process for streaming
template <class T> struct promote
{
    typename stream_as<T>::t1 static from(T x) {
        typedef typename stream_as<T>::t1 t1;
        return t1(x);
    }
};

#if defined(BOOST_MSVC) // No intmax streaming built-in

// On this platform, stream them as long/unsigned long if they fit.
// Otherwise, write a string.
template <> struct promote<boost::uintmax_t> {
    std::string static from(const boost::uintmax_t x) {
        if (x > ULONG_MAX)
            return std::string("large unsigned value");
        else
            return boost::lexical_cast<std::string>((unsigned long)x);
    }
};
template <> struct promote<boost::intmax_t> {
    std::string static from(const boost::intmax_t x) {
        if (x > boost::intmax_t(ULONG_MAX))
            return std::string("large positive signed value");
        else if (x >= 0)
            return boost::lexical_cast<std::string>((unsigned long)x);
        
        if (x < boost::intmax_t(LONG_MIN))
            return std::string("large negative signed value");
        else
            return boost::lexical_cast<std::string>((long)x);
    }
};
#endif

// This is the function which converts types to the form I want to stream them in.
template <class T>
typename stream_as<T>::t2 stream_number(T x)
{
    return promote<T>::from(x);
}

//
// Tests for built-in signed and unsigned types
//

// Tag types for selecting tests
struct unsigned_tag {};
struct signed_tag {};

// Tests for unsigned numbers. The extra default Number parameter works around
// an MSVC bug.
template <class Number>
void test_aux(unsigned_tag, Number* = 0)
{
    typedef typename boost::detail::numeric_traits<Number>::difference_type difference_type;
    BOOST_STATIC_ASSERT(!boost::detail::is_signed<Number>::value);
    BOOST_STATIC_ASSERT(
        (sizeof(Number) < sizeof(boost::intmax_t))
        | (boost::is_same<difference_type, boost::intmax_t>::value));

    // Force casting to Number here to work around the fact that it's an enum on MSVC
    BOOST_STATIC_ASSERT(Number(complement_traits<Number>::max) > Number(0));
    BOOST_STATIC_ASSERT(Number(complement_traits<Number>::min) == Number(0));
    
    const Number max = complement_traits<Number>::max;
    const Number min = complement_traits<Number>::min;
    
    const Number test_max = (sizeof(Number) < sizeof(boost::intmax_t))
        ? max
        : max / 2 - 1;

    std::cout << std::hex << "(unsigned) min = " << stream_number(min) << ", max = "
              << stream_number(max) << "..." << std::flush;
    std::cout << "difference_type = " << typeid(difference_type).name() << "..."
              << std::flush;
    
    difference_type d1 = boost::detail::numeric_distance(Number(0), test_max);
    difference_type d2 = boost::detail::numeric_distance(test_max, Number(0));
    
    std::cout << "0->" << stream_number(test_max) << "==" << std::dec << stream_number(d1) << "; "
              << std::hex << stream_number(test_max) << "->0==" << std::dec << stream_number(d2) << "..." << std::flush;

    assert(d1 == difference_type(test_max));
    assert(d2 == -difference_type(test_max));
}

// Tests for signed numbers. The extra default Number parameter works around an
// MSVC bug.
template <class Number>
void test_aux(signed_tag, Number* = 0)
{
    typedef typename boost::detail::numeric_traits<Number>::difference_type difference_type;
    BOOST_STATIC_ASSERT(boost::detail::is_signed<Number>::value);
    BOOST_STATIC_ASSERT(
        (sizeof(Number) < sizeof(boost::intmax_t))
        | (boost::is_same<difference_type, Number>::value));

    // Force casting to Number here to work around the fact that it's an enum on MSVC
    BOOST_STATIC_ASSERT(Number(complement_traits<Number>::max) > Number(0));
    BOOST_STATIC_ASSERT(Number(complement_traits<Number>::min) < Number(0));
    
    const Number max = complement_traits<Number>::max;
    const Number min = complement_traits<Number>::min;
    
    std::cout << std::hex << "min = " << stream_number(min) << ", max = "
              << stream_number(max) << "..." << std::flush;
    std::cout << "difference_type = " << typeid(difference_type).name() << "..."
              << std::flush;
    assert(min < max);

    difference_type d1 = boost::detail::numeric_distance(min, max);
    difference_type d2 = boost::detail::numeric_distance(max, min);
    if (sizeof(Number) < sizeof(boost::intmax_t))
    {
        std::cout << stream_number(min) << "->" << stream_number(max) << "==";
        std::cout << std::dec << stream_number(d1) << "; ";
        std::cout << std::hex << stream_number(max) << "->" << stream_number(min)
                  << "==" << std::dec << stream_number(d2) << "..." << std::flush;
        assert(d1 == difference_type(max) - difference_type(min));
        assert(d2 == difference_type(min) - difference_type(max));
    }
}


// Test for all numbers. The extra default Number parameter works around an MSVC
// bug.
template <class Number>
void test(Number* = 0)
{
    std::cout << "testing " << typeid(Number).name() << "..." << std::flush;
    typedef typename boost::detail::numeric_traits<Number>::difference_type difference_type;
    BOOST_STATIC_ASSERT(boost::detail::is_signed<difference_type>::value);

    typedef typename boost::detail::if_true<
        boost::detail::is_signed<Number>::value
        >::template then<signed_tag, unsigned_tag>::type signedness;
    
    test_aux<Number>(signedness());
    std::cout << "passed" << std::endl;
}

int main()
{
    test<char>();
    test<unsigned char>();
    test<signed char>();
    test<short>();
    test<unsigned short>();
    test<int>();
    test<unsigned int>();
    test<long>();
    test<unsigned long>();
#if defined(ULLONG_MAX) || defined(ULONG_LONG_MAX)
    test<long long>();
    test<unsigned long long>();
#elif defined(BOOST_MSVC)
    // The problem of not having compile-time static class constants other than
    // enums prevents this from working, since values get truncated.
    // test<boost::uintmax_t>();
    // test<boost::intmax_t>();
#endif
    return 0;
}
