//  (C) Copyright Gennadiy Rozental 2001-2003.
//  (C) Copyright Beman Dawes 2000.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : test failures reported by BOOST_CHECK. Should fail during run.
// ***************************************************************************

// Boost.Test
#include <boost/test/test_tools.hpp>

int test_main( int, char*[] )  // note the name
{
    int v = 1;

    BOOST_CHECK( v == 2 );
    BOOST_CHECK( 2 == v );

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.7  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.6  2003/02/13 08:47:10  rogeeff
//  *** empty log message ***
//
//  Revision 1.5  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
