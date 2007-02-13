///////////////////////////////////////////////////////////////////////////////
/// \file operators.hpp
/// Contains all the overloaded operators that make it possible to build
/// expression templates using proto components
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_OPERATORS_HPP_EAN_04_01_2005
#define BOOST_PROTO_OPERATORS_HPP_EAN_04_01_2005

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/tags.hpp>
#include <boost/xpressive/proto/expr.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto
{
    template<typename Domain, typename Expr, typename EnableIf>
    struct generate
    {
        typedef Expr type;
        static Expr const &make(Expr const &expr)
        {
            return expr;
        }
    };

    namespace detail
    {
        template<typename Tag, typename Left, typename Right, typename Enable1 = void, typename Enable2 = void>
        struct as_expr_if2
        {};

        template<typename Tag, typename Left, typename Right>
        struct as_expr_if2<Tag, Left, Right, typename Left::is_boost_proto_expr_, void>
          : generate<
                typename Left::domain
              , expr<
                    Tag
                  , args2<
                        ref<Left>
                      , typename generate<typename Left::domain, expr<tag::terminal, args1<Right &> > >::type
                    >
                >
            >
        {
            typedef expr<tag::terminal, args1<Right &> > term_type;
            typedef expr<Tag, args2<ref<Left>, typename generate<typename Left::domain, term_type>::type> > expr_type;

            template<typename Left2>
            static typename generate<typename Left2::domain, expr_type>::type
            make(Left2 &left, Right &right)
            {
                term_type term = {right};
                expr_type that = {{left}, generate<typename Left::domain, term_type>::make(term)};
                return generate<typename Left::domain, expr_type>::make(that);
            }
        };

        template<typename Tag, typename Left, typename Right>
        struct as_expr_if2<Tag, Left, Right, void, typename Right::is_boost_proto_expr_>
          : generate<
                typename Right::domain
              , expr<
                    Tag
                  , args2<
                        typename generate<typename Right::domain, expr<tag::terminal, args1<Left &> > >::type
                      , ref<Right>
                    >
                >
            >
        {
            typedef expr<tag::terminal, args1<Left &> > term_type;
            typedef expr<Tag, args2<typename generate<typename Right::domain, term_type>::type, ref<Right> > > expr_type;

            template<typename Right2>
            static typename generate<typename Right2::domain, expr_type>::type
            make(Left &left, Right2 &right)
            {
                term_type term = {left};
                expr_type that = {generate<typename Right::domain, term_type>::make(term), {right}};
                return generate<typename Right::domain, expr_type>::make(that);
            }
        };

        template<typename Tag, typename Left, typename Right, typename Enable1 = void, typename Enable2 = void>
        struct as_expr_if
          : as_expr_if2<Tag, Left, Right>
        {};

        template<typename Tag, typename Left, typename Right>
        struct as_expr_if<Tag, Left, Right, typename Left::is_boost_proto_expr_, typename Right::is_boost_proto_expr_>
          : generate<
                typename Left::domain
              , expr<Tag, args2<ref<Left>, ref<Right> > >
            >
        {
            typedef expr<Tag, args2<ref<Left>, ref<Right> > > expr_type;
            BOOST_MPL_ASSERT((is_same<typename Left::domain, typename Right::domain>));

            template<typename Left2>
            static typename generate<typename Left2::domain, expr_type>::type
            make(Left2 &left, Right &right)
            {
                expr_type that = {{left}, {right}};
                return generate<typename Left::domain, expr_type>::make(that);
            }
        };
    } // detail

#define BOOST_PROTO_UNARY_OP(op, tag)\
    template<typename Arg>\
    inline typename generate<typename Arg::domain, expr<tag, args1<ref<typename Arg::boost_proto_expr_type_> > > >::type const\
    operator op(Arg &arg)\
    {\
        expr<tag, args1<ref<typename Arg::boost_proto_expr_type_> > > that = {{arg}};\
        return generate<typename Arg::domain, expr<tag, args1<ref<typename Arg::boost_proto_expr_type_> > > >::make(that);\
    }\
    template<typename Arg>\
    inline typename generate<typename Arg::domain, expr<tag, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::type const\
    operator op(Arg const &arg)\
    {\
        expr<tag, args1<ref<typename Arg::boost_proto_expr_type_ const> > > that = {{arg}};\
        return generate<typename Arg::domain, expr<tag, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::make(that);\
    }\
    /**/

#define BOOST_PROTO_BINARY_OP(op, tag)\
    template<typename Left, typename Right>\
    inline typename detail::as_expr_if<tag, Left, Right>::type const\
    operator op(Left &left, Right &right)\
    {\
        return detail::as_expr_if<tag, Left, Right>::make(left, right);\
    }\
    template<typename Left, typename Right>\
    inline typename detail::as_expr_if<tag, Left, Right const>::type const\
    operator op(Left &left, Right const &right)\
    {\
        return detail::as_expr_if<tag, Left, Right const>::make(left, right);\
    }\
    template<typename Left, typename Right>\
    inline typename detail::as_expr_if<tag, Left const, Right>::type const\
    operator op(Left const &left, Right &right)\
    {\
        return detail::as_expr_if<tag, Left const, Right>::make(left, right);\
    }\
    template<typename Left, typename Right>\
    inline typename detail::as_expr_if<tag, Left const, Right const>::type const\
    operator op(Left const &left, Right const &right)\
    {\
        return detail::as_expr_if<tag, Left const, Right const>::make(left, right);\
    }\
    /**/

    BOOST_PROTO_UNARY_OP(+, tag::unary_plus)
    BOOST_PROTO_UNARY_OP(-, tag::unary_minus)
    BOOST_PROTO_UNARY_OP(*, tag::unary_star)
    BOOST_PROTO_UNARY_OP(~, tag::complement)
    BOOST_PROTO_UNARY_OP(&, tag::address_of)
    BOOST_PROTO_UNARY_OP(!, tag::logical_not)
    BOOST_PROTO_UNARY_OP(++, tag::pre_inc)
    BOOST_PROTO_UNARY_OP(--, tag::pre_dec)

    BOOST_PROTO_BINARY_OP(<<, tag::left_shift)
    BOOST_PROTO_BINARY_OP(>>, tag::right_shift)
    BOOST_PROTO_BINARY_OP(*, tag::multiply)
    BOOST_PROTO_BINARY_OP(/, tag::divide)
    BOOST_PROTO_BINARY_OP(%, tag::modulus)
    BOOST_PROTO_BINARY_OP(+, tag::add)
    BOOST_PROTO_BINARY_OP(-, tag::subtract)
    BOOST_PROTO_BINARY_OP(<, tag::less)
    BOOST_PROTO_BINARY_OP(>, tag::greater)
    BOOST_PROTO_BINARY_OP(<=, tag::less_equal)
    BOOST_PROTO_BINARY_OP(>=, tag::greater_equal)
    BOOST_PROTO_BINARY_OP(==, tag::equal)
    BOOST_PROTO_BINARY_OP(!=, tag::not_equal)
    BOOST_PROTO_BINARY_OP(||, tag::logical_or)
    BOOST_PROTO_BINARY_OP(&&, tag::logical_and)
    BOOST_PROTO_BINARY_OP(&, tag::bitwise_and)
    BOOST_PROTO_BINARY_OP(|, tag::bitwise_or)
    BOOST_PROTO_BINARY_OP(^, tag::bitwise_xor)
    BOOST_PROTO_BINARY_OP(BOOST_PP_COMMA(), tag::comma)
    BOOST_PROTO_BINARY_OP(->*, tag::mem_ptr)

    BOOST_PROTO_BINARY_OP(<<=, tag::left_shift_assign)
    BOOST_PROTO_BINARY_OP(>>=, tag::right_shift_assign)
    BOOST_PROTO_BINARY_OP(*=, tag::multiply_assign)
    BOOST_PROTO_BINARY_OP(/=, tag::divide_assign)
    BOOST_PROTO_BINARY_OP(%=, tag::modulus_assign)
    BOOST_PROTO_BINARY_OP(+=, tag::add_assign)
    BOOST_PROTO_BINARY_OP(-=, tag::subtract_assign)
    BOOST_PROTO_BINARY_OP(&=, tag::bitwise_and_assign)
    BOOST_PROTO_BINARY_OP(|=, tag::bitwise_or_assign)
    BOOST_PROTO_BINARY_OP(^=, tag::bitwise_xor_assign)

    template<typename Arg>
    inline typename generate<typename Arg::domain, expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_> > > >::type const
    operator ++(Arg &arg, int)
    {
        expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_> > > that = {{arg}};
        return generate<typename Arg::domain, expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_> > > >::make(that);
    }

    template<typename Arg>
    inline typename generate<typename Arg::domain, expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::type const
    operator ++(Arg const &arg, int)
    {
        expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_ const> > > that = {{arg}};
        return generate<typename Arg::domain, expr<tag::post_inc, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::make(that);
    }

    template<typename Arg>
    inline typename generate<typename Arg::domain, expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_> > > >::type const
    operator --(Arg &arg, int)
    {
        expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_> > > that = {{arg}};
        return generate<typename Arg::domain, expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_> > > >::make(that);
    }

    template<typename Arg>
    inline typename generate<typename Arg::domain, expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::type const
    operator --(Arg const &arg, int)
    {
        expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_ const> > > that = {{arg}};
        return generate<typename Arg::domain, expr<tag::post_dec, args1<ref<typename Arg::boost_proto_expr_type_ const> > > >::make(that);
    }

#undef BOOST_PROTO_UNARY_OP
#undef BOOST_PROTO_BINARY_OP

}}

#endif
