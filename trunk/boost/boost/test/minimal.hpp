//  (C) Copyright Gennadiy Rozental 2002-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : simple minimal testing definitions and implementation
// ***************************************************************************

#ifndef BOOST_TEST_MINIMAL_HPP_071894GER
#define BOOST_TEST_MINIMAL_HPP_071894GER

#define BOOST_CHECK(exp)       \
  ( (exp)                      \
      ? static_cast<void>(0)   \
      : boost::minimal_test::the_monitor->report_error(#exp,__FILE__,__LINE__, BOOST_CURRENT_FUNCTION) )

#define BOOST_REQUIRE(exp)     \
  ( (exp)                      \
      ? static_cast<void>(0)   \
      : boost::minimal_test::the_monitor->report_critical_error(#exp,__FILE__,__LINE__,BOOST_CURRENT_FUNCTION))

#define BOOST_ERROR( msg_ )    \
        boost::minimal_test::the_monitor->report_error( (msg_),__FILE__,__LINE__, BOOST_CURRENT_FUNCTION, true )
#define BOOST_FAIL( msg_ )     \
        boost::minimal_test::the_monitor->report_critical_error( (msg_),__FILE__,__LINE__, BOOST_CURRENT_FUNCTION, true )

//____________________________________________________________________________//

// Boost.Test
#include <boost/test/impl/execution_monitor.ipp>
#include <boost/test/utils/class_properties.hpp>

// Boost
#include <boost/cstdlib.hpp>            // for exit codes
#include <boost/current_function.hpp>   // for BOOST_CURRENT_FUNCTION

// STL
#include <iostream>                     // std::cerr, std::endl
#include <string>                       // std::string

//____________________________________________________________________________//

int test_main( int argc, char* argv[] );  // prototype for user's test_main()

namespace boost {
namespace minimal_test {

typedef boost::unit_test::const_string const_string;

class monitor : public boost::execution_monitor {
public:
    // constructor
    monitor( int argc, char** argv )
    : p_errors_counter( 0 ), m_argc( argc ), m_argv( argv ) {}

    // execution monitor hook implementation
    virtual int function()
    {
        return test_main( m_argc, m_argv );
    }

    void        report_error( const char* msg_, const char* file_, int line_, const_string func_name_, bool is_msg_ = false )
    {
        ++p_errors_counter.value;
        std::cerr << file_ << "(" << line_ << "): ";
        
        if( is_msg_ )
            std::cerr << msg_;
        else
            std::cerr << "test " << msg_ << " failed";

        if( func_name_ != "(unknown)" )
            std::cerr << " in function: '" << func_name_ << "'";
        
        std::cerr << std::endl;
    }

    void        report_critical_error( const char* msg_, const char* file_, int line_, const_string func_name_, bool is_msg_ = false )
    {
        report_error( msg_, file_, line_, func_name_, is_msg_ );
        throw boost::execution_exception( boost::execution_exception::no_error, "" );
    }

    // public properties
    BOOST_READONLY_PROPERTY( int, ( monitor ) ) p_errors_counter;

private:
    // Data members
    int         m_argc;
    char**      m_argv;
}; // monitor

monitor* the_monitor;

} // namespace minimal_test
} // namespace boost

//____________________________________________________________________________//

int BOOST_TEST_CALL_DECL main( int argc, char* argv[] )
{
    using boost::minimal_test::the_monitor;

    the_monitor = new boost::minimal_test::monitor( argc, argv );

    try {
        int run_result = boost::minimal_test::the_monitor->execute();

        BOOST_CHECK( run_result == 0 || run_result == boost::exit_success );
    }
    catch( boost::execution_exception const& exex ) {
        if( exex.code() != boost::execution_exception::no_error )
            BOOST_ERROR( (std::string( "exception \"" ).
                            append( exex.what().begin(), exex.what().end() ).
                            append( "\" caught" ) ).c_str() );
        std::cerr << "\n**** Testing aborted.";
    }

    if( boost::minimal_test::the_monitor->p_errors_counter != 0 ) {
        std::cerr << "\n**** " << the_monitor->p_errors_counter.get()
                  << " error" << (the_monitor->p_errors_counter > 1 ? "s" : "" ) << " detected\n";

        delete the_monitor;
        return boost::exit_test_failure;
    }

    std::cout << "\n**** no errors detected\n";
    
    delete the_monitor;
    return boost::exit_success;
}

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.18  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.17  2005/01/31 07:50:05  rogeeff
//  cdecl portability fix
//
//  Revision 1.16  2005/01/31 06:01:27  rogeeff
//  BOOST_TEST_CALL_DECL correctness fixes
//
//  Revision 1.15  2005/01/22 19:22:12  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
// ***************************************************************************


#endif // BOOST_TEST_MINIMAL_HPP_071894GER
