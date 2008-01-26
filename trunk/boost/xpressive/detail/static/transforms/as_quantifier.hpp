///////////////////////////////////////////////////////////////////////////////
// as_quantifier.hpp
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_STATIC_TRANSFORMS_AS_QUANTIFIER_HPP_EAN_04_01_2007
#define BOOST_XPRESSIVE_DETAIL_STATIC_TRANSFORMS_AS_QUANTIFIER_HPP_EAN_04_01_2007

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/static/static.hpp>
#include <boost/xpressive/proto/proto.hpp>

namespace boost { namespace xpressive { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////////
    // generic_quant_tag
    template<uint_t Min, uint_t Max>
    struct generic_quant_tag
    {
        typedef mpl::integral_c<uint_t, Min> min_type;
        typedef mpl::integral_c<uint_t, Max> max_type;
    };
}}}

namespace boost { namespace xpressive { namespace grammar_detail
{
    using detail::uint_t;

    ///////////////////////////////////////////////////////////////////////////////
    // min_type / max_type
    template<typename Tag>
    struct min_type : Tag::min_type {};

    template<>
    struct min_type<proto::tag::posit> : mpl::integral_c<uint_t, 1> {};

    template<>
    struct min_type<proto::tag::dereference> : mpl::integral_c<uint_t, 0> {};

    template<>
    struct min_type<proto::tag::logical_not> : mpl::integral_c<uint_t, 0> {};

    template<typename Tag>
    struct max_type : Tag::max_type {};

    template<>
    struct max_type<proto::tag::posit> : mpl::integral_c<uint_t, UINT_MAX-1> {};

    template<>
    struct max_type<proto::tag::dereference> : mpl::integral_c<uint_t, UINT_MAX-1> {};

    template<>
    struct max_type<proto::tag::logical_not> : mpl::integral_c<uint_t, 1> {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_simple_quantifier
    template<typename Grammar, typename Greedy>
    struct as_simple_quantifier : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef typename proto::result_of::arg<Expr>::type arg_type;
            typedef typename Grammar::template result<void(arg_type, detail::true_xpression, Visitor)>::type xpr_type;
            typedef detail::simple_repeat_matcher<xpr_type, Greedy> matcher_type;
            typedef typename proto::terminal<matcher_type>::type type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            typedef result<void(Expr, State, Visitor)> result_;
            typedef typename result_::arg_type arg_type;
            typedef typename result_::xpr_type xpr_type;
            typedef typename result_::matcher_type matcher_type;
            typedef typename Expr::proto_tag tag;

            xpr_type const &xpr = Grammar()(proto::arg(expr), detail::true_xpression(), visitor);
            matcher_type matcher(xpr, (uint_t)min_type<tag>(), (uint_t)max_type<tag>(), xpr.get_width().value());
            return proto::terminal<matcher_type>::type::make(matcher);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // add_hidden_mark
    struct add_hidden_mark : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef
                typename shift_right<
                    terminal<detail::mark_begin_matcher>::type
                  , typename shift_right<
                        Expr
                      , terminal<detail::mark_end_matcher>::type
                    >::type
                >::type
            type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            // we're inserting a hidden mark ... so grab the next hidden mark number.
            int mark_nbr = visitor.get_hidden_mark();
            detail::mark_begin_matcher begin(mark_nbr);
            detail::mark_end_matcher end(mark_nbr);

            typename result<void(Expr, State, Visitor)>::type that
                = {{begin}, {expr, {end}}};
            return that;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // InsertMark
    struct InsertMark
      : or_<
            when<proto::assign<detail::basic_mark_tag, _>, _>
          , otherwise<add_hidden_mark>
        >
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_default_quantifier_impl
    template<typename Greedy, uint_t Min, uint_t Max>
    struct as_default_quantifier_impl : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef
                typename InsertMark::template result<void(typename proto::result_of::arg<Expr>::type, State, Visitor)>::type
            marked_sub_type;

