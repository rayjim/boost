/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <string>
#include <vector>

#include <boost/detail/lightweight_test.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_directive.hpp>
#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/support_argument.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <string>
#include <iostream>
#include "test.hpp"

using namespace spirit_test;

int
main()
{
    using namespace boost::spirit::ascii;

    {
        BOOST_TEST(test("a,b,c,d,e,f,g,h", char_ % ','));
        BOOST_TEST(test("a,b,c,d,e,f,g,h,", char_ % ',', false));
    }

    {
        BOOST_TEST(test("a, b, c, d, e, f, g, h", char_ % ',', space));
        BOOST_TEST(test("a, b, c, d, e, f, g, h,", char_ % ',', space, false));
    }

    {
        std::string s;
        BOOST_TEST(test_attr("a,b,c,d,e,f,g,h", char_ % ',', s));
        BOOST_TEST(s == "abcdefgh");
    }

    {
        using boost::spirit::int_;

        std::vector<int> v;
        BOOST_TEST(test_attr("1,2", int_ % ',', v));
        BOOST_TEST(2 == v.size() && 1 == v[0] && 2 == v[1]);
    }

    {
        using boost::spirit::int_;

        std::vector<int> v;
        BOOST_TEST(test_attr("(1,2)", '(' >> int_ % ',' >> ')', v));
        BOOST_TEST(2 == v.size() && 1 == v[0] && 2 == v[1]);
    }

    {
        std::vector<std::string> v;
        BOOST_TEST(test_attr("a,b,c,d", +alpha % ',', v));
        BOOST_TEST(4 == v.size() && "a" == v[0] && "b" == v[1]
            && "c" == v[2] && "d" == v[3]);
    }

    {
        std::vector<boost::optional<std::string> > v;
        BOOST_TEST(test_attr("#a,#", ('#' >> -alpha) % ',', v)); 
        BOOST_TEST(2 == v.size() && 
            v[0] && "a" == boost::get<std::string>(v[0]) && 
            v[1] && boost::get<std::string>(v[1]).size() == 1 && 
                    boost::get<std::string>(v[1])[0] == '\0');
    }

    { // actions
        namespace phx = boost::phoenix;
        using boost::phoenix::begin;
        using boost::phoenix::end;
        using boost::phoenix::construct;
        using boost::spirit::qi::_1;

        std::string s;
        BOOST_TEST(test("a,b,c,d,e,f,g,h", (char_ % ',')
            [phx::ref(s) = construct<std::string>(begin(_1), end(_1))]));
    }

    return boost::report_errors();
}

