///////////////////////////////////////////////////////////////////////////////
// transform.hpp
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_STATIC_TRANSFORMS_TRANSFORM_HPP_EAN_04_01_2007
#define BOOST_XPRESSIVE_DETAIL_STATIC_TRANSFORMS_TRANSFORM_HPP_EAN_04_01_2007

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/proto/proto.hpp>
#include <boost/xpressive/proto/transform/fold.hpp>
#include <boost/xpressive/detail/static/transforms/as_matcher.hpp>
#include <boost/xpressive/detail/static/transforms/as_alternate.hpp>
#include <boost/xpressive/detail/static/transforms/as_sequence.hpp>
#include <boost/xpressive/detail/static/transforms/as_quantifier.hpp>
#include <boost/xpressive/detail/static/transforms/as_marker.hpp>
#include <boost/xpressive/detail/static/transforms/as_set.hpp>
#include <boost/xpressive/detail/static/transforms/as_independent.hpp>
#include <boost/xpressive/detail/static/transforms/as_modifier.hpp>
#include <boost/xpressive/detail/static/transforms/as_inverse.hpp>
#include <boost/xpressive/detail/static/transforms/as_action.hpp>
#include <boost/xpressive/proto/transform/arg.hpp>
#include <boost/xpressive/proto/transform/compose.hpp>

namespace boost { namespace xpressive { namespace detail
{
    template<typename Char>
    struct Grammar;

    ///////////////////////////////////////////////////////////////////////////
    // CharLiteral
    template<typename Char>
    struct CharLiteral
      : proto::or_<
            proto::terminal<char>
          , proto::terminal<Char>
        >
    {};

    template<>
    struct CharLiteral<char>
      : proto::terminal<char>
    {};

    ///////////////////////////////////////////////////////////////////////////
    // ListSet
    template<typename Char>
    struct ListSet
      : proto::or_<
            proto::comma<ListSet<Char>, CharLiteral<Char> >
          , proto::assign<set_initializer_type, CharLiteral<Char> >
        >
    {};

    ///////////////////////////////////////////////////////////////////////////
    // as_repeat
    template<typename Char, typename Tag, bool Greedy>
    struct as_repeat
      : proto::trans::conditional<
            use_simple_repeat<proto::result_of::arg<mpl::_> >
          , as_simple_quantifier<proto::unary_expr<Tag, Grammar<Char> >, Greedy>
          , as_default_quantifier<proto::unary_expr<Tag, Grammar<Char> >, Greedy>
        >
    {};

    ///////////////////////////////////////////////////////////////////////////
    // NonGreedyRepeatCases
    template<typename Char>
    struct NonGreedyRepeatCases
    {
        template<typename Tag, typename Dummy = void>
        struct case_
          : proto::not_<proto::_>
        {};

        template<typename Dummy>
        struct case_<proto::tag::unary_star, Dummy>
          : as_repeat<Char, proto::tag::unary_star, false>
        {};

        template<typename Dummy>
        struct case_<proto::tag::unary_plus, Dummy>
          : as_repeat<Char, proto::tag::unary_plus, false>
        {};

        template<typename Dummy>
        struct case_<proto::tag::logical_not, Dummy>
          : as_repeat<Char, proto::tag::logical_not, false>
        {};

        template<uint_t Min, uint_t Max, typename Dummy>
        struct case_<generic_quant_tag<Min, Max>, Dummy>
          : as_repeat<Char, generic_quant_tag<Min, Max>, false>
        {};
    };

    ///////////////////////////////////////////////////////////////////////////
    // InvertibleCases
    template<typename Char>
    struct InvertibleCases
    {
        template<typename Tag, typename Dummy = void>
        struct case_
          : proto::not_<proto::_>
        {};

        template<typename Dummy>
        struct case_<proto::tag::comma, Dummy>
          : as_list_set<ListSet<Char> >
        {};

        template<typename Dummy>
        struct case_<proto::tag::assign, Dummy>
          : as_list_set<ListSet<Char> >
        {};

        template<typename Dummy>
        struct case_<proto::tag::subscript, Dummy>
          : proto::trans::right<proto::subscript<set_initializer_type, as_set<Grammar<Char> > > >
        {};

        template<typename Dummy>
        struct case_<lookahead_tag<true>, Dummy>
          : proto::trans::arg<proto::unary_expr<lookahead_tag<true>, as_lookahead<Grammar<Char> > > >
        {};

        template<typename Dummy>
        struct case_<lookbehind_tag<true>, Dummy>
          : proto::trans::arg<proto::unary_expr<lookbehind_tag<true>, as_lookbehind<Grammar<Char> > > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::terminal, Dummy>
          : proto::or_<
                as_matcher<CharLiteral<Char> >
              , as_matcher<proto::terminal<posix_charset_placeholder> >
              , as_matcher<proto::terminal<range_placeholder<proto::_> > >
              , as_matcher<proto::terminal<logical_newline_placeholder> >
              , as_matcher<proto::terminal<assert_word_placeholder<word_boundary<true> > > >
            >
        {};
    };

    ///////////////////////////////////////////////////////////////////////////
    // Cases
    template<typename Char>
    struct Cases
    {
        template<typename Tag, typename Dummy = void>
        struct case_
          : proto::not_<proto::_>
        {};

