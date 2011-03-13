/* test_minstd_rand0.cpp
 *
 * Copyright Steven Watanabe 2011
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 *
 */

#include <boost/random/linear_congruential.hpp>
#include <boost/cstdint.hpp>

#define BOOST_RANDOM_URNG boost::random::minstd_rand0

#define BOOST_RANDOM_SEED_WORDS 1

// validation values from the publications
#define BOOST_RANDOM_VALIDATION_VALUE 1043618065
#define BOOST_RANDOM_SEED_SEQ_VALIDATION_VALUE 849515105
#define BOOST_RANDOM_ITERATOR_VALIDATION_VALUE 1263181168

#define BOOST_RANDOM_GENERATE_VALUES { 0xC00041A6U, 0xCD8358EBU, 0x430A4B7AU, 0x31B781ADU }

#include "test_generator.ipp"
