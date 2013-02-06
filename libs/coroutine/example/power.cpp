
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::coroutine< int() >       coro1_t;
typedef boost::coroutines::coroutine< void( int) >  coro2_t;
typedef boost::range_iterator< coro1_t >::type      iterator_t;

void power( coro2_t & c, int number, int exponent)
{
    int counter = 0;
    int result = 1;
    while ( counter++ < exponent)
    {
            result = result * number;
            c( result);
    }
}

int main()
{
    {
        std::cout << "using range functions" << std::endl;
        coro1_t c( boost::bind( power, _1, 2, 8) );
        iterator_t e( boost::end( c) );
        for ( iterator_t i( boost::begin( c) ); i != e; ++i)
            std::cout << * i <<  " ";
    }

    {
        std::cout << "\nusing BOOST_FOREACH" << std::endl;
        coro1_t c( boost::bind( power, _1, 2, 8) );
        BOOST_FOREACH( int i, c)
        { std::cout << i <<  " "; }
    }

    std::cout << "\nDone" << std::endl;

    return EXIT_SUCCESS;
}
#if 0
int main()
{
    {
        std::cout << "using range functions" << std::endl;
        boost::coroutines::coroutine< int() > c(
            [&]( boost::coroutines::coroutine< void( int) > &c) {
                int counter = 0;
                int result = 1;
                while ( counter++ < 8)
                {
                    result = result * 2;
                    c( result);
                }
            });
        iterator_t e( boost::end( c) );
        for ( iterator_t i( boost::begin( c) ); i != e; ++i)
            std::cout << * i <<  " ";
    }

    {
        std::cout << "\nusing BOOST_FOREACH" << std::endl;
        boost::coroutines::coroutine< int() > c(
            [&]( boost::coroutines::coroutine< void( int) > &c) {
                int counter = 0;
                int result = 1;
                while ( counter++ < 8)
                {
                    result = result * 2;
                    c( result);
                }
            });
        for ( int i : c) {
            std::cout << i <<  " ";
        }
    }

    std::cout << "\nDone" << std::endl;

    return EXIT_SUCCESS;
}
#endif
