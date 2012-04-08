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

// class packaged_task<R>

// void swap(packaged_task& other);


#include <boost/thread/future.hpp>
#include <boost/detail/lightweight_test.hpp>

class A
{
    long data_;

public:
    explicit A(long i) : data_(i) {}

    long operator()() const {return data_ ;}
    long operator()(long i, long j) const {return data_ + i + j;}
};

int main()
{
  boost::unique_lock<mutex> lk1(m);
  boost::unique_lock<mutex> lk2;
  lk1.swap(lk2);
  BOOST_TEST(lk1.mutex() == 0);
  BOOST_TEST(lk1.owns_lock() == false);
  BOOST_TEST(lk2.mutex() == &m);
  BOOST_TEST(lk2.owns_lock() == true);

  return boost::report_errors();
}

