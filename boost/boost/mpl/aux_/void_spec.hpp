//-----------------------------------------------------------------------------
// boost mpl/aux_/void_spec.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_VOID_SPEC_HPP_INCLUDED
#define BOOST_MPL_AUX_VOID_SPEC_HPP_INCLUDED

#include "boost/mpl/void.hpp"
#include "boost/mpl/aux_/arity.hpp"
#include "boost/mpl/aux_/template_arity_fwd.hpp"
#include "boost/mpl/aux_/config/dtp.hpp"
#include "boost/mpl/aux_/config/ttp.hpp"
#include "boost/mpl/aux_/config/lambda_support.hpp"
#include "boost/mpl/aux_/config/overload_resolution.hpp"
#include "boost/mpl/aux_/preprocessor/params.hpp"
#include "boost/mpl/aux_/preprocessor/enum.hpp"
#include "boost/mpl/aux_/preprocessor/def_params_tail.hpp"

#include "boost/config.hpp"

#define BOOST_MPL_AUX_VOID_SPEC_PARAMS(i) \
    BOOST_MPL_PP_ENUM(i, void_) \
/**/

#if defined(BOOST_NO_DEFAULT_TEMPLATE_PARAMETERS_IN_NESTED_TEMPLATES)
#   define BOOST_MPL_AUX_VOID_SPEC_ARITY(i, name) \
namespace aux { \
template< int N > \
struct arity< \
      name< BOOST_MPL_AUX_VOID_SPEC_PARAMS(i) > \
    , N \
    > \
{ \
    BOOST_STATIC_CONSTANT(int \
        , value = BOOST_MPL_METAFUNCTION_MAX_ARITY \
        ); \
}; \
} \
/**/
#else
#   define BOOST_MPL_AUX_VOID_SPEC_ARITY(i, name) /**/
#endif

#if defined(BOOST_EXTENDED_TEMPLATE_PARAMETERS_MATCHING) || \
    defined(BOOST_MPL_NO_FULL_LAMBDA_SUPPORT) && \
    defined(BOOST_MPL_BROKEN_OVERLOAD_RESOLUTION)
#   define BOOST_MPL_AUX_VOID_SPEC_TEMPLATE_ARITY(i, j, name) \
namespace aux { \
template< BOOST_MPL_PP_PARAMS(j, typename T) > \
struct template_arity< \
      name< BOOST_MPL_PP_PARAMS(j, T) > \
    > \
{ \
    BOOST_STATIC_CONSTANT(int, value = j ); \
}; \
\
template<> \
struct template_arity< \
      name< BOOST_MPL_PP_ENUM(i, void_) > \
    > \
{ \
    BOOST_STATIC_CONSTANT(int, value = j ); \
}; \
} \
/**/
#else
#   define BOOST_MPL_AUX_VOID_SPEC_TEMPLATE_ARITY(i, j, name) /**/
#endif

#define BOOST_MPL_AUX_VOID_SPEC_MAIN(i, name) \
template<> \
struct name< BOOST_MPL_AUX_VOID_SPEC_PARAMS(i) > \
{ \
    template< \
          BOOST_MPL_PP_PARAMS(i, typename T) \
        BOOST_MPL_PP_DEF_PARAMS_TAIL(i, typename T) \
        > \
    struct apply \
        : name< BOOST_MPL_PP_PARAMS(i, T) > \
    { \
    }; \
}; \
/**/

#define BOOST_MPL_AUX_VOID_SPEC_PARAM(param) param = void_

// agurt, 16/sep/02: temporary fix for VisualAge C++
#if defined(__IBMCPP__)
#   define BOOST_MPL_AUX_VOID_SPEC(i, name) /**/
#   define BOOST_MPL_AUX_VOID_SPEC_EXT(i, j, name) /**/
#else
#define BOOST_MPL_AUX_VOID_SPEC(i, name) \
BOOST_MPL_AUX_VOID_SPEC_MAIN(i, name) \
BOOST_MPL_AUX_VOID_SPEC_ARITY(i, name) \
BOOST_MPL_AUX_VOID_SPEC_TEMPLATE_ARITY(i, i, name) \
/**/

#define BOOST_MPL_AUX_VOID_SPEC_EXT(i, j, name) \
BOOST_MPL_AUX_VOID_SPEC_MAIN(i, name) \
BOOST_MPL_AUX_VOID_SPEC_ARITY(i, name) \
BOOST_MPL_AUX_VOID_SPEC_TEMPLATE_ARITY(i, j, name) \
/**/
#endif

#endif // BOOST_MPL_AUX_VOID_SPEC_HPP_INCLUDED
