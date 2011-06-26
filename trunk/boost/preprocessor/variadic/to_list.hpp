# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_VARIADIC_TO_LIST_HPP
# define BOOST_PREPROCESSOR_VARIADIC_TO_LIST_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/tuple/to_list.hpp>
#
# /* BOOST_PP_VARIADIC_TO_LIST */
#
# if BOOST_PP_VARIADICS
#    define BOOST_PP_VARIADIC_TO_LIST(...) BOOST_PP_TUPLE_TO_LIST((__VA_ARGS__))
# endif
#
# endif
