//-----------------------------------------------------------------------------
// boost mpl/v2_1/eval.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_V2_1_EVAL_HPP_INCLUDED
#define BOOST_MPL_V2_1_EVAL_HPP_INCLUDED

#include "boost/mpl/v2_1/expr.hpp"
#include "boost/mpl/v2_1/metafunction_base.hpp"

namespace boost { namespace mpl { namespace v2_1 {

template<
      typename F
    >
struct eval
    : expr<F>::type::template apply<>
{
};

}}} // namespace boost::mpl::v2_1

#define EVAL(expr) eval<expr>::type

#endif // BOOST_MPL_V2_1_EVAL_HPP_INCLUDED
