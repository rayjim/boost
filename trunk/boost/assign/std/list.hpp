// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//


#ifndef BOOST_ASSIGN_STD_LIST_HPP
#define BOOST_ASSIGN_STD_LIST_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/assign/list_inserter.hpp>
#include <boost/config.hpp>
#include <list>

namespace boost
{
namespace assign
{

    template< class V, class A, class V2 >
    inline list_inserter< assign_detail::call_push_back< std::list<V,A> >, V >
    operator+=( std::list<V,A>& c, V2 v )
    {
        return push_back( c )( v );
    }
    
}
}

#endif
