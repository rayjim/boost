// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

#ifndef BOOST_FUNCTION10_HPP
#define BOOST_FUNCTION10_HPP

#include <boost/function/function_base.hpp>

#define BOOST_FUNCTION_NUM_ARGS 10
#define BOOST_FUNCTION_COMMA ,
#define BOOST_FUNCTION_TEMPLATE_PARMS typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
#define BOOST_FUNCTION_TEMPLATE_ARGS T1, T2, T3, T4, T5, T6, T7, T8, T9, T10
#define BOOST_FUNCTION_OTHER_TEMPLATE_PARMS typename OtherT1, typename OtherT2, typename OtherT3, typename OtherT4, typename OtherT5, typename OtherT6, typename OtherT7, typename OtherT8, typename OtherT9, typename OtherT10
#define BOOST_FUNCTION_OTHER_TEMPLATE_ARGS OtherT1, OtherT2, OtherT3, OtherT4, OtherT5, OtherT6, OtherT7, OtherT8, OtherT9, OtherT10
#define BOOST_FUNCTION_PARMS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10
#define BOOST_FUNCTION_ARGS a1, a2, a3, a4, a5, a6, a7, a8, a9, a10
#define BOOST_FUNCTION_FUNCTION function10
#define BOOST_FUNCTION_INVOKER_BASE invoker_base10
#define BOOST_FUNCTION_FUNCTION_INVOKER function_invoker10
#define BOOST_FUNCTION_VOID_FUNCTION_INVOKER void_function_invoker10
#define BOOST_FUNCTION_FUNCTION_OBJ_INVOKER function_obj_invoker10
#define BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER void_function_obj_invoker10
#define BOOST_FUNCTION_GET_FUNCTION_INVOKER get_function_invoker10
#define BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER get_function_obj_invoker10

#include <boost/function/function_template.hpp>

#undef BOOST_FUNCTION_NUM_ARGS
#undef BOOST_FUNCTION_COMMA
#undef BOOST_FUNCTION_TEMPLATE_PARMS
#undef BOOST_FUNCTION_TEMPLATE_ARGS
#undef BOOST_FUNCTION_OTHER_TEMPLATE_PARMS
#undef BOOST_FUNCTION_OTHER_TEMPLATE_ARGS
#undef BOOST_FUNCTION_PARMS
#undef BOOST_FUNCTION_ARGS
#undef BOOST_FUNCTION_FUNCTION
#undef BOOST_FUNCTION_INVOKER_BASE
#undef BOOST_FUNCTION_FUNCTION_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#undef BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER

#endif // BOOST_FUNCTION10_HPP
