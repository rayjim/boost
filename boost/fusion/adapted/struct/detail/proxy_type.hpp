/*=============================================================================
    Copyright (c) 2009-2010 Hartmut Kaiser
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STRUCT_DETAIL_PROXY_TYPE_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_DETAIL_PROXY_TYPE_HPP

#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#define BOOST_FUSION_ADAPT_STRUCT_PROXY_DUMMY4(A,B,C,D)
#define BOOST_FUSION_ADAPT_STRUCT_PROXY_TYPE_NS_BEGIN(R,DATA,ELEM)              \
    namespace ELEM {
#define BOOST_FUSION_ADAPT_STRUCT_PROXY_TYPE_NS_END(Z,I,DATA) }
#define BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION_I(Z,I,ELEM) ELEM::

#define BOOST_FUSION_ADAPT_STRUCT_DEFINE_PROXY_TYPE(                            \
    WRAPPED_TYPE,NAMESPACE_SEQ,NAME)                                            \
                                                                                \
    BOOST_PP_IF(                                                                \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        BOOST_PP_SEQ_FOR_EACH_R,                                                \
        BOOST_FUSION_ADAPT_STRUCT_PROXY_DUMMY4)(                                \
            1,                                                                  \
            BOOST_FUSION_ADAPT_STRUCT_PROXY_TYPE_NS_BEGIN,                      \
            _,                                                                  \
            BOOST_PP_SEQ_TAIL(NAMESPACE_SEQ))                                   \
                                                                                \
    struct NAME                                                                 \
    {                                                                           \
        NAME(WRAPPED_TYPE& obj)                                                 \
          : obj(obj)                                                            \
        {}                                                                      \
                                                                                \
        WRAPPED_TYPE& obj;                                                      \
    };                                                                          \
                                                                                \
    BOOST_PP_REPEAT_1(                                                          \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        BOOST_FUSION_ADAPT_STRUCT_PROXY_TYPE_NS_END,                            \
        _)

#define BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION(NAMESPACE_SEQ)          \
    BOOST_PP_IF(                                                                \
        BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(NAMESPACE_SEQ)),                         \
        BOOST_PP_SEQ_FOR_EACH_R,                                                \
        BOOST_FUSION_ADAPT_STRUCT_PROXY_DUMMY4)(                                \
            1,                                                                  \
            BOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DECLARATION_I,                  \
            _,                                                                  \
            BOOST_PP_SEQ_TAIL(NAMESPACE_SEQ))

#endif
