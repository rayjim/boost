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
# ifndef BOOST_PREPROCESSOR_LOGICAL_NOR_HPP
# define BOOST_PREPROCESSOR_LOGICAL_NOR_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/logical/bitnor.hpp>
# include <boost/preprocessor/logical/bool.hpp>
#
# /* BOOST_PP_NOR */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_NOR BOOST_PP_NOR_I
# else
#    define BOOST_PP_NOR(p, q) BOOST_PP_NOR_I(p, q)
# endif
#
# define BOOST_PP_NOR_I(p, q) BOOST_PP_BITNOR(BOOST_PP_BOOL(p), BOOST_PP_BOOL(q))
#
# endif
