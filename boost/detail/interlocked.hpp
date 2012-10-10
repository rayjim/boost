#ifndef BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED
#define BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/interlocked.hpp
//
//  Copyright 2005 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>

#if defined( BOOST_USE_WINDOWS_H )

# include <windows.h>

# define BOOST_INTERLOCKED_INCREMENT InterlockedIncrement
# define BOOST_INTERLOCKED_DECREMENT InterlockedDecrement
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange
# define BOOST_INTERLOCKED_EXCHANGE InterlockedExchange
# define BOOST_INTERLOCKED_EXCHANGE_ADD InterlockedExchangeAdd
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER InterlockedCompareExchangePointer
# define BOOST_INTERLOCKED_EXCHANGE_POINTER InterlockedExchangePointer

#elif defined(_WIN32_WCE)

#if _WIN32_WCE >= 0x600

extern "C" long __cdecl _InterlockedIncrement( long volatile * );
extern "C" long __cdecl _InterlockedDecrement( long volatile * );
extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __cdecl _InterlockedExchange( long volatile *, long );
extern "C" long __cdecl _InterlockedExchangeAdd( long volatile *, long );

# define BOOST_INTERLOCKED_INCREMENT _InterlockedIncrement
# define BOOST_INTERLOCKED_DECREMENT _InterlockedDecrement
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange
# define BOOST_INTERLOCKED_EXCHANGE _InterlockedExchange
# define BOOST_INTERLOCKED_EXCHANGE_ADD _InterlockedExchangeAdd

#else
// under Windows CE we still have old-style Interlocked* functions

extern "C" long __cdecl InterlockedIncrement( long* );
extern "C" long __cdecl InterlockedDecrement( long* );
extern "C" long __cdecl InterlockedCompareExchange( long*, long, long );
extern "C" long __cdecl InterlockedExchange( long*, long );
extern "C" long __cdecl InterlockedExchangeAdd( long*, long );

# define BOOST_INTERLOCKED_INCREMENT InterlockedIncrement
# define BOOST_INTERLOCKED_DECREMENT InterlockedDecrement
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange
# define BOOST_INTERLOCKED_EXCHANGE InterlockedExchange
# define BOOST_INTERLOCKED_EXCHANGE_ADD InterlockedExchangeAdd

#endif

# define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)(dest),(long)(exchange),(long)(compare)))
# define BOOST_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)BOOST_INTERLOCKED_EXCHANGE((long*)(dest),(long)(exchange)))

#elif defined( BOOST_MSVC ) || defined( BOOST_INTEL_WIN )

#if defined( BOOST_MSVC ) && BOOST_MSVC >= 1500

#include <intrin.h>

#elif defined( __CLRCALL_PURE_OR_CDECL )

extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedIncrement( long volatile * );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedDecrement( long volatile * );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchange( long volatile *, long );
extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchangeAdd( long volatile *, long );

#else

extern "C" long __cdecl _InterlockedIncrement( long volatile * );
extern "C" long __cdecl _InterlockedDecrement( long volatile * );
extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
extern "C" long __cdecl _InterlockedExchange( long volatile *, long );
extern "C" long __cdecl _InterlockedExchangeAdd( long volatile *, long );

#endif

# pragma intrinsic( _InterlockedIncrement )
# pragma intrinsic( _InterlockedDecrement )
# pragma intrinsic( _InterlockedCompareExchange )
# pragma intrinsic( _InterlockedExchange )
# pragma intrinsic( _InterlockedExchangeAdd )

# if defined(_M_IA64) || defined(_M_AMD64)

extern "C" void* __cdecl _InterlockedCompareExchangePointer( void* volatile *, void*, void* );
extern "C" void* __cdecl _InterlockedExchangePointer( void* volatile *, void* );

#  pragma intrinsic( _InterlockedCompareExchangePointer )
#  pragma intrinsic( _InterlockedExchangePointer )

#  define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER _InterlockedCompareExchangePointer
#  define BOOST_INTERLOCKED_EXCHANGE_POINTER _InterlockedExchangePointer

# else

#  define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)BOOST_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
#  define BOOST_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)BOOST_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))

# endif

# define BOOST_INTERLOCKED_INCREMENT _InterlockedIncrement
# define BOOST_INTERLOCKED_DECREMENT _InterlockedDecrement
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange
# define BOOST_INTERLOCKED_EXCHANGE _InterlockedExchange
# define BOOST_INTERLOCKED_EXCHANGE_ADD _InterlockedExchangeAdd

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( __CYGWIN__ )

#if defined(__MINGW64__)
#define BOOST_INTERLOCKED_IMPORT
#else
#define BOOST_INTERLOCKED_IMPORT __declspec(dllimport)
#endif


namespace boost
{

namespace detail
{

extern "C" BOOST_INTERLOCKED_IMPORT long __stdcall InterlockedIncrement( long volatile * );
extern "C" BOOST_INTERLOCKED_IMPORT long __stdcall InterlockedDecrement( long volatile * );
extern "C" BOOST_INTERLOCKED_IMPORT long __stdcall InterlockedCompareExchange( long volatile *, long, long );
extern "C" BOOST_INTERLOCKED_IMPORT long __stdcall InterlockedExchange( long volatile *, long );
extern "C" BOOST_INTERLOCKED_IMPORT long __stdcall InterlockedExchangeAdd( long volatile *, long );

# if defined(_M_IA64) || defined(_M_AMD64)
extern "C" BOOST_INTERLOCKED_IMPORT void* __stdcall InterlockedCompareExchangePointer( void* volatile *, void*, void* );
extern "C" BOOST_INTERLOCKED_IMPORT void* __stdcall InterlockedExchangePointer( void* volatile *, void* );
# endif

} // namespace detail

} // namespace boost

# define BOOST_INTERLOCKED_INCREMENT ::boost::detail::InterlockedIncrement
# define BOOST_INTERLOCKED_DECREMENT ::boost::detail::InterlockedDecrement
# define BOOST_INTERLOCKED_COMPARE_EXCHANGE ::boost::detail::InterlockedCompareExchange
# define BOOST_INTERLOCKED_EXCHANGE ::boost::detail::InterlockedExchange
# define BOOST_INTERLOCKED_EXCHANGE_ADD ::boost::detail::InterlockedExchangeAdd

# if defined(_M_IA64) || defined(_M_AMD64)
#  define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER ::boost::detail::InterlockedCompareExchangePointer
#  define BOOST_INTERLOCKED_EXCHANGE_POINTER ::boost::detail::InterlockedExchangePointer
# else
#  define BOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
    ((void*)BOOST_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
#  define BOOST_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
    ((void*)BOOST_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))
# endif

#else

# error "Interlocked intrinsics not available"

#endif

#endif // #ifndef BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED
