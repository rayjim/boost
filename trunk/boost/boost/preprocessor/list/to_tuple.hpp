#ifndef BOOST_PREPROCESSOR_LIST_TO_TUPLE_HPP
#define BOOST_PREPROCESSOR_LIST_TO_TUPLE_HPP

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

#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/list/enum.hpp>

/** <P>Converts the list to a tuple.</P>

<P>For example,</P>

<PRE>
  BOOST_PP_LIST_TO_TUPLE(BOOST_PP_TUPLE_TO_LIST(3,(A,B,C)))
</PRE>

<P>expands to (A,B,C).</P>

<H3>Note</H3>
<UL>
  <LI>The supported size of the list being converted to a tuple is limited by
      BOOST_PP_LIMIT_MAG rather than BOOST_PP_LIMIT_TUPLE.</P>
</UL>

<H3>Uses</H3>
<UL>
  <LI>BOOST_PP_FOR()
</UL>
*/
#define BOOST_PP_LIST_TO_TUPLE(L) BOOST_PP_LIST_TO_TUPLE_R(0,L)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define BOOST_PP_LIST_TO_TUPLE_R(D,L) (BOOST_PP_LIST_ENUM_R(D,L))
#endif
#endif
