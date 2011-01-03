/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_ARGUMENT_FEBRUARY_17_2007_0339PM)
#define BOOST_SPIRIT_ARGUMENT_FEBRUARY_17_2007_0339PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/spirit/home/phoenix/core/actor.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/support/assert_msg.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#if !defined(SPIRIT_ARGUMENTS_LIMIT)
# define SPIRIT_ARGUMENTS_LIMIT PHOENIX_LIMIT
#endif

#define SPIRIT_DECLARE_ARG(z, n, data)                                          \
    phoenix::actor<argument<n> > const                                          \
        BOOST_PP_CAT(_, BOOST_PP_INC(n)) = argument<n>();

#define SPIRIT_USING_ARGUMENT(z, n, data) using spirit::BOOST_PP_CAT(_, n);

namespace boost { namespace spirit
{
    namespace result_of
    {
        template <typename Sequence, int N>
        struct get_arg
        {
            typedef typename
                fusion::result_of::size<Sequence>::type
            sequence_size;

            // report invalid argument not found (N is out of bounds)
            BOOST_SPIRIT_ASSERT_MSG(
                (N < sequence_size::value),
                index_is_out_of_bounds, ());

            typedef typename
                fusion::result_of::at_c<Sequence, N>::type
            type;

            static type call(Sequence& seq)
            {
                return fusion::at_c<N>(seq);
            }
        };

        template <typename Sequence, int N>
        struct get_arg<Sequence&, N> : get_arg<Sequence, N>
        {
        };
    }

    template <int N, typename T>
    typename result_of::get_arg<T, N>::type
    get_arg(T& val)
    {
        return result_of::get_arg<T, N>::call(val);
    }

    struct attribute_context
    {
        typedef mpl::true_ no_nullary;

        template <typename Env>
        struct result
        {
            // FIXME: is this remove_const really necessary?
            typedef typename
                remove_const<
                    typename mpl::at_c<typename Env::args_type, 0>::type
                >::type
            type;
        };

        template <typename Env>
        typename result<Env>::type
        eval(Env const& env) const
        {
            return fusion::at_c<0>(env.args());
        }
    };

    template <int N>
    struct argument
    {
        typedef mpl::true_ no_nullary;

        template <typename Env>
        struct result
        {
            typedef typename
                mpl::at_c<typename Env::args_type, 0>::type
            arg_type;

            typedef typename result_of::get_arg<arg_type, N>::type type;
        };

        template <typename Env>
        typename result<Env>::type
        eval(Env const& env) const
        {
            return get_arg<N>(fusion::at_c<0>(env.args()));
        }
    };

    // _0 refers to the whole attribute as generated by the lhs parser
    phoenix::actor<attribute_context> const _0 = attribute_context();

    // _1, _2, ... refer to the attributes of the single components the lhs
    // parser is composed of
    phoenix::actor<argument<0> > const _1 = argument<0>();
    phoenix::actor<argument<1> > const _2 = argument<1>();
    phoenix::actor<argument<2> > const _3 = argument<2>();

    // '_pass' may be used to make a match fail in retrospective
    phoenix::actor<phoenix::argument<2> > const _pass = phoenix::argument<2>();

    //  Bring in the rest of the arguments and attributes (_4 .. _N+1), using PP
    BOOST_PP_REPEAT_FROM_TO(
        3, SPIRIT_ARGUMENTS_LIMIT, SPIRIT_DECLARE_ARG, _)

    // You can bring these in with the using directive
    // without worrying about bringing in too much.
    namespace labels
    {
        BOOST_PP_REPEAT(SPIRIT_ARGUMENTS_LIMIT, SPIRIT_USING_ARGUMENT, _)
    }

}}

#undef SPIRIT_DECLARE_ARG
#endif
