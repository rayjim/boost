
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/assert.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_pointer.hpp>


BOOST_MPL_ASSERT(( boost::is_same<int,int> ));
BOOST_MPL_ASSERT(( boost::is_pointer<int*> ));
BOOST_MPL_ASSERT_NOT(( boost::is_same<int,long> ));
BOOST_MPL_ASSERT_NOT(( boost::is_pointer<int> ));
BOOST_MPL_ASSERT_RELATION( 5, >, 1 );
BOOST_MPL_ASSERT_RELATION( 1, <, 5 );
BOOST_MPL_ASSERT_MSG( true, GLOBAL_SCOPE_ERROR, (int,long) );
BOOST_MPL_ASSERT_MSG( true, ANOTHER_GLOBAL_SCOPE_ERROR, () );

namespace my {
BOOST_MPL_ASSERT(( boost::is_same<int,int> ));
BOOST_MPL_ASSERT(( boost::is_pointer<int*> ));
BOOST_MPL_ASSERT_NOT(( boost::is_same<int,long> ));
BOOST_MPL_ASSERT_NOT(( boost::is_pointer<int> ));
BOOST_MPL_ASSERT_RELATION( 5, >, 1 );
BOOST_MPL_ASSERT_RELATION( 1, <, 5 );
BOOST_MPL_ASSERT_MSG( true, GLOBAL_SCOPE_ERROR, (int,long) );
BOOST_MPL_ASSERT_MSG( true, NAMESPACE_SCOPE_ERROR, () );
BOOST_MPL_ASSERT_MSG( true, ANOTHER_NAMESPACE_SCOPE_ERROR, () );
}

template< typename T >
struct her
{
    BOOST_MPL_ASSERT(( boost::is_same<void,T> ));
    BOOST_MPL_ASSERT(( boost::is_pointer<T*> ));
    BOOST_MPL_ASSERT_NOT(( boost::is_same<int,T> ));
    BOOST_MPL_ASSERT_NOT(( boost::is_pointer<T> ));
    BOOST_MPL_ASSERT_RELATION( sizeof(T*), >, 1 );
    BOOST_MPL_ASSERT_RELATION( 1, <, sizeof(T*) );
    BOOST_MPL_ASSERT_MSG( true, GLOBAL_SCOPE_ERROR, (int,long) );
    BOOST_MPL_ASSERT_MSG( true, CLASS_SCOPE_ERROR, () );
#if !defined(BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
    BOOST_MPL_ASSERT_MSG( true, ANOTHER_CLASS_SCOPE_ERROR, (types<int, T>) );
#endif

    void f()
    {
        BOOST_MPL_ASSERT(( boost::is_same<void,T> ));
        BOOST_MPL_ASSERT(( boost::is_pointer<T*> ));
        BOOST_MPL_ASSERT_NOT(( boost::is_same<int,T> ));
        BOOST_MPL_ASSERT_NOT(( boost::is_pointer<T> ));
        BOOST_MPL_ASSERT_RELATION( sizeof(T*), >, 1 );
        BOOST_MPL_ASSERT_RELATION( 1, <, sizeof(T*) );
        BOOST_MPL_ASSERT_MSG( true, GLOBAL_SCOPE_ERROR, (int,long) );
        BOOST_MPL_ASSERT_MSG( true, MEMBER_FUNCTION_SCOPE_ERROR, () );
#if !defined(BOOST_MPL_CFG_NO_DEFAULT_PARAMETERS_IN_NESTED_TEMPLATES)
        BOOST_MPL_ASSERT_MSG( true, ANOTHER_MEMBER_FUNCTION_SCOPE_ERROR, (types<int, T>) );
#endif
    }
};

int main()
{
    her<void> h;
    h.f();
    
    BOOST_MPL_ASSERT(( boost::is_same<int,int> ));
    BOOST_MPL_ASSERT(( boost::is_pointer<int*> ));
    BOOST_MPL_ASSERT_NOT(( boost::is_same<int,long> ));
    BOOST_MPL_ASSERT_NOT(( boost::is_pointer<int> ));
    BOOST_MPL_ASSERT_RELATION( 5, >, 1 );
    BOOST_MPL_ASSERT_RELATION( 1, <, 5 );
    BOOST_MPL_ASSERT_MSG( true, GLOBAL_SCOPE_ERROR, (int,long) );
    BOOST_MPL_ASSERT_MSG( true, FUNCTION_SCOPE_ERROR, () );
    BOOST_MPL_ASSERT_MSG( true, ANOTHER_FUNCTION_SCOPE_ERROR, () );

    return 0;
}
