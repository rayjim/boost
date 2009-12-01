//Copyright (c) 2006-2009 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_F0EE17BE6C1211DE87FF459155D89593
#define UUID_F0EE17BE6C1211DE87FF459155D89593

#if defined(__GNUC__) && !defined(BOOST_EXCEPTION_ENABLE_WARNINGS)
#pragma GCC system_header
#endif

#include "boost/exception/info.hpp"
#include <errno.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(push) 
#pragma warning(disable:4996) //'strerror': This function or variable may be unsafe
#endif

namespace
boost
    {
    typedef error_info<struct errinfo_errno_,int> errinfo_errno;

    //Usage hint:
    //if( c_function(....)!=0 )
    //    BOOST_THROW_EXCEPTION(
    //        failure() <<
    //        errinfo_errno(errno) <<
    //        errinfo_api_function("c_function") );
    inline
    std::string
    to_string( errinfo_errno const & e )
        {
        std::ostringstream tmp;
        int v=e.value();
        tmp << v << ", \"" << strerror(v) << "\"";
        return tmp.str();
        }
    }

#ifdef _MSC_VER
#pragma warning(pop) 
#endif

#endif
