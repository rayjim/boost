// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#include <boost/test/unit_test.hpp>
#include "read_reversing_filter_test.hpp"
#include "write_reversing_filter_test.hpp"

using boost::unit_test_framework::test_suite;      

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("reverse test");
    test->add(BOOST_TEST_CASE(&read_reversing_filter_test));
    test->add(BOOST_TEST_CASE(&write_reversing_filter_test));
    return test;
}
