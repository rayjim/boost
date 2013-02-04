/*=============================================================================
    Copyright (c) 2012 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#define BOOST_FUSION_DONT_USE_PREPROCESSED_FILES // $$$ JDG temp $$$

#include <boost/config.hpp>

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#include <boost/fusion/container/map/map.hpp>

struct k1 {};
struct k2 {};

#define FUSION_SEQUENCE boost::fusion::map<boost::fusion::pair<k1, std::vector<x>>>

#define FUSION_SEQUENCE2 boost::fusion::map<        \
    boost::fusion::pair<k1, std::vector<x>>,        \
    boost::fusion::pair<k2, x>>

#include "move.hpp"

#else
#include <boost/detail/lightweight_test.hpp>
#endif

int
main()
{
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
    test();
#endif

    return boost::report_errors();
}

