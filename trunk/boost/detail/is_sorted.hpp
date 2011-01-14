/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_DETAIL_SORTED_HPP
#define BOOST_DETAIL_SORTED_HPP

#include <boost/detail/iterator.hpp>

#include <functional>

namespace boost {
namespace detail {

template<class Iterator, class Comp>
inline Iterator is_sorted_until (Iterator first, Iterator last, Comp compare) {
  if (first == last)
    return last;

  Iterator it = first; ++it;

  for (; it != last; first = it, ++it)
    if (compare(*it, *first))
      return it;

  return it;
}

template<class Iterator>
inline Iterator is_sorted_until (Iterator first, Iterator last) {
  typedef typename boost::detail::iterator_traits<Iterator>::value_type
    value_type;

  typedef std::less<value_type> compare; 

  return is_sorted_until(first, last, compare()); 
}

template<class Iterator, class Comp>
inline bool is_sorted (Iterator first, Iterator last, Comp compare) {
  return is_sorted_until(first, last, compare) == last;
} 

template<class Iterator>
inline bool is_sorted (Iterator first, Iterator last) {
  return is_sorted_until(first, last) == last;
} 

} // detail
} // boost

#endif // BOOST_DETAIL_SORTED_HPP

