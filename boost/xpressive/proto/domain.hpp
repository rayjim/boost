///////////////////////////////////////////////////////////////////////////////
/// \file domain.hpp
/// Contains definition of domain\<\> class template and helpers for
/// defining domains with a generator and a grammar for controlling
/// operator overloading.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_DOMAIN_HPP_EAN_02_13_2007
#define BOOST_PROTO_DOMAIN_HPP_EAN_02_13_2007

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/generate.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto
{

    namespace detail
    {
        struct not_a_generator
        {};

        struct not_a_grammar
        {};
    }

    namespace domainns_
    {
        /// \brief For use in defining domain tags to be used
        /// with \c proto::extends\<\>. A \e Domain associates
        /// an expression type with a \e Generator, and optionally
        /// a \e Grammar.
        ///
        /// The Generator determines how new expressions in the
        /// domain are constructed. Typically, a generator wraps
        /// all new expressions in a wrapper that imparts
        /// domain-specific behaviors to expressions within its
        /// domain. (See \c proto::extends\<\>.)
        ///
        /// The Grammar determines whether a given expression is
        /// valid within the domain, and automatically disables
        /// any operator overloads which would cause an invalid
        /// expression to be created. By default, the Grammar
        /// parameter defaults to the wildcard, \c proto::_, which
        /// makes all expressions valid within the domain.
        ///
        template<typename Generator, typename Grammar>
        struct domain
          : Generator
        {
            typedef Grammar proto_grammar;

            /// INTERNAL ONLY
            ///
            typedef void proto_is_domain_;
        };

        /// \brief The domain expressions have by default, if
        /// \c proto::extends\<\> has not been used to associate
        /// a domain with an expression.
        ///
        struct default_domain
          : domain<>
        {};

        /// \brief A pseudo-domain for use in functions and
        /// metafunctions that require a domain parameter. It
        /// indicates that the domain of the parent node should
        /// be inferred from the domains of the children nodes.
        ///
        /// \attention \c deduce_domain is not itself a valid domain.
        ///
        struct deduce_domain
          : domain<detail::not_a_generator, detail::not_a_grammar>
        {};
    }

    namespace result_of
    {
        /// A metafunction that returns \c mpl::true_
        /// if the type \c T is the type of a Prot domain;
        /// \c mpl::false_ otherwise. If \c T inherits from
        /// \c proto::domain\<\>, \c is_domain\<T\> is
        /// \c mpl::true_.
        template<typename T, typename EnableIf>
        struct is_domain
          : mpl::false_
        {};

        /// INTERNAL ONLY
        ///
        template<typename T>
        struct is_domain<T, typename T::proto_is_domain_>
          : mpl::true_
        {};

        /// A metafunction that returns the domain of
        /// a given type. If \c T is a Proto expression
        /// type, it returns that expression's associated
        /// domain. If not, it returns
        /// \c proto::default_domain.
        template<typename T, typename EnableIf>
        struct domain_of
        {
            typedef default_domain type;
        };

        /// INTERNAL ONLY
        ///
        template<typename T>
        struct domain_of<T, typename T::proto_is_expr_>
        {
            typedef typename T::proto_domain type;
        };
    }
}}

#endif
