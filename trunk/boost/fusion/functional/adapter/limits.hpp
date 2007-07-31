/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_LIMITS_HPP_INCLUDED)
#   define BOOST_FUSION_FUNCTIONAL_ADAPTER_LIMITS_HPP_INCLUDED

#   include <boost/fusion/sequence/container/vector/limits.hpp>

#   if !defined(BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY)
#       define BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif
#   if !defined(BOOST_FUSION_UNFUSED_RVALUE_ARGS_MAX_ARITY)
#       define BOOST_FUSION_UNFUSED_RVALUE_ARGS_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_RVALUE_ARGS_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_RVALUE_ARGS_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif
#   if !defined(BOOST_FUSION_UNFUSED_LVALUE_ARGS_MAX_ARITY)
#       define BOOST_FUSION_UNFUSED_LVALUE_ARGS_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_LVALUE_ARGS_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_LVALUE_ARGS_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif
#   if !defined(BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY)
#       define BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif
#   if !defined(BOOST_FUSION_CONSTRUCTOR_MAX_ARITY)
#       define BOOST_FUSION_CONSTRUCTOR_MAX_ARITY 6
#   endif

#endif

