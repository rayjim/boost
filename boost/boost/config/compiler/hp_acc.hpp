//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  HP aCC C++ compiler setup:

#if (__HP_aCC <= 33100)
#    define BOOST_NO_INTEGRAL_INT64_T
#    define BOOST_NO_OPERATORS_IN_NAMESPACE
#  if !defined(_NAMESPACE_STD)
#     define BOOST_NO_STD_LOCALE
#     define BOOST_NO_STRINGSTREAM
#  endif
#endif

#if (__HP_aCC <= 33300)
// member templates are sufficiently broken that we disable them for now
#    define BOOST_NO_MEMBER_TEMPLATES
#    define BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#    define BOOST_NO_USING_DECLARATION_OVERLOADS_FROM_TYPENAME_BASE
#endif

#if (__HP_aCC <= 33900) || !defined(BOOST_STRICT_CONFIG)
#    define BOOST_NO_UNREACHABLE_RETURN_DETECTION
#    define BOOST_NO_TEMPLATE_TEMPLATES
#    define BOOST_NO_SWPRINTF
#    define BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
//     std lib config should set this one already:
//#    define BOOST_NO_STD_ALLOCATOR
#endif 

// optional features rather than defects:
#if (__HP_aCC >= 33900)
#    define BOOST_HAS_LONG_LONG
#    define BOOST_HAS_PARTIAL_STD_ALLOCATOR
#endif

#if (__HP_aCC <= 53800 )
#    define BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#endif

#define BOOST_COMPILER "HP aCC version " BOOST_STRINGIZE(__HP_aCC)

//
// versions check:
// we don't support HP aCC prior to version 0:
#if __HP_aCC < 33000
#  error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 0:
#if (__HP_aCC > 53800)
#  if defined(BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif



