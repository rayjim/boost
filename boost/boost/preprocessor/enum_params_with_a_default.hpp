#ifndef BOOST_PREPROCESSOR_ENUM_PARAMS_WITH_A_DEFAULT_HPP
#define BOOST_PREPROCESSOR_ENUM_PARAMS_WITH_A_DEFAULT_HPP

/* Copyright (C) 2001
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

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>

/** <P>Generates a comma separated list of parameters with a default.</P>

<P>In other words, expands to the sequence:</P>

<PRE>
  P##0 = D, P##1 = D, ..., P##N-1 = D
</PRE>

<H3>Uses</H3>
<UL>
  <LI>BOOST_PP_REPEAT()
</UL>
*/
#define BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(N,P,D) BOOST_PP_ENUM(N,BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT_F,(P,D))

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#if !defined(BOOST_NO_COMPILER_CONFIG) && defined(__MWERKS__) && __MWERKS__ <= 0x2406
#  define BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT_F(I,PD) BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2,0,PD),I)=BOOST_PP_TUPLE_ELEM(2,1,PD)
#else
#  define BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT_F(I,PD) BOOST_PP_CAT(BOOST_PP_TUPLE2_ELEM0 PD,I)=BOOST_PP_TUPLE2_ELEM1 PD
#endif
#endif

/** <P>Obsolete. Use BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT().</P> */
#define BOOST_PREPROCESSOR_ENUM_PARAMS_WITH_A_DEFAULT(N,P,D) BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(N,P,D)
#endif
