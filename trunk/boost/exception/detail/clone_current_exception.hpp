//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_81522C0EB56511DFAB613DB0DFD72085
#define UUID_81522C0EB56511DFAB613DB0DFD72085

#ifdef BOOST_NO_EXCEPTIONS
#error This header requires exception handling to be enabled.
#endif

namespace
boost
    {
    namespace
    exception_detail
        {
        namespace
        clone_current_exception_result
            {
            int const success=0;
            int const bad_alloc=1;
            int const not_supported=2;
            }

        class clone_base;
        int clone_current_exception_msvc( clone_base const * & cloned );

        inline
        int
        clone_current_exception( clone_base const * & cloned )
            {
#ifdef BOOST_ENABLE_NON_INTRUSIVE_EXCEPTION_PTR
#ifdef _MSC_VER
            return clone_current_exception_msvc(cloned);
#endif
#endif
            return clone_current_exception_result::not_supported;
            }
        }
    }

#endif
