//-----------------------------------------------------------------------------
// boost mpl/test/find.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mpl/find.hpp"
#include "boost/mpl/list.hpp"
#include "boost/mpl/list_c.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/int.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{    
    typedef mpl::list<int,char,long,short,char,long,double,long>::type types;
    typedef mpl::list_c<int,1,0,5,1,7,5,0,5> values;

    typedef mpl::find<types, short>::type types_iter;
    typedef mpl::find< values, mpl::integral_c<int,7> >::type values_iter;
   
    BOOST_MPL_ASSERT_IS_SAME(types_iter::type, short);
    BOOST_STATIC_ASSERT(values_iter::type::value == 7);

    typedef mpl::begin<types>::type types_first;
    typedef mpl::begin<values>::type values_first;
    BOOST_STATIC_ASSERT((mpl::distance< types_first,types_iter >::type::value == 3));
    BOOST_STATIC_ASSERT((mpl::distance< values_first,values_iter >::type::value == 4));

    return 0;
}
