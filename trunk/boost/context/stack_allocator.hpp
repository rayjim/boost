
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CTX_STACK_ALLOCATOR_H
#define BOOST_CTX_STACK_ALLOCATOR_H

#include <cstddef>

#include <boost/config.hpp>

#include <boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace ctx {

class BOOST_CONTEXT_DECL stack_allocator
{
public:
    static bool is_stack_unbound();

    static std::size_t default_stacksize();

    static std::size_t minimum_stacksize();

    static std::size_t maximum_stacksize();

    void * allocate( std::size_t) const;

    void deallocate( void *, std::size_t) const;
};

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CTX_STACK_ALLOCATOR_H
