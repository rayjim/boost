//  Copyright (c) 2001-2009 Joel de Guzman
//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_GENERATOR_BINDER_APR_17_2009_0952PM)
#define BOOST_SPIRIT_GENERATOR_BINDER_APR_17_2009_0952PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <utility>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find_if.hpp>

namespace boost { namespace spirit { namespace karma { namespace detail
{
    // generator_binder for plain rules
    template <typename Generator, typename Auto>
    struct generator_binder
    {
        generator_binder(Generator const& g)
          : g(g) {}

        template <typename OutputIterator, typename Delimiter, typename Context>
        bool operator()(OutputIterator& sink, Context& context
          , Delimiter const& delim) const
        {
            // If Auto is false, the component's attribute is unused.
            return g.generate(sink, context, delim, unused);
        }

        Generator g;
    };

    // generator_binder for auto rules
    template <typename Generator>
    struct generator_binder<Generator, mpl::true_>
    {
        generator_binder(Generator const& g)
          : g(g) {};

        template <typename OutputIterator, typename Delimiter, typename Context>
        bool operator()(OutputIterator& sink, Context& context
          , Delimiter const& delim) const
        {
            // If Auto is true, we pass the rule's attribute on to the component.
            return g.generate(sink, context, delim
                , fusion::at_c<0>(context.attributes));
        }

        Generator g;
    };

    template <typename Auto, typename Generator>
    inline generator_binder<Generator, Auto>
    bind_generator(Generator const& g)
    {
        return generator_binder<Generator, Auto>(g);
    }

    template <typename Types, typename Pred, typename Default>
    struct extract_param
    {
        typedef typename mpl::find_if<Types, Pred>::type pos;

        typedef typename
            mpl::eval_if<
                is_same<pos, typename mpl::end<Types>::type>
              , mpl::identity<Default>
              , mpl::deref<pos>
            >::type
        type;
    };

}}}}

#endif
