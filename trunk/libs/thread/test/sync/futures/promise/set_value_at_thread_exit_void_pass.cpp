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

// void promise<void>::set_value_at_thread_exit();

#define BOOST_THREAD_VERSION 4

#include <boost/thread/future.hpp>
#include <boost/detail/lightweight_test.hpp>

int i = 0;

boost::promise<void> p;
void func()
{
  p.set_value_at_thread_exit();
  i = 1;
}

void func2(boost::promise<void> p2)
{
  p2.set_value_at_thread_exit();
  i = 1;
}

int main()
{
  try
  {
    boost::future<void> f = p.get_future();
    boost::thread(func).detach();
    f.get();
    BOOST_TEST(i == 1);

  }
  catch(std::exception ex)
  {
    BOOST_TEST(false);
  }
  catch(...)
  {
    BOOST_TEST(false);
  }

  // BUG when moving promise. fixme
//  try
//  {
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//    boost::promise<void> p2; // BUG
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//    boost::future<void> f = p2.get_future();
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//    boost::thread(func2, boost::move(p2)).detach(); // BUG
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//    f.get();
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//    BOOST_TEST(i == 1);
//    std::cout << __FILE__ << ":" << __LINE__ <<std::endl;
//
//  }
//  catch(std::exception ex)
//  {
//    std::cout << __FILE__ << ":" << __LINE__ << " " << ex.what() << std::endl;
//    BOOST_TEST(false);
//  }
//  catch(...)
//  {
//    BOOST_TEST(false);
//  }
  return boost::report_errors();
}

