//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/special_functions/fpclassify.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/special_functions/fpclassify.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void check()
{
   check_result<int>(boost::math::fpclassify BOOST_NO_MACRO_EXPAND<float>(f));
   check_result<int>(boost::math::fpclassify BOOST_NO_MACRO_EXPAND<double>(d));
   check_result<int>(boost::math::fpclassify BOOST_NO_MACRO_EXPAND<long double>(l));

   check_result<bool>(boost::math::isfinite BOOST_NO_MACRO_EXPAND<float>(f));
   check_result<bool>(boost::math::isfinite BOOST_NO_MACRO_EXPAND<double>(d));
   check_result<bool>(boost::math::isfinite BOOST_NO_MACRO_EXPAND<long double>(l));

   check_result<bool>(boost::math::isinf BOOST_NO_MACRO_EXPAND<float>(f));
   check_result<bool>(boost::math::isinf BOOST_NO_MACRO_EXPAND<double>(d));
   check_result<bool>(boost::math::isinf BOOST_NO_MACRO_EXPAND<long double>(l));

   check_result<bool>(boost::math::isnormal BOOST_NO_MACRO_EXPAND<float>(f));
   check_result<bool>(boost::math::isnormal BOOST_NO_MACRO_EXPAND<double>(d));
   check_result<bool>(boost::math::isnormal BOOST_NO_MACRO_EXPAND<long double>(l));
}