        template<typename Dummy>
        struct case_<proto::tag::right_shift, Dummy>
          : proto::trans::reverse_fold<proto::right_shift<Grammar<Char>, Grammar<Char> > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::terminal, Dummy>
          : in_sequence<as_matcher<proto::terminal<proto::_> > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::bitwise_or, Dummy>
          : in_sequence<as_alternate<proto::bitwise_or<Grammar<Char>, Grammar<Char> > > >
        {};

        template<typename Dummy, bool Greedy>
        struct case_<optional_tag<Greedy> , Dummy>
          : in_sequence<proto::trans::arg<proto::unary_expr<optional_tag<Greedy>, as_optional<Grammar<Char>, Greedy> > > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::unary_star, Dummy>
          : proto::trans::compose<as_repeat<Char, proto::tag::unary_star, true>, Grammar<Char> >
        {};

        template<typename Dummy>
        struct case_<proto::tag::unary_plus, Dummy>
          : proto::trans::compose<as_repeat<Char, proto::tag::unary_plus, true>, Grammar<Char> >
        {};

        template<typename Dummy>
        struct case_<proto::tag::logical_not, Dummy>
          : proto::trans::compose<as_repeat<Char, proto::tag::logical_not, true>, Grammar<Char> >
        {};

        template<uint_t Min, uint_t Max, typename Dummy>
        struct case_<generic_quant_tag<Min, Max> , Dummy>
          : proto::trans::compose<as_repeat<Char, generic_quant_tag<Min, Max>, true>, Grammar<Char> >
        {};

        template<typename Dummy>
        struct case_<proto::tag::unary_minus, Dummy>
          : proto::trans::compose<
                proto::trans::arg<proto::unary_minus<proto::switch_<NonGreedyRepeatCases<Char> > > >
              , Grammar<Char>
            >
        {};

        template<typename Dummy>
        struct case_<proto::tag::complement, Dummy>
          : in_sequence<as_inverse<
                proto::trans::arg<proto::complement<proto::switch_<InvertibleCases<Char> > > >
            > >
        {};

        template<typename Dummy>
        struct case_<modifier_tag, Dummy>
          : as_modifier<proto::binary_expr<modifier_tag, proto::_, Grammar<Char> > >
        {};

        template<typename Dummy>
        struct case_<lookahead_tag<true> , Dummy>
          : in_sequence<proto::trans::arg<proto::unary_expr<lookahead_tag<true>, as_lookahead<Grammar<Char> > > > >
        {};

        template<typename Dummy>
        struct case_<lookbehind_tag<true> , Dummy>
          : in_sequence<proto::trans::arg<proto::unary_expr<lookbehind_tag<true>, as_lookbehind<Grammar<Char> > > > >
        {};

        template<typename Dummy>
        struct case_<keeper_tag, Dummy>
          : in_sequence<proto::trans::arg<proto::unary_expr<keeper_tag, as_keeper<Grammar<Char> > > > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::comma, Dummy>
          : in_sequence<as_list_set<ListSet<Char> > >
        {};

        template<typename Dummy>
        struct case_<proto::tag::assign, Dummy>
          : proto::or_<
                proto::trans::compose<as_marker<proto::assign<basic_mark_tag, Grammar<Char> > >, Grammar<Char> >
              , in_sequence<as_list_set<ListSet<Char> > >
            >
        {};

        template<typename Dummy>
        struct case_<proto::tag::subscript, Dummy>
          : proto::or_<
                in_sequence<proto::trans::right<proto::subscript<set_initializer_type, as_set<Grammar<Char> > > > >
              , proto::trans::compose<as_action<proto::subscript<Grammar<Char>, proto::_> >, Grammar<Char> >
            >
        {};
    };

    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    template<typename Char>
    struct Grammar
      : proto::switch_<Cases<Char> >
    {};

    ///////////////////////////////////////////////////////////////////////////
    //
    namespace meta
    {
        template<typename Expr, typename Visitor, typename EnableIf = void>
        struct transform
          : proto::terminal<any_matcher>
        {
            static typename transform::type call(Expr const &, Visitor &)
            {
                return transform::type::make(any_matcher());
            }
        };

        template<typename Expr, typename Visitor>
        struct transform<
            Expr
          , Visitor
          , typename enable_if<proto::matches<Expr, Grammar<typename Visitor::char_type> > >::type
        >
          : Grammar<typename Visitor::char_type>::template apply<Expr, end_xpression, Visitor>
        {
            static typename transform::type call(Expr const &expr, Visitor &visitor)
            {
                return Grammar<typename Visitor::char_type>::call(expr, end_xpression(), visitor);
            }
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    // INVALID_REGULAR_EXPRESSION
    struct INVALID_REGULAR_EXPRESSION
      : mpl::false_
    {};

    ///////////////////////////////////////////////////////////////////////////
    // transform()
    template<typename Expr, typename Visitor>
    typename meta::transform<Expr, Visitor>::type
    transform(Expr const &expr, Visitor &visitor)
    {
        typedef typename Visitor::char_type char_type;
        typedef
            typename mpl::if_<
                proto::matches<Expr, Grammar<char_type> >
              , mpl::true_
              , INVALID_REGULAR_EXPRESSION
            >::type
        does_expression_match_xpressive_grammar;

        BOOST_MPL_ASSERT((does_expression_match_xpressive_grammar));
        return meta::transform<Expr, Visitor>::call(expr, visitor);
    }

}}}

#endif
