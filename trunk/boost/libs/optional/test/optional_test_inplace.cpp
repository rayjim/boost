// (C) 2003, Fernando Luis Cacciola Carballal.
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#include<iostream>
#include<stdexcept>
#include<string>

#define BOOST_ENABLE_ASSERT_HANDLER

#include "boost/optional.hpp"
#include "boost/utility/in_place_factory.hpp"
#include "boost/utility/typed_in_place_factory.hpp"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "boost/test/minimal.hpp"

#include "optional_test_common.cpp"

struct A
{
  A ( double a0, std::string a1 ) : m_a0(a0), m_a1(a1) {}

  friend bool operator == ( A const& x, A const& y )
    { return x.m_a0 == y.m_a0 && x.m_a1 == y.m_a1 ; }

  double      m_a0 ;
  std::string m_a1 ;
} ;

int test_main( int, char* [] )
{
  double a0 = 3.14 ;
  std::string a1("pi");

  A a(a0,a1);

  boost::optional<A> opt1(a);

  boost::optional<A> opt2 ( boost::in_place(a0,a1) ) ;

  boost::optional<A> opt3 ( boost::in_place<A>(a0,a1) ) ;

  BOOST_CHECK( opt1 == opt2 ) ;
  BOOST_CHECK( opt2 == opt2 ) ;
  BOOST_CHECK( *opt2 == a ) ;

  return 0;
}




