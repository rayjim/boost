/*
 *
 * Copyright (c) 1998-2002
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 */
 
 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         user.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: User settable options.
  */

// define if you want the regex library to use the C locale
// even on Win32:
// #define BOOST_REGEX_USE_C_LOCALE

// define this is you want the regex library to use the C++
// locale:
// #define BOOST_REGEX_USE_CPP_LOCALE

// define this if the runtime library is a dll, and you
// want BOOST_REGEX_DYN_LINK to set up dll exports/imports
// with __declspec(dllexport)/__declspec(dllimport.)
// #define BOOST_REGEX_HAS_DLL_RUNTIME

// define this if you want to dynamically link to regex,
// if the runtime library is also a dll (Probably Win32 specific,
// and has no effect unless BOOST_REGEX_HAS_DLL_RUNTIME is set):
// #define BOOST_REGEX_DYN_LINK

// define this if you don't want the lib to automatically
// select its link libraries:
// #define BOOST_REGEX_NO_LIB

// define this if templates with switch statements cause problems:
// #define BOOST_REGEX_NO_TEMPLATE_SWITCH_MERGE
 
// define this to disable Win32 support when available:
// #define BOOST_REGEX_NO_W32

// define this if bool is not a real type:
// #define BOOST_REGEX_NO_BOOL

// define this if no template instances are to be placed in
// the library rather than users object files:
// #define BOOST_REGEX_NO_EXTERNAL_TEMPLATES

// define this if the forward declarations in regex_fwd.hpp
// cause more problems than they are worth:
// #define BOOST_REGEX_NO_FWD

// define this if your compiler can't cope with the new 
// version 4 regex code.
// #define BOOST_REGEX_V3

// define this if your compiler supports MS Windows structured
// exception handling.
// #define BOOST_REGEX_HAS_MS_STACK_GUARD

// define this if you want to use the recursive algorithm
// even if BOOST_REGEX_HAS_MS_STACK_GUARD is not defined.
// #define BOOST_REGEX_RECURSIVE

// define this if you want to use the non-recursive
// algorithm, even if the recursive version would be the default.
// #define BOOST_REGEX_NON_RECURSIVE

// define this if you want to set the size of the memory blocks
// used by the non-recursive algorithm.
// #define BOOST_REGEX_BLOCKSIZE 4096

// define this if you want to set the maximum number of memory blocks
// used by the non-recursive algorithm.
// #define BOOST_REGEX_MAX_BLOCKS 1024

// define this if you want to set the maximum number of memory blocks
// cached by the non-recursive algorithm: Normally this is 16, but can be 
// higher if you have multiple threads all using boost.regex, or lower 
// if you don't want boost.regex to cache memory.
// #define BOOST_REGEX_MAX_CACHE_BLOCKS 16
