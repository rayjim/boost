//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for config setup
// This file should compile, if it does not then
// one or more macros need to be defined.
// see boost_*.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_*.cxx on
// Wed Dec 10 13:38:42 GMTST 2003

#include <boost/config.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include "test.hpp"

#ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#include "boost_no_arg_dep_lookup.cxx"
#else
namespace boost_no_argument_dependent_lookup = empty_boost;
#endif
#ifndef BOOST_NO_AUTO_PTR
#include "boost_no_auto_ptr.cxx"
#else
namespace boost_no_auto_ptr = empty_boost;
#endif
#ifndef BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#include "boost_no_bcb_partial_spec.cxx"
#else
namespace boost_bcb_partial_specialization_bug = empty_boost;
#endif
#ifndef BOOST_NO_CV_SPECIALIZATIONS
#include "boost_no_cv_spec.cxx"
#else
namespace boost_no_cv_specializations = empty_boost;
#endif
#ifndef BOOST_NO_CWCHAR
#include "boost_no_cwchar.cxx"
#else
namespace boost_no_cwchar = empty_boost;
#endif
#ifndef BOOST_DEDUCED_TYPENAME
#include "boost_no_ded_typename.cxx"
#else
namespace boost_deduced_typename = empty_boost;
#endif
#ifndef BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#include "boost_no_dep_val_param.cxx"
#else
namespace boost_no_dependent_types_in_template_value_parameters = empty_boost;
#endif
#ifndef BOOST_NO_EXCEPTIONS
#include "boost_no_exceptions.cxx"
#else
namespace boost_no_exceptions = empty_boost;
#endif
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include "boost_no_func_tmp_order.cxx"
#else
namespace boost_no_function_template_ordering = empty_boost;
#endif
#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#include "boost_no_inline_memb_init.cxx"
#else
namespace boost_no_inclass_member_initialization = empty_boost;
#endif
#ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#include "boost_no_iter_construct.cxx"
#else
namespace boost_no_templated_iterator_constructors = empty_boost;
#endif
#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#include "boost_no_limits_const_exp.cxx"
#else
namespace boost_no_limits_compile_time_constants = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#include "boost_no_mem_func_spec.cxx"
#else
namespace boost_no_member_function_specializations = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#include "boost_no_mem_templ_frnds.cxx"
#else
namespace boost_no_member_template_friends = empty_boost;
#endif
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
#include "boost_no_ops_in_namespace.cxx"
#else
namespace boost_no_operators_in_namespace = empty_boost;
#endif
#ifndef BOOST_NO_PRIVATE_IN_AGGREGATE
#include "boost_no_priv_aggregate.cxx"
#else
namespace boost_no_private_in_aggregate = empty_boost;
#endif
#ifndef BOOST_NO_POINTER_TO_MEMBER_CONST
#include "boost_no_ptr_mem_const.cxx"
#else
namespace boost_no_pointer_to_member_const = empty_boost;
#endif
#ifndef BOOST_NO_STRINGSTREAM
#include "boost_no_sstream.cxx"
#else
namespace boost_no_stringstream = empty_boost;
#endif
#ifndef BOOST_NO_STD_ALLOCATOR
#include "boost_no_std_allocator.cxx"
#else
namespace boost_no_std_allocator = empty_boost;
#endif
#ifndef BOOST_NO_STD_ITERATOR
#include "boost_no_std_iterator.cxx"
#else
namespace boost_no_std_iterator = empty_boost;
#endif
#ifndef BOOST_NO_STD_MIN_MAX
#include "boost_no_std_min_max.cxx"
#else
namespace boost_no_std_min_max = empty_boost;
#endif
#ifndef BOOST_NO_STD_WSTREAMBUF
#include "boost_no_std_wstreambuf.cxx"
#else
namespace boost_no_std_wstreambuf = empty_boost;
#endif
#ifndef BOOST_NO_SWPRINTF
#include "boost_no_swprintf.cxx"
#else
namespace boost_no_swprintf = empty_boost;
#endif
#ifndef BOOST_NO_TEMPLATE_TEMPLATES
#include "boost_no_template_template.cxx"
#else
namespace boost_no_template_templates = empty_boost;
#endif
#ifndef BOOST_NO_USING_TEMPLATE
#include "boost_no_using_template.cxx"
#else
namespace boost_no_using_template = empty_boost;
#endif
#ifndef BOOST_NO_VOID_RETURNS
#include "boost_no_void_returns.cxx"
#else
namespace boost_no_void_returns = empty_boost;
#endif
#ifdef BOOST_HAS_GETTIMEOFDAY
#include "boost_has_gettimeofday.cxx"
#else
namespace boost_has_gettimeofday = empty_boost;
#endif
#ifdef BOOST_HAS_MACRO_USE_FACET
#include "boost_has_macro_use_facet.cxx"
#else
namespace boost_has_macro_use_facet = empty_boost;
#endif
#ifdef BOOST_HAS_NRVO
#include "boost_has_nrvo.cxx"
#else
namespace boost_has_nrvo = empty_boost;
#endif
#ifdef BOOST_HAS_PTHREAD_YIELD
#include "boost_has_pthread_yield.cxx"
#else
namespace boost_has_pthread_yield = empty_boost;
#endif
#ifdef BOOST_HAS_SIGACTION
#include "boost_has_sigaction.cxx"
#else
namespace boost_has_sigaction = empty_boost;
#endif
#ifdef BOOST_HAS_UNISTD_H
#include "boost_has_unistd_h.cxx"
#else
namespace boost_has_unistd_h = empty_boost;
#endif
#ifdef BOOST_HAS_WINTHREADS
#include "boost_has_winthreads.cxx"
#else
namespace boost_has_winthreads = empty_boost;
#endif

