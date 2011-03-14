/* boost random/detail/integer_log2.hpp header file
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 */

#ifndef BOOST_RANDOM_DETAIL_INTEGER_LOG2_HPP
#define BOOST_RANDOM_DETAIL_INTEGER_LOG2_HPP

#include <boost/limits.hpp>
#include <boost/pending/integer_log2.hpp>

namespace boost {
namespace random {
namespace detail {

template<int Shift>
struct integer_log2_impl
{
    template<class T>
    static int apply(T t, int accum)
    {
        int update = ((t >> Shift) != 0) * Shift;
        return integer_log2_impl<Shift / 2>::apply(t >> update, accum + update);
    }
};

template<>
struct integer_log2_impl<1>
{
    template<class T>
    static int apply(T t, int accum)
    {
        return int(t >> 1) + accum;
    }
};

template<class T>
inline int integer_log2(T t)
{
    return integer_log2_impl<
        ::boost::detail::max_pow2_less<
            ::std::numeric_limits<T>::digits, 4
        >::value
    >::apply(t, 0);
}

} // namespace detail
} // namespace random
} // namespace boost

#endif // BOOST_RANDOM_DETAIL_INTEGER_LOG2_HPP
