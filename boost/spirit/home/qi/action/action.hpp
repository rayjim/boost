/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_ACTION_JANUARY_07_2007_1128AM)
#define SPIRIT_ACTION_JANUARY_07_2007_1128AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/support/argument.hpp>
#include <boost/spirit/home/support/context.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/action_dispatch.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace spirit { namespace qi
{
    BOOST_PP_REPEAT(SPIRIT_ARGUMENTS_LIMIT, SPIRIT_USING_ARGUMENT, _)

    template <typename Subject, typename Action>
    struct action : unary_parser<action<Subject, Action> >
    {
        typedef Subject subject_type;
        typedef Action action_type;

        template <typename Context, typename Iterator>
        struct attribute
          : traits::attribute_of<Subject, Context, Iterator>
        {};

        action(Subject const& subject, Action f)
          : subject(subject), f(f) {}

        template <typename Iterator, typename Context
          , typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr_) const
        {
            typedef typename attribute<Context, Iterator>::type attr_type;
            typedef traits::make_attribute<attr_type, Attribute> make_attribute;

            // create an attribute if one is not supplied
            typename make_attribute::type attr = make_attribute::call(attr_);

            if (subject.parse(first, last, context, skipper, attr))
            {
                // call the function, passing the attribute, the context.
                // The client can return false to fail parsing.
                return traits::action_dispatch<Subject>()(f, attr, context);
            }
            return false;
        }

        template <typename Context>
        info what(Context& context) const
        {
            // the action is transparent (does not add any info)
            return subject.what(context);
        }

        Subject subject;
        Action f;

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        action& operator= (action const&);
    };
}}}

namespace boost { namespace spirit
{
    // Qi action meta-compiler
    template <>
    struct make_component<qi::domain, tag::action>
    {
        template <typename Sig>
        struct result;

        template <typename This, typename Elements, typename Modifiers>
        struct result<This(Elements, Modifiers)>
        {
            typedef typename
                remove_const<typename Elements::car_type>::type
            subject_type;

            typedef typename
                remove_const<typename Elements::cdr_type::car_type>::type
            action_type;

            typedef qi::action<subject_type, action_type> type;
        };

        template <typename Elements>
        typename result<make_component(Elements, unused_type)>::type
        operator()(Elements const& elements, unused_type) const
        {
            typename result<make_component(Elements, unused_type)>::type
                result(elements.car, elements.cdr.car);
            return result;
        }
    };
}}

namespace boost { namespace spirit { namespace traits
{
    template <typename Subject, typename Action>
    struct has_semantic_action<qi::action<Subject, Action> >
      : mpl::true_ {};

}}}

#endif
