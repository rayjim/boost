//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

// Boost.Test
#include <boost/test/test_tools.hpp>

int add( int i, int j ) { return i+j; }

int test_main( int, char* [] )  // note the name!
{
    // six ways to detect and report the same error:
    BOOST_CHECK( add(2,2) == 4 );          // #1 continues on error

    BOOST_REQUIRE( add(2,2) == 4 );        // #2 throws on error

    if ( add(2,2) != 4 )
        BOOST_ERROR( "Ouch...");           // #3 continues on error

    if ( add(2,2) != 4 )
        BOOST_FAIL( "Ouch..." );           // #4 throws on error

    if ( add(2,2) != 4 )
        throw "Oops...";                   // #5 throws on error

    return add(2,2) == 4 ? 0 : 1;          // #6 returns error code
}

// EOF
