/* boost random/xor_combine.hpp header file
 *
 * Copyright Jens Maurer 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 */

#ifndef BOOST_RANDOM_XOR_COMBINE_HPP
#define BOOST_RANDOM_XOR_COMBINE_HPP

#include <iostream>
#include <cassert>
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>     // uint32_t


namespace boost {
namespace random {

template<class URNG1, int s1, class URNG2, int s2,
#ifndef BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
  typename URNG1::result_type 
#else
  uint32_t
#endif
  val = 0>
class xor_combine
{
public:
  typedef URNG1 base1_type;
  typedef URNG2 base2_type;
  typedef typename base1_type::result_type result_type;

  BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);
  BOOST_STATIC_CONSTANT(int, shift1 = s1);
  BOOST_STATIC_CONSTANT(int, shfit2 = s2);

  xor_combine() : _rng1(), _rng2()
  { }
  xor_combine(const base1_type & rng1, const base2_type & rng2)
    : _rng1(rng1), _rng2(rng2) { }
  template<class It> xor_combine(It& first, It last)
    : _rng1(first, last), _rng2( /* advanced by other call */ first, last) { }
  void seed() { _rng1.seed(); _rng2.seed(); }
  template<class It> void seed(It& first, It last)
  {
    _rng1.seed(first, last);
    _rng2.seed(first, last);
  }

  const base1_type& base1() { return _rng1; }
  const base2_type& base2() { return _rng2; }

  result_type operator()()
  {
    // MSVC fails BOOST_STATIC_ASSERT with std::numeric_limits at class scope
#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
    BOOST_STATIC_ASSERT(std::numeric_limits<typename base1_type::result_type>::is_integer);
    BOOST_STATIC_ASSERT(std::numeric_limits<typename base2_type::result_type>::is_integer);
    BOOST_STATIC_ASSERT(std::numeric_limits<typename base1_type::result_type>::digits >= std::numeric_limits<typename base2_type::result_type>::digits);
#endif
    return (_rng1() << s1) ^ (_rng2() << s2);
  }

  result_type min() const { return std::min(_rng1.min(), _rng2.min()); }
  result_type max() const { return std::max(_rng1.min(), _rng2.max()); }
  static bool validation(result_type x) { return val == x; }

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os, const xor_combine& s)
  {
    os << s._rng1 << " " << s._rng2 << " ";
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& is, xor_combine& s)
  {
    is >> s._rng1 >> std::ws >> s._rng2 >> std::ws;
    return is;
  }

  friend bool operator==(const xor_combine& x, const xor_combine& y)
  { return x._rng1 == y._rng1 && x._rng2 == y._rng2; }
  friend bool operator!=(const xor_combine& x, const xor_combine& y)
  { return !(x == y); }
#else
  // Use a member function; Streamable concept not supported.
  bool operator==(const xor_combine& rhs) const
  { return _rng1 == rhs._rng1 && _rng2 == rhs._rng2; }
  bool operator!=(const xor_combine& rhs) const
  { return !(*this == rhs); }
#endif

private:
  base1_type _rng1;
  base2_type _rng2;
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class URNG1, int s1, class URNG2, int s2,
#ifndef BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
  typename URNG1::result_type 
#else
  uint32_t
#endif
  val>
const bool xor_combine<URNG1, s1, URNG2, s2, val>::has_fixed_range;
#endif

} // namespace random
} // namespace boost

#endif // BOOST_RANDOM_XOR_COMBINE_HPP
