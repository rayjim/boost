///////////////////////////////////////////////////////////////////////////////
// literal_matcher.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_LITERAL_MATCHER_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_LITERAL_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/core/quant_style.hpp>
#include <boost/xpressive/detail/core/state.hpp>
#include <boost/xpressive/detail/utility/traits_utils.hpp>

namespace boost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // literal_matcher
    //
    template<typename TraitsT, bool ICaseT, bool NotT>
    struct literal_matcher
      : quant_style_fixed_width<1>
    {
        typedef typename TraitsT::char_type char_type;
        typedef mpl::bool_<NotT> not_type;
        typedef mpl::bool_<ICaseT> icase_type;
        char_type ch_;

        literal_matcher(char_type ch, TraitsT const &traits)
          : ch_(detail::translate(ch, traits, icase_type()))
        {
        }

        template<typename BidiIterT, typename NextT>
        bool match(state_type<BidiIterT> &state, NextT const &next) const
        {
            if(state.eos() || NotT ==
                (detail::translate(*state.cur_, traits_cast<TraitsT>(state), icase_type()) == this->ch_))
            {
                return false;
            }

            ++state.cur_;
            if(next.match(state))
            {
                return true;
            }

            --state.cur_;
            return false;
        }
    };

}}}

#endif
