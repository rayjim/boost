//  (C) Copyright Gennadiy Rozental 2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : class basic_cstring comparisons implementation
// ***************************************************************************

#ifndef  BASIC_CSTRING_COMPARE_HPP_071894GER
#define  BASIC_CSTRING_COMPARE_HPP_071894GER

// Boost.Test
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>

// STL
#include <functional>
#include <cctype>

# if defined(BOOST_NO_STDC_NAMESPACE) && !BOOST_WORKAROUND(__BORLANDC__, <= 0x570) && !BOOST_WORKAROUND(__GNUC__, < 3)
namespace std { using ::toupper; }
# endif

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************                case_ins_compare              ************** //
// ************************************************************************** //

namespace ut_detail {

template<class CharT>
struct case_ins
{
    static bool         eq( CharT c1, CharT c2 ) { return (std::toupper)( c1 ) == (std::toupper)( c2 ); }
    static bool         lt( CharT c1, CharT c2 ) { return (std::toupper)( c1 ) <  (std::toupper)( c2 ); }

    static int          compare( CharT const* s1, CharT const* s2, std::size_t n )
    {
        for( std::size_t i = 0; i < n; ++i ) {
            if( !eq( s1[i], s2[i] ) )
                return lt( s1[i], s2[i] ) ? -1 : 1;
        }
        return 0;
    }
};

} // namespace ut_detail

// ************************************************************************** //
// **************                  case_ins_eq                 ************** //
// ************************************************************************** //

template<class CharT>
inline bool
case_ins_eq( basic_cstring<CharT> x, basic_cstring<CharT> y )
{
    return x.size() == y.size() && ut_detail::case_ins<CharT>::compare( x.begin(), y.begin(), x.size() ) == 0;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                 case_ins_less                ************** //
// ************************************************************************** //

template<class CharT>
class case_ins_less : public std::binary_function<basic_cstring<CharT>,basic_cstring<CharT>,bool>
{
public:
    bool operator()( basic_cstring<CharT> x, basic_cstring<CharT> y ) const
    {
        return x.size() != y.size() 
                ? x.size() < y.size() 
                : ut_detail::case_ins<CharT>::compare( x.begin(), y.begin(), x.size() ) < 0;
    }
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************                  operator <                  ************** //
// ************************************************************************** //

template<class CharT>
inline bool
operator <( boost::unit_test::basic_cstring<CharT> const& x,
            boost::unit_test::basic_cstring<CharT> const& y )
{
    typedef typename boost::unit_test::basic_cstring<CharT>::traits_type traits_type;
    return x.size() != y.size() 
            ? x.size() < y.size() 
            : traits_type::compare( x.begin(), y.begin(), x.size() ) < 0;
}

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2005/01/31 20:07:21  rogeeff
//  Sunpro CC 5.3 workarounds
//
//  Revision 1.2  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.1  2005/01/22 18:21:40  rogeeff
//  moved sharable staff into utils
//
// ***************************************************************************

#endif // BASIC_CSTRING_COMPARE_HPP_071894GER
