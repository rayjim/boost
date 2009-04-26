//  Copyright (c) 2001-2009 Joel de Guzman
//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_GRAMMAR_MAR_05_2007_0542PM)
#define BOOST_SPIRIT_KARMA_GRAMMAR_MAR_05_2007_0542PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/nonterminal/rule.hpp>
#include <boost/spirit/home/karma/reference.hpp>
#include <boost/noncopyable.hpp>

namespace boost { namespace spirit { namespace karma
{
    template <
        typename OutputIterator
      , typename T1 = unused_type
      , typename T2 = unused_type
      , typename T3 = unused_type
    >
    struct grammar
      : proto::extends<
            typename proto::terminal<
                reference<rule<OutputIterator, T1, T2, T3> const>
            >::type
          , grammar<OutputIterator, T1, T2, T3>
        >
      , generator<grammar<OutputIterator, T1, T2, T3> >
      , noncopyable
    {
        typedef OutputIterator iterator_type;
        typedef rule<OutputIterator, T1, T2, T3> start_type;
        typedef typename start_type::sig_type sig_type;
        typedef typename start_type::locals_type locals_type;
        typedef typename start_type::delimiter_type delimiter_type;
        typedef grammar<OutputIterator, T1, T2, T3> base_type;
        typedef reference<start_type const> reference_;
        typedef typename proto::terminal<reference_>::type terminal;
        static size_t const params_size = start_type::params_size;

        // the output iterator is always wrapped by karma
        typedef detail::output_iterator<OutputIterator> output_iterator;

        grammar(start_type const& start
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

        template <typename Context, typename Delimiter, typename Attribute>
        bool generate(output_iterator& sink, Context& context
          , Delimiter const& delim, Attribute const& attr) const
        {
            return this->proto_base().child0.generate(
                sink, context, delim, attr);
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
        #include <boost/spirit/home/karma/nonterminal/detail/fcall.hpp>

        std::string name_;
    };

}}}

#endif
