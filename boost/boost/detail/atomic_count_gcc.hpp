#ifndef BOOST_DETAIL_ATOMIC_COUNT_GCC_HPP_INCLUDED
#define BOOST_DETAIL_ATOMIC_COUNT_GCC_HPP_INCLUDED

//
//  boost/detail/atomic_count_gcc.hpp
//
//  atomic_count for GNU libstdc++ v3
//
//  http://gcc.gnu.org/onlinedocs/porting/Thread-safety.html
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2002 Lars Gullik Bj�nnes <larsbj@lyx.org>
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <bits/atomicity.h>

namespace boost
{

namespace detail
{

class atomic_count
{
public:

    explicit atomic_count(long v) : value_(v) {}

    void operator++()
    {
        __atomic_add(&value_, 1);
    }

    long operator--()
    {
        return __exchange_and_add(&value_, -1) - 1;
    }

    operator long() const
    {
        return __exchange_and_add(&value_, 0);
    }

private:

    atomic_count(atomic_count const &);
    atomic_count & operator=(atomic_count const &);

    mutable _Atomic_word value_;
};

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_ATOMIC_COUNT_GCC_HPP_INCLUDED
