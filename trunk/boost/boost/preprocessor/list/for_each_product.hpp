#ifndef BOOST_PREPROCESSOR_LIST_FOR_EACH_PRODUCT_HPP
#define BOOST_PREPROCESSOR_LIST_FOR_EACH_PRODUCT_HPP

/* Copyright (C) 2002
 * Housemarque Oy
 * http://www.housemarque.com
 *
 * Permission to copy, use, modify, sell and distribute this software is
 * granted provided this copyright notice appears in all copies. This
 * software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 * See http://www.boost.org for most recent version.
 */

/** \file

<a href="../../../../boost/preprocessor/list/for_each_product.hpp">Click here to see the header.</a>
*/

#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/for.hpp>

/** THIS IS NOT YET FULLY DOCUMENTED - DO NOT USE!

Repeats F(R,P,X) for each element X of the cartesian product of the lists LL.

This macro is useful for generating code to avoid combinatorial explosion.

<B>LL</B> is a list of lists.

<B>X</B> is an element of the cartesian product of the lists LL.

<H3>Example</H3>
- <a href="../../example/is_integral.cpp">is_integral.cpp</a>
- <a href="../../example/array_arithmetic.c">array_arithmetic.c</a>

<H3>Uses</H3>
- BOOST_PP_FOR()
*/
#define BOOST_PP_LIST_FOR_EACH_PRODUCT(F,P,LL) BOOST_PP_LIST_FOR_EACH_PRODUCT_R(0,F,P,LL)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if !defined(BOOST_NO_COMPILER_CONFIG) && defined(__MWERKS__) && __MWERKS__ <= 0x2406 ||\
  !defined(BOOST_NO_COMPILER_CONFIG) && defined(_MSC_VER)
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_R(R,F,P,LL) BOOST_PP_FOR##R((BOOST_PP_TUPLE_ELEM(3,0,LL),BOOST_PP_TUPLE_ELEM(3,1,LL),(_,_,0),F,P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I0)
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_C(R,P) BOOST_PP_TUPLE_ELEM(3,2,BOOST_PP_TUPLE_ELEM(5,0,P))
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_F(R,P) (BOOST_PP_LIST_REST(BOOST_PP_TUPLE_ELEM(5,0,P)),BOOST_PP_TUPLE_ELEM(5,1,P),BOOST_PP_TUPLE_ELEM(5,2,P),BOOST_PP_TUPLE_ELEM(5,3,P),BOOST_PP_TUPLE_ELEM(5,4,P))

#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I(R,P) BOOST_PP_TUPLE_ELEM(5,3,P)(R,BOOST_PP_TUPLE_ELEM(5,4,P),(BOOST_PP_TUPLE_ELEM(3,0,BOOST_PP_TUPLE_ELEM(5,0,P)),BOOST_PP_TUPLE_ELEM(5,2,P),1))
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,I) BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(3,2,BOOST_PP_TUPLE_ELEM(5,1,P)),BOOST_PP_LIST_FOR_EACH_PRODUCT_I##I##_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_I)
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P) (BOOST_PP_TUPLE_ELEM(3,0,BOOST_PP_TUPLE_ELEM(5,1,P)),BOOST_PP_TUPLE_ELEM(3,1,BOOST_PP_TUPLE_ELEM(5,1,P)),(BOOST_PP_TUPLE_ELEM(3,0,BOOST_PP_TUPLE_ELEM(5,0,P)),BOOST_PP_TUPLE_ELEM(5,2,P),1),BOOST_PP_TUPLE_ELEM(5,3,P),BOOST_PP_TUPLE_ELEM(5,4,P))
#else
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_R(R,F,P,LL) BOOST_PP_FOR##R((BOOST_PP_TUPLE3_ELEM0 LL,BOOST_PP_TUPLE3_ELEM1 LL,(_,_,0),F,P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I0)
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_C(R,P) BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM0 P
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_F(R,P) (BOOST_PP_LIST_REST(BOOST_PP_TUPLE5_ELEM0 P),BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE5_ELEM2 P,BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)

#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I(R,P) BOOST_PP_TUPLE5_ELEM3 P(R,BOOST_PP_TUPLE5_ELEM4 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1))
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,I) BOOST_PP_IF(BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_LIST_FOR_EACH_PRODUCT_I##I##_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_I)
#  define BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P) (BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE3_ELEM1 BOOST_PP_TUPLE5_ELEM1 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1),BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)
#endif
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I0_C(R,P) BOOST_PP_FOR##R(BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I1)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I0(R,P) BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,0)(R,P)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I1_C(R,P) BOOST_PP_FOR##R(BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I2)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I1(R,P) BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,1)(R,P)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I2_C(R,P) BOOST_PP_FOR##R(BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I3)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I2(R,P) BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,2)(R,P)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I3_C(R,P) BOOST_PP_FOR##R(BOOST_PP_LIST_FOR_EACH_PRODUCT_I_H(P),BOOST_PP_LIST_FOR_EACH_PRODUCT_C,BOOST_PP_LIST_FOR_EACH_PRODUCT_F,BOOST_PP_LIST_FOR_EACH_PRODUCT_I4)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I3(R,P) BOOST_PP_LIST_FOR_EACH_PRODUCT_I_C(P,3)(R,P)
#endif
#endif
