///////////////////////////////////////////////////////////////////////////////
// protect.hpp
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/proto/core.hpp>
#include <boost/proto/transform/make.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/test/unit_test.hpp>
namespace proto=boost::proto;
using proto::_;

template<typename T>
struct identity
{
    typedef T type;
};

struct Test
  : proto::make< proto::protect<_> >
{};

struct Test1
  : proto::make< identity<proto::protect<_> > >
{};

struct Test2
  : proto::make< identity<proto::protect<int> > >
{};

struct Test3
  : proto::make< identity<proto::protect<identity<_> > > >
{};

struct Test4
  : proto::make< identity<proto::protect<identity<int> > > >
{};

struct Test5
  : proto::make< identity<proto::protect<identity<identity<int> > > > >
{};

void test_protect()
{
    proto::terminal<int>::type i = {42};

    _ t = Test()(i);
    _ t1 = Test1()(i);
    int t2 = Test2()(i);
    identity<_> t3 = Test3()(i);
    identity<int> t4 = Test4()(i);
    identity<identity<int> > t5 = Test5()(i);
}

using namespace boost::unit_test;
///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test proto::protect");

    test->add(BOOST_TEST_CASE(&test_protect));

    return test;
}