            typedef
                typename shift_right<
                    terminal<detail::repeat_begin_matcher>::type
                  , typename shift_right<
                        marked_sub_type
                      , typename terminal<detail::repeat_end_matcher<Greedy> >::type
                    >::type
                >::type
            type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            // Ensure this sub-expression is book-ended with mark matchers
            typename result<void(Expr, State, Visitor)>::marked_sub_type const &
                marked_sub = InsertMark()(proto::arg(expr), state, visitor);

            // Get the mark_number from the begin_mark_matcher
            int mark_number = proto::arg(proto::left(marked_sub)).mark_number_;
            BOOST_ASSERT(0 != mark_number);

            uint_t min_ = (uint_t)min_type<typename Expr::proto_tag>();
            uint_t max_ = (uint_t)max_type<typename Expr::proto_tag>();

            detail::repeat_begin_matcher begin(mark_number);
            detail::repeat_end_matcher<Greedy> end(mark_number, min_, max_);

            typename result<void(Expr, State, Visitor)>::type that
                = {{begin}, {marked_sub, {end}}};
            return that;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // optional_tag
    template<typename Greedy>
    struct optional_tag
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_default_optional
    template<typename Grammar, typename Greedy>
    struct as_default_optional : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef detail::optional_matcher<
                typename Grammar::template result<void(Expr, detail::alternate_end_xpression, Visitor)>::type
              , Greedy
            > type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            return typename result<void(Expr, State, Visitor)>::type(
                Grammar()(expr, detail::alternate_end_xpression(), visitor)
            );
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // as_mark_optional
    template<typename Grammar, typename Greedy>
    struct as_mark_optional : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef detail::optional_mark_matcher<
                typename Grammar::template result<void(Expr, detail::alternate_end_xpression, Visitor)>::type
              , Greedy
            > type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            int mark_number = proto::arg(proto::left(expr)).mark_number_;
            return typename result<void(Expr, State, Visitor)>::type(
                Grammar()(expr, detail::alternate_end_xpression(), visitor)
              , mark_number
            );
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // IsMarkerOrRepeater
    struct IsMarkerOrRepeater
      : or_<
            shift_right<terminal<detail::repeat_begin_matcher>, _>
          , assign<terminal<detail::mark_placeholder>, _>
        >
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_optional
    template<typename Grammar, typename Greedy>
    struct as_optional
      : or_<
            when<IsMarkerOrRepeater, as_mark_optional<Grammar, Greedy> >
          , otherwise<as_default_optional<Grammar, Greedy> > 
        >
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // make_optional_
    template<typename Greedy>
    struct make_optional_ : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef typename unary_expr<optional_tag<Greedy>, Expr>::type type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename unary_expr<optional_tag<Greedy>, Expr>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            typename unary_expr<optional_tag<Greedy>, Expr>::type that = {expr};
            return that;
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // as_default_quantifier_impl
    template<typename Greedy, uint_t Max>
    struct as_default_quantifier_impl<Greedy, 0, Max>
      : call<make_optional_<Greedy>(as_default_quantifier_impl<Greedy, 1, Max>)>
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_default_quantifier_impl
    template<typename Greedy>
    struct as_default_quantifier_impl<Greedy, 0, 1>
      : call<make_optional_<Greedy>(_arg)>
    {};

    ///////////////////////////////////////////////////////////////////////////////
    // as_default_quantifier
    template<typename Greedy>
    struct as_default_quantifier : proto::callable
    {
        template<typename Sig> struct result {};

        template<typename This, typename Expr, typename State, typename Visitor>
        struct result<This(Expr, State, Visitor)>
        {
            typedef
                as_default_quantifier_impl<
                    Greedy
                  , min_type<typename Expr::proto_tag>::value
                  , max_type<typename Expr::proto_tag>::value
                >
            impl;

            typedef typename impl::template result<void(Expr, State, Visitor)>::type type;
        };

        template<typename Expr, typename State, typename Visitor>
        typename result<void(Expr, State, Visitor)>::type
        operator ()(Expr const &expr, State const &state, Visitor &visitor) const
        {
            return as_default_quantifier_impl<
                Greedy
              , min_type<typename Expr::proto_tag>::value
              , max_type<typename Expr::proto_tag>::value
            >()(expr, state, visitor);
        }
    };

}}}

#endif
