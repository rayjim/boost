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

// <boost/thread/locks.hpp>

// template <class Mutex> class unique_lock;

// unique_lock(unique_lock const&) = delete;

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
  boost::unique_lock<boost::mutex> ul;
  BOOST_TEST(!ul.owns_lock());
  BOOST_TEST(ul.mutex() == 0);

  return boost::report_errors();
}

