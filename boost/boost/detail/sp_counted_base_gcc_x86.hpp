#ifndef BOOST_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
#define BOOST_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  detail/sp_counted_base_gcc_x86.hpp - g++ on 486+ or AMD64
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
//  Lock-free algorithm by Alexander Terekhov
//
//  Thanks to Ben Hitchings for the #weak + (#shared != 0)
//  formulation
//

#include <typeinfo>

namespace boost
{

namespace detail
{

inline long atomic_exchange_and_add( long * pw, long dv )
{
    // long r = *pw;
    // *pw += dv;
    // return r;

    long r;

    __asm__ __volatile__
    (
        "lock\n\t"
        "xadd %1, %0":
        "=m"( *pw ), "=r"( r ): // outputs (%0, %1)
        "0"( *pw ), "1"( dv ): // inputs (%0, %1)
        "memory", "cc" // clobbers
    );

    return r;
}

inline long atomic_conditional_increment( long * pw )
{
    // long r = *pw;
    // if( r != 0 ) ++*pw;
    // return r;

    long r;

    __asm__ __volatile__
    (
        "movl %0, %%eax\n\t"
        "0:\n\t"
        "test %%eax, %%eax\n\t"
        "je 1f\n\t"
        "leal 1(%%eax), %%ebx\n\t"
        "lock\n\t"
        "cmpxchgl %%ebx, %0\n\t"
        "jne 0b\n\t"
        "1:":
        "=m"( *pw ), "=eax"( r ): // outputs (%0, %1)
        "0"( *pw ): // input (%0)
        "ebx", "memory", "cc" // clobbers
    );

    return r;
}

class sp_counted_base
{
private:

    sp_counted_base( sp_counted_base const & );
    sp_counted_base & operator= ( sp_counted_base const & );

    long use_count_;        // #shared
    long weak_count_;       // #weak + (#shared != 0)

public:

    sp_counted_base(): use_count_( 1 ), weak_count_( 1 )
    {
    }

    virtual ~sp_counted_base() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destroy() is called when weak_count_ drops to zero.

    virtual void destroy() // nothrow
    {
        delete this;
    }

    virtual void * get_deleter( std::type_info const & ti ) = 0;

    void add_ref_copy()
    {
        atomic_exchange_and_add( &use_count_, +1 );
    }

    bool add_ref_lock() // true on success
    {
		return atomic_conditional_increment( &use_count_ ) != 0;
    }

    void release() // nothrow
    {
        if( atomic_exchange_and_add( &use_count_, -1 ) == 1 )
        {
            dispose();
            weak_release();
        }
    }

    void weak_add_ref() // nothrow
    {
        atomic_exchange_and_add( &weak_count_, +1 );
    }

    void weak_release() // nothrow
    {
        if( atomic_exchange_and_add( &weak_count_, -1 ) == 1 )
        {
            destroy();
        }
    }

    long use_count() const // nothrow
    {
        return static_cast<long const volatile &>( use_count_ );
    }
};

} // namespace detail

} // namespace boost

#endif  // #ifndef BOOST_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
