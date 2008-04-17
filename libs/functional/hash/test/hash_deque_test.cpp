
// Copyright 2005-2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./config.hpp"

#ifdef TEST_EXTENSIONS
#  ifdef TEST_STD_INCLUDES
#    include <functional>
#  else
#    include <boost/functional/hash.hpp>
#  endif
#endif

#include <boost/test/minimal.hpp>

#ifdef TEST_EXTENSIONS

#include <deque>

using std::deque;
#define CONTAINER_TYPE deque
#include "./hash_sequence_test.hpp"

#endif // TEST_EXTENSIONS

int test_main(int, char**)
{
#ifdef TEST_EXTENSIONS
    deque_tests::deque_hash_integer_tests();
#endif

    return 0;
}
