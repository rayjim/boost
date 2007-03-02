/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/algorithm/query/any.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/vector_c.hpp>

int
main()
{
    {
        boost::fusion::vector<int, short, double> t(1, 2, 3.3);
        BOOST_TEST(boost::fusion::any(t, boost::lambda::_1 == 2));
    }

    {
        boost::fusion::vector<int, short, double> t(1, 2, 3.3);
        BOOST_TEST(!boost::fusion::any(t, boost::lambda::_1 == 3));
    }

    {
        typedef boost::mpl::vector_c<int, 1, 2, 3> mpl_vec;
        BOOST_TEST(boost::fusion::any(mpl_vec(), boost::lambda::_1 == 2));
        BOOST_TEST(!boost::fusion::any(mpl_vec(), boost::lambda::_1 == 4));
    }

    return boost::report_errors();
}

