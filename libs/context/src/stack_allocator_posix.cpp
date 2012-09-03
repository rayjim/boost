
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_CONTEXT_SOURCE

#include <boost/context/stack_allocator.hpp>

extern "C" {
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
}

#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>

#if !defined (SIGSTKSZ)
# define SIGSTKSZ (8 * 1024)
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace {

static rlimit stacksize_limit_()
{
    rlimit limit;
    const int result = ::getrlimit( RLIMIT_STACK, & limit);
    BOOST_ASSERT( 0 == result);
    return limit;
}

static rlimit stacksize_limit()
{
    static rlimit limit = stacksize_limit_();
    return limit;
}

static std::size_t pagesize()
{
    static std::size_t size = ::getpagesize();
    return size;
}

static std::size_t page_count( std::size_t stacksize)
{
    return static_cast< std::size_t >( 
        std::ceil(
            static_cast< float >( stacksize) / pagesize() ) );
}

}

namespace boost {
namespace ctx {

bool
stack_allocator::is_stack_unbound()
{ return RLIM_INFINITY == stacksize_limit().rlim_max; }

std::size_t
stack_allocator::maximum_stacksize()
{
    BOOST_ASSERT( ! is_stack_unbound() );
    return static_cast< std::size_t >( stacksize_limit().rlim_max);
}

std::size_t
stack_allocator::minimum_stacksize()
{ return SIGSTKSZ; }

std::size_t
stack_allocator::default_stacksize()
{
    std::size_t size = 64 * 1024; // 64 kB
    if ( is_stack_unbound() )
        return std::max( size, minimum_stacksize() );
    
    BOOST_ASSERT( maximum_stacksize() >= minimum_stacksize() );
    return maximum_stacksize() == minimum_stacksize()
        ? minimum_stacksize()
        : std::min( size, maximum_stacksize() );
}

void *
stack_allocator::allocate( std::size_t size) const
{
    if ( minimum_stacksize() > size)
        throw std::invalid_argument(
            boost::str( boost::format("invalid stack size: must be at least %d bytes")
                % minimum_stacksize() ) );

    if ( ! is_stack_unbound() && ( maximum_stacksize() < size) )
        throw std::invalid_argument(
            boost::str( boost::format("invalid stack size: must not be larger than %d bytes")
                % maximum_stacksize() ) );

    const std::size_t pages( page_count( size) + 1); // add one guard page
    const std::size_t size_( pages * pagesize() );
    BOOST_ASSERT( 0 < size && 0 < size_);

    const int fd( ::open("/dev/zero", O_RDONLY) );
    BOOST_ASSERT( -1 != fd);
    void * limit =
# if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
        ::mmap( 0, size_, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
# else
        ::mmap( 0, size_, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
# endif
    ::close( fd);
    if ( ! limit) throw std::bad_alloc();

    std::memset( limit, size_, '\0');

    const int result( ::mprotect( limit, pagesize(), PROT_NONE) );
    BOOST_ASSERT( 0 == result);

    return static_cast< char * >( limit) + size_;
}

void
stack_allocator::deallocate( void * vp, std::size_t size) const
{
    if ( vp)
    {
        const std::size_t pages = page_count( size) + 1;
        const std::size_t size_ = pages * pagesize();
        BOOST_ASSERT( 0 < size && 0 < size_);
        void * limit = static_cast< char * >( vp) - size_;
        ::munmap( limit, size_);
    }
}

}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
