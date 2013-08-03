//  (C) Copyright John Maddock 2000. 
//  (C) Copyright Antony Polukhin 2013.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"
#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/is_copy_constructible.hpp>
#endif

#include <boost/move/core.hpp>

struct has {
    has(){}
    has(const has&){}
};

// MSVC can not generate neither default constructor, nor assignment operator, 
// nor copy constructor for `has2` type. Supressing those warnings is essential, 
// because we treat warnings as errors in those tests
#if (defined _MSC_VER)
# pragma warning( push )
# pragma warning( disable : 4510 4512 4610)
#endif
struct has2 {
    const int& i;
    explicit has2(const int& val) : i(val) {}
};
#if (defined _MSC_VER)
# pragma warning( pop )
#endif

struct has3 {
    has3(has3&){}
};


struct has4 { // Copy constructor must be generated by compiler
    has4(has4*){}
};

struct has_not: public boost::noncopyable {
    typedef boost::noncopyable base_t;
    has_not() : base_t() {}
};

#ifndef BOOST_NO_CXX11_DELETED_FUNCTIONS

struct has_not2 {
    has_not2() {}
    has_not2(has_not2&) = delete;
};

struct has_not3 {
    has_not3() {}
    has_not3(const has_not3&) = delete;
};

#endif // BOOST_NO_CXX11_DELETED_FUNCTIONS

struct has_not4: private boost::noncopyable {
    typedef boost::noncopyable base_t;
    has_not4() : base_t() {}
private:
    has_not4(const has_not4&);
};

struct has_not5 {
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(has_not5)
};


TT_TEST_BEGIN(is_copy_constructible)

// Main part of the test
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has2>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has3>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has4>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has_not>::value, false);
#ifndef BOOST_NO_CXX11_DELETED_FUNCTIONS
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has_not2>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has_not3>::value, false);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has_not4>::value, false);

// Requires some basic support from Boost.Move in C++03
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<has_not5>::value, false);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<bool>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<bool const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<bool volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<bool const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<signed char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<signed char const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<signed char volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<signed char const volatile>::value, true);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned char const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<char const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned char volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<char volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned char const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<char const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned short>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<short>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned short const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<short const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned short volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<short volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned short const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<short const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned int const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned int volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned int const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned long>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned long const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned long volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned long const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long const volatile>::value, true);
#endif

#ifdef BOOST_HAS_LONG_LONG

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::ulong_long_type>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::long_long_type>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::ulong_long_type const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::long_long_type const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::ulong_long_type volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::long_long_type volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::ulong_long_type const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible< ::boost::long_long_type const volatile>::value, true);
#endif
#endif

#ifdef BOOST_HAS_MS_INT64

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int8>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int8>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int8 const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int8 const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int8 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int8 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int8 const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int8 const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int16>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int16>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int16 const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int16 const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int16 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int16 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int16 const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int16 const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int32>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int32>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int32 const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int32 const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int32 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int32 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int32 const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int32 const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int64>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int64>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int64 const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int64 const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int64 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int64 volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<unsigned __int64 const volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<__int64 const volatile>::value, true);
#endif
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<float>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<float const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<float volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<float const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<double const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<double volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<double const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long double const>::value, true);
#ifndef TEST_STD
// unspecified behaviour:
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long double volatile>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<long double const volatile>::value, true);
#endif

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<void*>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int*const>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<f1>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<f2>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<f3>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<mf1>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<mf2>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<mf3>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<mp>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<cmf>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<enum_UDT>::value, true);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int&>::value, true);
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
// Code like `int&& a = 10;` or 
//      struct nonc {
//          nonc() = default;
//          nonc(const nonc&) = delete;
//          nonc(nonc&&) = delete;
//          nonc& operator=(const nonc&) = delete;
//          nonc& operator=(nonc&&) = delete;
//      };
//
//      nonc && a = nonc();
// is legal in C++11. so this trait MUST return true.
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int&&>::value, true);
#endif
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<const int&>::value, true);


// Following three tests may give different results because of compiler and C++03/C++11.
// On C++11 compiler following code:
//      int c[2][4][5][6][3];
//      int b[2][4][5][6][3] = std::move(c);
// does not compile, so we expect `false` to be the result of those three tests.
BOOST_CHECK_SOFT_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int[2]>::value, false, true);
BOOST_CHECK_SOFT_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int[3][2]>::value, false, true);
BOOST_CHECK_SOFT_INTEGRAL_CONSTANT(::tt::is_copy_constructible<int[2][4][5][6][3]>::value, false, true);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<void>::value, false);

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<empty_POD_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<POD_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<POD_union_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<empty_POD_union_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<nothrow_copy_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<nothrow_assign_UDT>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<nothrow_construct_UDT>::value, true);

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES) && !defined(BOOST_NO_CXX11_NOEXCEPT)
// Copy constructor of nothrow_move_UDT is implicitly deleted because of user declared move constructor.
// Hovewer not all compilers implement that feature correctly.
BOOST_CHECK_SOFT_INTEGRAL_CONSTANT(::tt::is_copy_constructible<nothrow_move_UDT>::value, false, true);
#endif

//TODO: What do we need to do in this situation?
//BOOST_CHECK_INTEGRAL_CONSTANT(::tt::is_copy_constructible<test_abc1>::value, true);


TT_TEST_END



