///////////////////////////////////////////////////////////////////////////////
/// \file proto1.hpp
/// The proto1 expression template compiler and supporting utilities.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_V1_HPP_EAN_04_01_2005
#define BOOST_PROTO_V1_HPP_EAN_04_01_2005

#include <boost/xpressive/proto/v1_/proto_fwd.hpp>
#include <boost/xpressive/proto/v1_/op_tags.hpp>
#include <boost/xpressive/proto/v1_/op_base.hpp>
#include <boost/xpressive/proto/v1_/operators.hpp>
#include <boost/xpressive/proto/v1_/arg_traits.hpp>

namespace boost { namespace proto1
{
    ///////////////////////////////////////////////////////////////////////////////
    // compile_result
    template<typename Op, typename State, typename Visitor, typename DomainTag>
    struct compile_result
    {
        typedef typename as_op<Op>::type op_type;
        typedef typename tag_type<op_type>::type tag_type;
        typedef compiler<tag_type, DomainTag> compiler_type;
        typedef typename compiler_type::BOOST_NESTED_TEMPLATE apply<op_type, State, Visitor>::type type;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // compile
    template<typename Op, typename State, typename Visitor, typename DomainTag>
    typename compile_result<Op, State, Visitor, DomainTag>::type const
    compile(Op const &op, State const &state, Visitor &visitor, DomainTag)
    {
        typedef typename as_op<Op>::type op_type;
        typedef compiler<typename tag_type<op_type>::type, DomainTag> compiler;
        return compiler::call(as_op<Op>::make(op), state, visitor);
    }

}} // namespace boost::proto1

#endif
