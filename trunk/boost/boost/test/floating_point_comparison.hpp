//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines algoirthms for comparing 2 floating point values
// ***************************************************************************

#ifndef BOOST_TEST_FLOATING_POINT_COMPARISON_HPP_071894GER
#define BOOST_TEST_FLOATING_POINT_COMPARISON_HPP_071894GER

#include <boost/limits.hpp>  // for std::numeric_limits

#include <boost/test/utils/class_properties.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace test_tools {

using unit_test::readonly_property;

// ************************************************************************** //
// **************        floating_point_comparison_type        ************** //
// ************************************************************************** //

enum floating_point_comparison_type { FPC_STRONG, FPC_WEAK };

// ************************************************************************** //
// **************                    details                   ************** //
// ************************************************************************** //

namespace tt_detail {

template<typename FPT>
inline FPT
fpt_abs( FPT arg ) 
{
    return arg < 0 ? -arg : arg;
}

//____________________________________________________________________________//

// both f1 and f2 are unsigned here
template<typename FPT>
inline FPT 
safe_fpt_division( FPT f1, FPT f2 )
{
    return  (f2 < 1 && f1 > f2 * (std::numeric_limits<FPT>::max)())               ? (std::numeric_limits<FPT>::max)()
            : ((f2 > 1 && f1 < f2 * (std::numeric_limits<FPT>::min)() || f1 == 0) ? 0
                                                                                  : f1/f2 );
}

//____________________________________________________________________________//

} // namespace tt_detail

// ************************************************************************** //
// **************             close_at_tolerance               ************** //
// ************************************************************************** //

template<typename FPT, typename PersentType = FPT >
class close_at_tolerance {
public:
    explicit    close_at_tolerance( PersentType percentage_tolerance, floating_point_comparison_type fpc_type = FPC_STRONG ) 
    : p_fraction_tolerance( static_cast<FPT>(0.01)*percentage_tolerance ), p_strong_or_weak( fpc_type ==  FPC_STRONG ) {}

    bool        operator()( FPT left, FPT right ) const
    {
        FPT diff = tt_detail::fpt_abs( left - right );
        FPT d1   = tt_detail::safe_fpt_division( diff, tt_detail::fpt_abs( right ) );
        FPT d2   = tt_detail::safe_fpt_division( diff, tt_detail::fpt_abs( left ) );
        
        return p_strong_or_weak ? (d1 <= p_fraction_tolerance.get() && d2 <= p_fraction_tolerance.get()) 
                                : (d1 <= p_fraction_tolerance.get() || d2 <= p_fraction_tolerance.get());
    }

    // Public properties
    readonly_property<FPT>  p_fraction_tolerance;
    readonly_property<bool> p_strong_or_weak;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************               check_is_close                 ************** //
// ************************************************************************** //

template<typename FPT, typename PersentType>
inline bool
check_is_close( FPT left, FPT right, PersentType percentage_tolerance, floating_point_comparison_type fpc_type = FPC_STRONG )
{
    close_at_tolerance<FPT,PersentType> pred( percentage_tolerance, fpc_type );

    return pred( left, right );
}

//____________________________________________________________________________//

template<typename FPT>
inline FPT
compute_tolerance( FPT percentage_tolerance )
{
    close_at_tolerance<FPT> pred( percentage_tolerance );

    return pred.p_fraction_tolerance.get();
}

//____________________________________________________________________________//

} // namespace test_tools

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.21  2005/02/20 08:27:05  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.20  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.19  2005/01/22 19:22:12  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
// ***************************************************************************

#endif // BOOST_FLOATING_POINT_COMAPARISON_HPP_071894GER
