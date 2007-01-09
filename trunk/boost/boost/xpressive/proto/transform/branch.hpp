///////////////////////////////////////////////////////////////////////////////
/// \file branch.hpp
/// A special-purpose proto transform for transforming one branch of the expression
/// tree separately from the rest. Given an expression and a new state, it
/// transforms the expression using the new state.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_BRANCH_HPP_EAN_12_16_2006
#define BOOST_PROTO_TRANSFORM_BRANCH_HPP_EAN_12_16_2006

#if !defined(__WAVE__) || !defined(BOOST_PROTO_DOXYGEN_INVOKED)
#include <boost/xpressive/proto/proto_fwd.hpp>
#else
/// INTERNAL ONLY
/// Needed to work around doxygen bug
struct a_dummy_global;
#endif

namespace boost { namespace proto { namespace transform
{

    // A branch compiler, for compiling a sub-tree with a specified state
    template<typename Grammar, typename BranchState>
    struct branch
      : Grammar
    {
        branch();

        template<typename Expr, typename State, typename Visitor>
        struct apply
          : Grammar::template apply<Expr, BranchState, Visitor>
        {};

        template<typename Expr, typename State, typename Visitor>
        static typename apply<Expr, State, Visitor>::type
        call(Expr const &expr, State const &, Visitor &visitor)
        {
            return Grammar::call(expr, BranchState(), visitor);
        }
    };

}}}

#endif
