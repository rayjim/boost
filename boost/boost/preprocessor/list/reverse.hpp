# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Permission to copy, use, modify, sell and distribute this software is
#  * granted provided this copyright notice appears in all copies. This
#  * software is provided "as is" without express or implied warranty, and
#  * with no claim as to its suitability for any purpose.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_LIST_REVERSE_HPP
# define BOOST_PREPROCESSOR_LIST_REVERSE_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/list/fold_left.hpp>
#
# /* BOOST_PP_LIST_REVERSE */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_LIST_REVERSE(list) BOOST_PP_LIST_FOLD_LEFT(BOOST_PP_LIST_REVERSE_O, BOOST_PP_NIL, list)
# else
#    define BOOST_PP_LIST_REVERSE(list) BOOST_PP_LIST_REVERSE_I(list)
#    define BOOST_PP_LIST_REVERSE_I(list) BOOST_PP_LIST_FOLD_LEFT(BOOST_PP_LIST_REVERSE_O, BOOST_PP_NIL, list)
# endif
#
# define BOOST_PP_LIST_REVERSE_O(d, s, x) (x, s)
#
# /* BOOST_PP_LIST_REVERSE_D */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_LIST_REVERSE_D(d, list) BOOST_PP_LIST_FOLD_LEFT_ ## d(BOOST_PP_LIST_REVERSE_O, BOOST_PP_NIL, list)
# else
#    define BOOST_PP_LIST_REVERSE_D(d, list) BOOST_PP_LIST_REVERSE_D_I(d, list)
#    define BOOST_PP_LIST_REVERSE_D_I(d, list) BOOST_PP_LIST_FOLD_LEFT_ ## d(BOOST_PP_LIST_REVERSE_O, BOOST_PP_NIL, list)
# endif
#
# endif
