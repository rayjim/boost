//  (C) Copyright Gennadiy Rozental 2001-2004.
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

#ifndef  BASIC_CSTRING_COMPARE_HPP
#define  BASIC_CSTRING_COMPARE_HPP

// Boost.Test
#include <boost/test/detail/basic_cstring/basic_cstring.hpp>

// STL
#include <functional>
#include <cctype>

# ifdef BOOST_NO_STDC_NAMESPACE
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
    static bool         eq( CharT const& c1, CharT const& c2 ) { return std::toupper( c1 ) == std::toupper( c2 ); }
    static bool         lt( CharT const& c1, CharT const& c2 ) { return std::toupper( c1 ) <  std::toupper( c2 ); }

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
case_ins_eq( basic_cstring<CharT> const& x, basic_cstring<CharT> const& y )
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
    bool operator()( basic_cstring<CharT> const& x, basic_cstring<CharT> const& y ) const
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
operator <( basic_cstring<CharT> const& x, basic_cstring<CharT> const& y )
{
    typedef typename basic_cstring<CharT>::traits_type traits_type;
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
//  Revision 1.2  2004/05/21 06:19:35  rogeeff
//  licence update
//
//  Revision 1.1  2004/05/11 11:00:55  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
// ***************************************************************************

#endif // BASIC_CSTRING_COMPARE_HPP
