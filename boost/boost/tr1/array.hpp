//  (C) Copyright John Maddock 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TR1_ARRAY_HPP_INCLUDED
#  define BOOST_TR1_ARRAY_HPP_INCLUDED
#  include <boost/tr1/detail/config.hpp>

#ifdef BOOST_HAS_TR1_ARRAY

#  include <BOOST_TR1_HEADER(array)>

#else

#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/detail/workaround.hpp>

namespace std{ namespace tr1{

using ::boost::array;

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570))
// [6.1.3.2] Tuple creation functions
using ::boost::swap;
#endif

// [6.2.2.5] Tuple interface to class template array
template <class T> struct tuple_size; // forward declaration
template <int I, class T> struct tuple_element; // forward declaration
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, size_t N>
struct tuple_size< ::boost::array<T, N> >
   : public ::boost::integral_constant< ::std::size_t, N>{};


template <int I, class T, size_t N>
struct tuple_element<I, ::boost::array<T, N> >
{
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x570))
   BOOST_STATIC_ASSERT(I < (int)N);
   BOOST_STATIC_ASSERT(I >= 0);
#endif
   typedef T type;
};
#endif
template <int I, class T, size_t N>
T& get( ::boost::array<T, N>& a)
{
   BOOST_STATIC_ASSERT(I < N);
   BOOST_STATIC_ASSERT(I >= 0);
   return a[I];
}

template <int I, class T, size_t N>
const T& get(const array<T, N>& a)
{
   BOOST_STATIC_ASSERT(I < N);
   BOOST_STATIC_ASSERT(I >= 0);
   return a[I];
}

} } // namespaces

#endif

#endif
