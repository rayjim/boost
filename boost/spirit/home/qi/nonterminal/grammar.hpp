/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_GRAMMAR_FEBRUARY_19_2007_0236PM)
#define BOOST_SPIRIT_GRAMMAR_FEBRUARY_19_2007_0236PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/nonterminal/rule.hpp>
#include <boost/spirit/home/qi/reference.hpp>
#include <boost/noncopyable.hpp>

namespace boost { namespace spirit { namespace qi
{
    template <
        typename Iterator
      , typename T1 = unused_type
      , typename T2 = unused_type
      , typename T3 = unused_type
    >
    struct grammar
      : proto::extends<
            typename proto::terminal<
                reference<rule<Iterator, T1, T2, T3> const>
            >::type
          , grammar<Iterator, T1, T2, T3>
        >
      , parser<grammar<Iterator, T1, T2, T3> >
      , noncopyable
    {
        typedef Iterator iterator_type;
        typedef rule<Iterator, T1, T2, T3> start_type;
        typedef typename start_type::sig_type sig_type;
        typedef typename start_type::locals_type locals_type;
        typedef typename start_type::skipper_type skipper_type;
        typedef grammar<Iterator, T1, T2, T3> base_type;
        typedef reference<start_type const> reference_;
        typedef typename proto::terminal<reference_>::type terminal;

        static size_t const params_size = start_type::params_size;

        grammar(
            start_type const& start
          , std::string const& name_ = "unnamed-grammar")
        : proto::extends<terminal, base_type>(terminal::make(start.alias()))
        , name_(name_)
        {}

        std::string name() const
        {
            return name_;
        }

        void name(std::string const& str)
        {
            name_ = str;
        }

        template <typename Context, typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Skipper const& skipper
          , Attribute& attr) const
        {
            return this->proto_base().child0.parse(
                first, last, context, skipper, attr);
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info(name_);
        }

        // bring in the operator() overloads
        start_type const& get_rule() const
        { return this->proto_base().child0.ref.get(); }
        typedef start_type rule_type;
        #include <boost/spirit/home/qi/nonterminal/detail/fcall.hpp>

        std::string name_;

    };
}}}

#endif
