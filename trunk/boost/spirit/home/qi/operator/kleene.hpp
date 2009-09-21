/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_KLEENE_JANUARY_07_2007_0818AM)
#define SPIRIT_KLEENE_JANUARY_07_2007_0818AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/meta_compiler.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/container.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>

namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_operator<qi::domain, proto::tag::dereference> // enables *p
      : mpl::true_ {};
}}

namespace boost { namespace spirit { namespace qi
{
    template <typename Subject>
    struct kleene : unary_parser<kleene<Subject> >
    {
        typedef Subject subject_type;

        template <typename Context, typename Iterator>
        struct attribute
        {
            // Build a std::vector from the subject's attribute. Note
            // that build_std_vector may return unused_type if the
            // subject's attribute is an unused_type.
            typedef typename
                traits::build_std_vector<
                    typename traits::
                        attribute_of<Subject, Context, Iterator>::type
                >::type
            type;
        };

        kleene(Subject const& subject)
          : subject(subject) {}

        template <typename Iterator, typename Context
          , typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr) const
        {
            // create a local value if Attribute is not unused_type
            typedef typename traits::result_of::value<Attribute>::type 
                value_type;
            value_type val = value_type();

            // Repeat while subject parses ok
            while (subject.parse(first, last, context, skipper, val))
            {
                // push the parsed value into our attribute
                traits::push_back(attr, val);
                traits::clear(val);
            }
            return true;
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info("kleene", subject.what(context));
        }

        Subject subject;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Elements, typename Modifiers>
    struct make_composite<proto::tag::dereference, Elements, Modifiers>
      : make_unary_composite<Elements, kleene>
    {};
}}}

namespace boost { namespace spirit { namespace traits
{
    template <typename T>
    struct has_semantic_action;

    template <typename Subject>
    struct has_semantic_action<qi::kleene<Subject> >
      : unary_has_semantic_action<Subject> {};
}}}

#endif
