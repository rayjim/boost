//  Copyright John Maddock 2012.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <mpfr.h>

#ifdef __GNUC__
#pragma message "MPFR_VERSION_STRING=" MPFR_VERSION_STRING
#endif

#if (__GNU_MP_VERSION < 4) || ((__GNU_MP_VERSION == 4) && (__GNU_MP_VERSION_MINOR < 2))
#error "Incompatible GMP version"
#endif

#if (MPFR_VERSION < 3)
#error "Incompatible MPFR version"
#endif

#ifdef __GNUC__
#pragma message "__GNU_MP_VERSION=" BOOST_STRINGIZE(__GNU_MP_VERSION)
#pragma message "__GNU_MP_VERSION_MINOR=" BOOST_STRINGIZE(__GNU_MP_VERSION_MINOR)
#endif 

#if (__GNU_MP_VERSION < 4) || ((__GNU_MP_VERSION == 4) && (__GNU_MP_VERSION_MINOR < 2))
#error "Incompatible GMP version"
#endif

int main()
{
   mpfr_buildopt_tls_p();
   return 0;
}

