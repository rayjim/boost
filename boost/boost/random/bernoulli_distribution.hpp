/* boost random/bernoulli_distribution.hpp header file
 *
 * Copyright Jens Maurer 2000-2001
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
 * Revision history
 *  2001-02-18  moved to individual header files
 */

#ifndef BOOST_RANDOM_BERNOULLI_DISTRIBUTION_HPP
#define BOOST_RANDOM_BERNOULLI_DISTRIBUTION_HPP

#include <cassert>

namespace boost {

// Bernoulli distribution: p(true) = p, p(false) = 1-p   (boolean)
template<class UniformRandomNumberGenerator, class RealType = double>
class bernoulli_distribution
{
public:
  typedef UniformRandomNumberGenerator base_type;
  typedef bool result_type;
  bernoulli_distribution(base_type & rng, const RealType& p = RealType(0.5)) 
    : _rng(&rng),
      _p(p),
      _threshold(static_cast<base_result>
                 (p * RealType(_rng->max() - _rng->min())) + _rng->min())
  {
    // for p == 0, we can only set _threshold = 0, which is not enough
    assert(p >= 0);
    assert(p <= 1);
  }

  // compiler-generated copy ctor and assignment operator are fine

  base_type& base() const { return *_rng; }
  RealType p() const { return _p; }
  void reset() { }

  result_type operator()()
  { return _p > RealType(0) && (*_rng)() <= _threshold; }

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  friend bool operator==(const bernoulli_distribution& x, 
                         const bernoulli_distribution& y)
  { return x._threshold == y._threshold && *x._rng == *y._rng; }
#else
  // Use a member function
  bool operator==(const bernoulli_distribution& rhs) const
  { return _threshold == rhs._threshold && *_rng == *rhs._rng;  }
#endif
private:
  typedef typename base_type::result_type base_result;
  base_type * _rng;
  RealType _p;
  base_result _threshold;
};

} // namespace boost

#endif // BOOST_RANDOM_BERNOULLI_DISTRIBUTION_HPP
