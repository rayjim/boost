/* boost random/detail/uniform_int_float.hpp header file
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
 */

#ifndef BOOST_RANDOM_DETAIL_PASS_THROUGH_ENGINE_HPP
#define BOOST_RANDOM_DETAIL_PASS_THROUGH_ENGINE_HPP

#include <boost/config.hpp>
#include <boost/random/detail/ptr_helper.hpp>


namespace boost {
namespace random {
namespace detail {

template<class UniformRandomNumberGenerator>
class pass_through_engine
{
private:
  typedef ptr_helper<UniformRandomNumberGenerator> helper_type;

public:
  typedef typename helper_type::value_type base_type;
  typedef typename base_type::result_type result_type;

  explicit pass_through_engine(UniformRandomNumberGenerator rng)
    // make argument an rvalue to avoid matching Generator& constructor
    : _rng(static_cast<typename helper_type::rvalue_type>(rng))
  { }

  result_type min() const { return base().min(); }
  result_type max() const { return base().max(); }
  base_type& base() { return helper_type::ref(_rng); }
  const base_type& base() const { return helper_type::ref(_rng); }

  result_type operator()() { return base()(); }

private:
  UniformRandomNumberGenerator _rng;
};

template<class UniformRandomNumberGenerator, class CharT, class Traits>
std::basic_ostream<CharT,Traits>&
operator<<(
    std::basic_ostream<CharT,Traits>& os
    , const pass_through_engine<UniformRandomNumberGenerator>& ud
    )
{
    return os << ud.base();
}

template<class UniformRandomNumberGenerator, class CharT, class Traits>
std::basic_istream<CharT,Traits>&
operator>>(
    std::basic_istream<CharT,Traits>& is
    , const pass_through_engine<UniformRandomNumberGenerator>& ud
    )
{
    return is >> ud.base();
}

} // namespace detail
} // namespace random
} // namespace boost

#endif // BOOST_RANDOM_DETAIL_PASS_THROUGH_ENGINE_HPP
