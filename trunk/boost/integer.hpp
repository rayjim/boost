//  boost integer.hpp header file  -------------------------------------------//

//  Copyright Beman Dawes and Daryle Walker 1999.  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/integer for documentation.

//  Revision History
//   22 Sep 01  Added value-based integer templates. (Daryle Walker)
//   01 Apr 01  Modified to use new <boost/limits.hpp> header. (John Maddock)
//   30 Jul 00  Add typename syntax fix (Jens Maurer)
//   28 Aug 99  Initial version

#ifndef BOOST_INTEGER_HPP
#define BOOST_INTEGER_HPP

#include <boost/integer_fwd.hpp>  // self include

#include <boost/integer_traits.hpp>  // for ::boost::integer_traits
#include <boost/limits.hpp>          // for ::std::numeric_limits
#include <boost/cstdint.hpp>         // for boost::int64_t and BOOST_NO_INTEGRAL_INT64_T

namespace boost
{

  //  Helper templates  ------------------------------------------------------//

  //  fast integers from least integers
  //  int_fast_t<> works correctly for unsigned too, in spite of the name.
  template< typename LeastInt >
  struct int_fast_t { typedef LeastInt fast; }; // imps may specialize

  namespace detail{

  //  convert category to type 
  template< int Category > struct int_least_helper {}; // default is empty

  //  specializatons: 1=long, 2=int, 3=short, 4=signed char,
  //     6=unsigned long, 7=unsigned int, 8=unsigned short, 9=unsigned char
  //  no specializations for 0 and 5: requests for a type > long are in error
#ifdef BOOST_HAS_LONG_LONG
  template<> struct int_least_helper<1> { typedef boost::long_long_type least; };
#endif
  template<> struct int_least_helper<2> { typedef long least; };
  template<> struct int_least_helper<3> { typedef int least; };
  template<> struct int_least_helper<4> { typedef short least; };
  template<> struct int_least_helper<5> { typedef signed char least; };
#ifdef BOOST_HAS_LONG_LONG
  template<> struct int_least_helper<6> { typedef boost::ulong_long_type least; };
#endif
  template<> struct int_least_helper<7> { typedef unsigned long least; };
  template<> struct int_least_helper<8> { typedef unsigned int least; };
  template<> struct int_least_helper<9> { typedef unsigned short least; };
  template<> struct int_least_helper<10> { typedef unsigned char least; };

  } // namespace detail

  //  integer templates specifying number of bits  ---------------------------//

  //  signed
  template< int Bits >   // bits (including sign) required
  struct int_t 
  {
      typedef typename detail::int_least_helper
        <
#ifdef BOOST_HAS_LONG_LONG
          (Bits-1 <= (int)(sizeof(boost::long_long_type) * CHAR_BIT)) +
#else
           1 +
#endif
          (Bits-1 <= ::std::numeric_limits<long>::digits) +
          (Bits-1 <= ::std::numeric_limits<int>::digits) +
          (Bits-1 <= ::std::numeric_limits<short>::digits) +
          (Bits-1 <= ::std::numeric_limits<signed char>::digits)
        >::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

  //  unsigned
  template< int Bits >   // bits required
  struct uint_t 
  {
      typedef typename detail::int_least_helper
        < 
          5 +
#ifdef BOOST_HAS_LONG_LONG
          (Bits-1 <= (int)(sizeof(boost::long_long_type) * CHAR_BIT)) +
#else
           1 +
#endif
          (Bits <= ::std::numeric_limits<unsigned long>::digits) +
          (Bits <= ::std::numeric_limits<unsigned int>::digits) +
          (Bits <= ::std::numeric_limits<unsigned short>::digits) +
          (Bits <= ::std::numeric_limits<unsigned char>::digits)
        >::least  least;
      typedef typename int_fast_t<least>::fast  fast;
      // int_fast_t<> works correctly for unsigned too, in spite of the name.
  };

  //  integer templates specifying extreme value  ----------------------------//

  //  signed
#if !defined(BOOST_NO_INTEGRAL_INT64_T) && defined(BOOST_HAS_LONG_LONG)
  template< boost::long_long_type MaxValue >   // maximum value to require support
#else
  template< long MaxValue >   // maximum value to require support
#endif
  struct int_max_value_t 
  {
      typedef typename detail::int_least_helper
        <
#ifdef BOOST_HAS_LONG_LONG
          (MaxValue <= ::boost::integer_traits<boost::long_long_type>::const_max) +
#else
           1 +
#endif
          (MaxValue <= ::boost::integer_traits<long>::const_max) +
          (MaxValue <= ::boost::integer_traits<int>::const_max) +
          (MaxValue <= ::boost::integer_traits<short>::const_max) +
          (MaxValue <= ::boost::integer_traits<signed char>::const_max)
        >::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

#if !defined(BOOST_NO_INTEGRAL_INT64_T) && defined(BOOST_HAS_LONG_LONG)
  template< boost::long_long_type MinValue >   // minimum value to require support
#else
  template< long MinValue >   // minimum value to require support
#endif
  struct int_min_value_t 
  {
      typedef typename detail::int_least_helper
        <
#ifdef BOOST_HAS_LONG_LONG
          (MinValue >= ::boost::integer_traits<boost::long_long_type>::const_min) +
#else
           1 +
#endif
          (MinValue >= ::boost::integer_traits<long>::const_min) +
          (MinValue >= ::boost::integer_traits<int>::const_min) +
          (MinValue >= ::boost::integer_traits<short>::const_min) +
          (MinValue >= ::boost::integer_traits<signed char>::const_min)
        >::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };

  //  unsigned
#if !defined(BOOST_NO_INTEGRAL_INT64_T) && defined(BOOST_HAS_LONG_LONG)
  template< boost::ulong_long_type MaxValue >   // minimum value to require support
#else
  template< long MaxValue >   // minimum value to require support
#endif
  struct uint_value_t 
  {
      typedef typename detail::int_least_helper
        < 
          5 +
#ifdef BOOST_HAS_LONG_LONG
          (MaxValue <= ::boost::integer_traits<boost::ulong_long_type>::const_max) +
#else
           1 +
#endif
          (MaxValue <= ::boost::integer_traits<unsigned long>::const_max) +
          (MaxValue <= ::boost::integer_traits<unsigned int>::const_max) +
          (MaxValue <= ::boost::integer_traits<unsigned short>::const_max) +
          (MaxValue <= ::boost::integer_traits<unsigned char>::const_max)
        >::least  least;
      typedef typename int_fast_t<least>::fast  fast;
  };


} // namespace boost

#endif  // BOOST_INTEGER_HPP
