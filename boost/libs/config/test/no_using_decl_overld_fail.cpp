
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
// This file should not compile, if it does then
// BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE need not be defined.
// see boost_no_using_decl_overld.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_using_decl_overld.cxx on
// Fri May 23 11:39:30 GMTST 2003

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
#include "boost_no_using_decl_overld.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
   return boost_no_using_declaration_overloads_from_typename_base::test();
}  
   
