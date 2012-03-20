//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Copyright (C) 2011 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <boost/thread/future.hpp>
// class promise<R>
// promise& operator=(const promise& rhs) = delete;

#define BOOST_THREAD_VERSION 2
#include <boost/thread/future.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{

  //BOOST_TEST(test_alloc_base::count == 0);
  {
    //boost::promise<int> p0(boost::allocator_arg, test_allocator<int>());
    //boost::promise<int> p(boost::allocator_arg, test_allocator<int>());
    boost::promise<int> p0;
    boost::promise<int> p;
    //BOOST_TEST(test_alloc_base::count == 2);
    p = p0;
    //BOOST_TEST(test_alloc_base::count == 1);
    boost::future<int> f = p.get_future();
    //BOOST_TEST(test_alloc_base::count == 1);
    BOOST_TEST(f.valid());
    try
    {
      f = p0.get_future();
      BOOST_TEST(false);
    }
    catch (const boost::future_error& e)
    {
      BOOST_TEST(e.code() == boost::system::make_error_code(boost::future_errc::no_state));
    }
    //BOOST_TEST(test_alloc_base::count == 1);
  }
  //BOOST_TEST(test_alloc_base::count == 0);
//  {
//    //boost::promise<int&> p0(boost::allocator_arg, test_allocator<int>());
//    //boost::promise<int&> p(boost::allocator_arg, test_allocator<int>());
//    boost::promise<int&> p0;
//    boost::promise<int&> p;
//    //BOOST_TEST(test_alloc_base::count == 2);
//    p = p0;
//    //BOOST_TEST(test_alloc_base::count == 1);
//    boost::future<int&> f = p.get_future();
//    //BOOST_TEST(test_alloc_base::count == 1);
//    BOOST_TEST(f.valid());
//    try
//    {
//      f = p0.get_future();
//      BOOST_TEST(false);
//    }
//    catch (const boost::future_error& e)
//    {
//      BOOST_TEST(e.code() == boost::system::make_error_code(boost::future_errc::no_state));
//    }
//    //BOOST_TEST(test_alloc_base::count == 1);
//  }
//  //BOOST_TEST(test_alloc_base::count == 0);
//  {
//    //boost::promise<void> p0(boost::allocator_arg, test_allocator<void>());
//    //boost::promise<void> p(boost::allocator_arg, test_allocator<void>());
//    boost::promise<void> p0;
//    boost::promise<void> p;
//    //BOOST_TEST(test_alloc_base::count == 2);
//    p = p0;
//    //BOOST_TEST(test_alloc_base::count == 1);
//    boost::future<void> f = p.get_future();
//    //BOOST_TEST(test_alloc_base::count == 1);
//    BOOST_TEST(f.valid());
//    try
//    {
//      f = p0.get_future();
//      BOOST_TEST(false);
//    }
//    catch (const boost::future_error& e)
//    {
//      BOOST_TEST(e.code() == boost::system::make_error_code(boost::future_errc::no_state));
//    }
//    //BOOST_TEST(test_alloc_base::count == 1);
//  }
  //BOOST_TEST(test_alloc_base::count == 0);


  return boost::report_errors();
}

