
//  (C) Copyright John Maddock 2009. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include "test.hpp"
#include "check_integral_constant.hpp"
#include <boost/type_traits/is_virtual_base_of.hpp>


TT_TEST_BEGIN(is_virtual_base_of)

BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Derived2,MultiBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,PrivateBase>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<NonDerived,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,void>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,const void>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<void,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<const void,Derived>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<int, int>::value), false);  // really it is!!!!!
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<const int, int>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<VB,VD>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<VD,VB>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<test_abc1,test_abc3>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<test_abc3,test_abc1>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit1>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit1,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit2>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit2,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<Base,virtual_inherit3>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit3,Base>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<boost::noncopyable,virtual_inherit4>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit4,boost::noncopyable>::value), false);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<int_convertible,virtual_inherit5>::value), true);
BOOST_CHECK_INTEGRAL_CONSTANT((::tt::is_virtual_base_of<virtual_inherit5,int_convertible>::value), false);

TT_TEST_END









