
//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(TEST_MF_HAS_TYPE_CHECK_TYPEDEF_HPP)
#define TEST_MF_HAS_TYPE_CHECK_TYPEDEF_HPP

#include "test_structs.hpp"
#include <boost/tti/type.hpp>
#include <boost/tti/mem_type.hpp>

BOOST_TTI_HAS_TYPE(AnIntType)
BOOST_TTI_TRAIT_HAS_TYPE(NameStruct,AStructType)
BOOST_TTI_HAS_TYPE(AnIntTypeReference)
BOOST_TTI_HAS_TYPE(BType)
BOOST_TTI_TRAIT_HAS_TYPE(TheInteger,AnIntegerType)
BOOST_TTI_HAS_TYPE(CType)
BOOST_TTI_HAS_TYPE(AnotherIntegerType)
BOOST_TTI_TRAIT_HAS_TYPE(SomethingElse,someOtherType)
BOOST_TTI_HAS_TYPE(NoOtherType)

BOOST_TTI_MTFC_HAS_TYPE(AnIntType)
BOOST_TTI_MTFC_TRAIT_HAS_TYPE(MFunctionNameStruct,AStructType)
BOOST_TTI_MTFC_HAS_TYPE(AnIntTypeReference)
BOOST_TTI_MTFC_HAS_TYPE(BType)
BOOST_TTI_MTFC_TRAIT_HAS_TYPE(MFCTheInteger,AnIntegerType)
BOOST_TTI_MTFC_HAS_TYPE(CType)
BOOST_TTI_MTFC_HAS_TYPE(AnotherIntegerType)
BOOST_TTI_MTFC_TRAIT_HAS_TYPE(FClassSomethingElse,someOtherType)
BOOST_TTI_MTFC_HAS_TYPE(NoOtherType)

BOOST_TTI_MEMBER_TYPE(AStructType)
BOOST_TTI_MEMBER_TYPE(BType)
BOOST_TTI_MEMBER_TYPE(CType)
BOOST_TTI_MEMBER_TYPE(AnIntType)

BOOST_TTI_MTFC_MEMBER_TYPE(CType)

#endif // TEST_MF_HAS_TYPE_CHECK_TYPEDEF_HPP
