// Copyright (C) 2007 Anthony Williams
// Copyright (C) 2013 Tim Blechmann
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/thread/thread_only.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread/mutex.hpp>

void test_physical_concurrency_is_non_zero()
{
    BOOST_CHECK(boost::thread::physical_concurrency()!=0);
}

boost::unit_test::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test::test_suite* test =
        BOOST_TEST_SUITE("Boost.Threads: physical concurrency test suite");

    test->add(BOOST_TEST_CASE(test_physical_concurrency_is_non_zero));
    return test;
}


