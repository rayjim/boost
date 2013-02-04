//  (C) Copyright Gennadiy Rozental 2011-2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74248 $
//
//  Description : bitwose comparison manipulator implementation
// ***************************************************************************

#ifndef BOOST_TEST_TOOLS_DETAIL_BITWISE_MANIP_HPP_012705GER
#define BOOST_TEST_TOOLS_DETAIL_BITWISE_MANIP_HPP_012705GER

// Boost Test
#include <boost/test/tools/detail/fwd.hpp>
#include <boost/test/tools/detail/indirections.hpp>

#include <boost/test/tools/assertion_result.hpp>
#include <boost/test/tools/assertion.hpp>

// STL
#include <climits>          // for CHAR_BIT

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace test_tools {

// ************************************************************************** //
// **************        bitwise comparison manipulator        ************** //
// ************************************************************************** //

struct bitwise {};

//____________________________________________________________________________//

inline int
operator<<( unit_test::lazy_ostream const&, bitwise )   { return 0; }

//____________________________________________________________________________//

namespace tt_detail {

template<typename Lhs, typename Rhs, typename E>
assertion_result
bitwise_compare(Lhs const& lhs, Rhs const& rhs, E const& expr )
{
    assertion_result    pr( true );

    std::size_t left_bit_size  = sizeof(Lhs)*CHAR_BIT;
    std::size_t right_bit_size = sizeof(Rhs)*CHAR_BIT;

    static Lhs const leftOne( 1 );
    static Rhs const rightOne( 1 );

    std::size_t total_bits = left_bit_size < right_bit_size ? left_bit_size : right_bit_size;

    for( std::size_t counter = 0; counter < total_bits; ++counter ) {
        if( (lhs & ( leftOne << counter )) != (rhs & (rightOne << counter)) ) {
            if( pr ) {
                pr.message() << " [";
                expr.report( pr.message().stream() );
                pr.message() << "]. Bitwise comparison failed";
                pr = false;
            }
            pr.message() << "\nMismatch in a position " << counter;
        }
    }

    if( left_bit_size != right_bit_size ) {
        if( pr ) {
            pr.message() << " [";
            expr.report( pr.message().stream() );
            pr.message() << "]. Bitwise comparison failed";
            pr = false;
        }
        pr.message() << "\nOperands bit sizes mismatch: " << left_bit_size << " != " << right_bit_size;
    }

    return pr;
}

//____________________________________________________________________________//

template<typename T1, typename T2, typename T3, typename T4>
assertion_result
operator<<(assertion_evaluate_t<assertion::binary_expr<T1,T2,assertion::op::EQ<T3,T4> > > const& ae, bitwise )
{
    return bitwise_compare( ae.m_e.lhs().value(), ae.m_e.rhs(), ae.m_e );
}

//____________________________________________________________________________//

inline check_type
operator<<( assertion_type const& at, bitwise )
{
    return CHECK_BUILT_ASSERTION;
}

//____________________________________________________________________________//

} // namespace tt_detail
} // namespace test_tools
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TOOLS_DETAIL_BITWISE_MANIP_HPP_012705GER
