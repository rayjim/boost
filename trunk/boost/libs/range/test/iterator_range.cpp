// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#include <boost/range/iterator_range.hpp>
#include <boost/range/functions.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>
#include <string>

// This should be included before "using namespace boost",
// otherwise gcc headers will be confused with boost::iterator
// namespace.
#include <boost/test/included/unit_test_framework.hpp> 

using namespace boost;
using namespace std;

struct add_one
{
    template< class T >
    T operator()( T r ) const
    {
        return r + 1;
    }
};

void check_iterator_range()
{
	
    typedef string::iterator               iterator;
    typedef string::const_iterator         const_iterator;
    typedef iterator_range<iterator>       irange;
    typedef iterator_range<const_iterator> cirange;
    string       str  = "hello world";
    const string cstr = "const world";
    irange r    = make_iterator_range( str );
    r           = make_iterator_range( str.begin(), str.end() );
    cirange r2  = make_iterator_range( cstr );
    r2          = make_iterator_range( cstr.begin(), cstr.end() );
    r2          = make_iterator_range( str );
 
    BOOST_CHECK( !r.empty() );
    BOOST_CHECK( !r2.empty() );
    
    if( !r )
        BOOST_CHECK( false );
    if( !r2 )
        BOOST_CHECK( false );

    BOOST_CHECK_EQUAL( r.size(), size( r ) );
    BOOST_CHECK_EQUAL( r2.size(), size( r2 ) );
    
    BOOST_CHECK_EQUAL( distance( r.begin(), r.end() ), 
                       distance( begin( r2 ), end( r2 ) ) );
    cout << r << r2;
    
    string res  = copy_range<string>( r );
    BOOST_CHECK( equal( res.begin(), res.end(), r.begin() ) );

}


using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Range Test Suite" );

    test->add( BOOST_TEST_CASE( &check_iterator_range ) );

    return test;
}