int test_main( int, char *[] )
{
   BOOST_TEST(0 == boost_no_intrinsic_wchar_t::test());
   BOOST_TEST(0 == boost_no_void_returns::test());
   BOOST_TEST(0 == boost_no_using_template::test());
   BOOST_TEST(0 == boost_no_using_declaration_overloads_from_typename_base::test());
   BOOST_TEST(0 == boost_function_scope_using_declaration_breaks_adl::test());
   BOOST_TEST(0 == boost_no_template_templates::test());
   BOOST_TEST(0 == boost_no_swprintf::test());
   BOOST_TEST(0 == boost_no_stdc_namespace::test());
   BOOST_TEST(0 == boost_no_std_wstring::test());
   BOOST_TEST(0 == boost_no_std_wstreambuf::test());
   BOOST_TEST(0 == boost_no_std_use_facet::test());
   BOOST_TEST(0 == boost_no_std_output_iterator_assign::test());
   BOOST_TEST(0 == boost_no_std_min_max::test());
   BOOST_TEST(0 == boost_no_std_messages::test());
   BOOST_TEST(0 == boost_no_std_locale::test());
   BOOST_TEST(0 == boost_no_std_iterator::test());
   BOOST_TEST(0 == boost_no_std_iterator_traits::test());
   BOOST_TEST(0 == boost_no_std_distance::test());
   BOOST_TEST(0 == boost_no_std_allocator::test());
   BOOST_TEST(0 == boost_no_stringstream::test());
   BOOST_TEST(0 == boost_no_sfinae::test());
   BOOST_TEST(0 == boost_no_unreachable_return_detection::test());
   BOOST_TEST(0 == boost_no_pointer_to_member_const::test());
   BOOST_TEST(0 == boost_no_private_in_aggregate::test());
   BOOST_TEST(0 == boost_no_template_partial_specialization::test());
   BOOST_TEST(0 == boost_no_operators_in_namespace::test());
   BOOST_TEST(0 == boost_no_member_templates::test());
   BOOST_TEST(0 == boost_no_member_template_friends::test());
   BOOST_TEST(0 == boost_no_member_template_keyword::test());
   BOOST_TEST(0 == boost_no_member_function_specializations::test());
   BOOST_TEST(0 == boost_no_long_long_numeric_limits::test());
   BOOST_TEST(0 == boost_no_limits_compile_time_constants::test());
   BOOST_TEST(0 == boost_no_limits::test());
   BOOST_TEST(0 == boost_no_templated_iterator_constructors::test());
   BOOST_TEST(0 == boost_no_integral_int64_t::test());
   BOOST_TEST(0 == boost_no_inclass_member_initialization::test());
   BOOST_TEST(0 == boost_no_ms_int64_numeric_limits::test());
   BOOST_TEST(0 == boost_no_function_template_ordering::test());
   BOOST_TEST(0 == boost_no_explicit_function_template_arguments::test());
   BOOST_TEST(0 == boost_no_exceptions::test());
   BOOST_TEST(0 == boost_no_exception_std_namespace::test());
   BOOST_TEST(0 == boost_no_dependent_types_in_template_value_parameters::test());
   BOOST_TEST(0 == boost_no_dependent_nested_derivations::test());
   BOOST_TEST(0 == boost_deduced_typename::test());
   BOOST_TEST(0 == boost_no_cwctype::test());
   BOOST_TEST(0 == boost_no_cwchar::test());
   BOOST_TEST(0 == boost_no_cv_void_specializations::test());
   BOOST_TEST(0 == boost_no_cv_specializations::test());
   BOOST_TEST(0 == boost_no_ctype_functions::test());
   BOOST_TEST(0 == boost_bcb_partial_specialization_bug::test());
   BOOST_TEST(0 == boost_no_auto_ptr::test());
   BOOST_TEST(0 == boost_no_argument_dependent_lookup::test());
   BOOST_TEST(0 == boost_has_winthreads::test());
   BOOST_TEST(0 == boost_msvc_std_iterator::test());
   BOOST_TEST(0 == boost_msvc6_member_templates::test());
   BOOST_TEST(0 == boost_has_unistd_h::test());
   BOOST_TEST(0 == boost_has_stlp_use_facet::test());
   BOOST_TEST(0 == boost_has_stdint_h::test());
   BOOST_TEST(0 == boost_has_slist::test());
   BOOST_TEST(0 == boost_has_sigaction::test());
   BOOST_TEST(0 == boost_has_sgi_type_traits::test());
   BOOST_TEST(0 == boost_has_sched_yield::test());
   BOOST_TEST(0 == boost_has_pthreads::test());
   BOOST_TEST(0 == boost_has_pthread_yield::test());
   BOOST_TEST(0 == boost_has_pthread_mutexattr_settype::test());
   BOOST_TEST(0 == boost_has_pthread_delay_np::test());
   BOOST_TEST(0 == boost_has_partial_std_allocator::test());
   BOOST_TEST(0 == boost_has_nrvo::test());
   BOOST_TEST(0 == boost_has_nl_types_h::test());
   BOOST_TEST(0 == boost_has_nanosleep::test());
   BOOST_TEST(0 == boost_has_ms_int64::test());
   BOOST_TEST(0 == boost_has_macro_use_facet::test());
   BOOST_TEST(0 == boost_has_long_long::test());
   BOOST_TEST(0 == boost_has_hash::test());
   BOOST_TEST(0 == boost_has_gettimeofday::test());
   BOOST_TEST(0 == boost_has_ftime::test());
   BOOST_TEST(0 == boost_has_dirent_h::test());
   BOOST_TEST(0 == boost_has_clock_gettime::test());
   BOOST_TEST(0 == boost_has_bethreads::test());
   BOOST_TEST(0 == boost_has_two_arg_use_facet::test());
   return 0;
}
