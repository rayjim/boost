//-----------------------------------------------------------------------------
// boost variant/recursive_variant.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_RECURSIVE_VARIANT_HPP
#define BOOST_VARIANT_RECURSIVE_VARIANT_HPP

#include "boost/variant/variant_fwd.hpp"
#include "boost/variant/detail/enable_recursive.hpp"
#include "boost/variant/detail/substitute_fwd.hpp"
#include "boost/variant/detail/make_variant_list.hpp"

#include "boost/mpl/aux_/lambda_arity_param.hpp"

#if !defined(BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT)
#   include "boost/mpl/apply_if.hpp"
#   include "boost/mpl/identity.hpp"
#   include "boost/mpl/is_sequence.hpp"
#   include "boost/mpl/protect.hpp"
#   include "boost/mpl/transform.hpp"
#else
#   include "boost/preprocessor/cat.hpp"
#   include "boost/preprocessor/repeat.hpp"
#endif

#include "boost/mpl/bool.hpp"
#include "boost/variant/variant.hpp"

namespace boost {

namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction specialization substitute
//
// Handles embedded variant types when substituting for recursive_variant_.
//

#if !defined(BOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

template <
      BOOST_VARIANT_ENUM_PARAMS(typename T)
    , typename RecursiveVariant
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(typename Arity)
    >
struct substitute<
      ::boost::variant<
          recursive_flag< T0 >
        , BOOST_VARIANT_ENUM_SHIFTED_PARAMS(T)
        >
    , RecursiveVariant
    , ::boost::recursive_variant_
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(Arity)
    >
{
    typedef ::boost::variant<
          recursive_flag< T0 >
        , BOOST_VARIANT_ENUM_SHIFTED_PARAMS(T)
        > type;
};

template <
      BOOST_VARIANT_ENUM_PARAMS(typename T)
    , typename RecursiveVariant
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(typename Arity)
    >
struct substitute<
      ::boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) >
    , RecursiveVariant
    , ::boost::recursive_variant_
      BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(Arity)
    >
{

#if !defined(BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT)

private: // helpers, for metafunction result (below)

    typedef typename mpl::apply_if<
          mpl::is_sequence<T0>
        , mpl::identity< T0 >
        , make_variant_list< BOOST_VARIANT_ENUM_PARAMS(T) >
        >::type initial_types;

    typedef typename mpl::transform<
          initial_types
        , mpl::protect< quoted_enable_recursive<RecursiveVariant,mpl::true_> >
        >::type types;

public: // metafunction result

    typedef ::boost::variant< types > type;

#else // defined(BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT)

private: // helpers, for metafunction result (below)

    #define BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS(z,N,_)  \
        typedef typename enable_recursive<   \
              BOOST_PP_CAT(T,N)              \
            , RecursiveVariant               \
            , mpl::true_                     \
            >::type BOOST_PP_CAT(wknd_T,N);  \
        /**/

    BOOST_PP_REPEAT(
          BOOST_VARIANT_LIMIT_TYPES
        , BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS
        , _
        )

    #undef BOOST_VARIANT_AUX_ENABLE_RECURSIVE_TYPEDEFS

public: // metafunction result

    typedef ::boost::variant< BOOST_VARIANT_ENUM_PARAMS(wknd_T) > type;

#endif // BOOST_VARIANT_NO_TYPE_SEQUENCE_SUPPORT workaround

};

#else // defined(BOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

//
// no specializations: embedded variants unsupported on these compilers!
//

#endif // !defined(BOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

}} // namespace detail::variant

///////////////////////////////////////////////////////////////////////////////
// metafunction recursive_variant
//
// See docs and boost/variant/variant_fwd.hpp for more information.
//
template < BOOST_VARIANT_ENUM_PARAMS(typename T) >
struct recursive_variant
{
public: // metafunction result

    typedef boost::variant<
          detail::variant::recursive_flag< T0 >
        , BOOST_VARIANT_ENUM_SHIFTED_PARAMS(T)
        > type;

};

} // namespace boost

#endif // BOOST_VARIANT_RECURSIVE_VARIANT_HPP
