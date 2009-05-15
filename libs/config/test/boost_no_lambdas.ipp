//  (C) Copyright Beman Dawes 2009

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for more information.

//  MACRO:         BOOST_NO_LAMBDAS
//  TITLE:         C++0x lambdas feature unavailable
//  DESCRIPTION:   The compiler does not support the C++0x lambdas feature

#include <cassert>

namespace boost_no_lambdas {

int test()
{
  assert( 12345 == [](){return 12345;} );
  return 0;
}

}
