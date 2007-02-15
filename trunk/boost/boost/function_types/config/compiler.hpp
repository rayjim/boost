
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_CONFIG_COMPILER_HPP_INCLUDED
#define BOOST_FT_CONFIG_COMPILER_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if defined(BOOST_MSVC)

#   if BOOST_MSVC < 1310
#     error "unsupported compiler version"
#   endif

    // enable clrcall calling covention (call to .NET managed code) when
    // compiling with /clr 
#   if BOOST_MSVC >= 1400 && defined(__cplusplus_cli)
#     ifndef BOOST_FT_CC_CLRCALL
#     define BOOST_FT_CC_CLRCALL callable_builtin
#     endif
#   endif

    // Intel x86 architecture specific calling conventions
#   ifdef _M_IX86
#     define BOOST_FT_COMMON_X86_CCs callable_builtin
#     if BOOST_MSVC < 1400
        // version 7.1 is missing a keyword to specify the thiscall cc ...
#       ifndef BOOST_FT_CC_IMPLICIT_THISCALL
#       define BOOST_FT_CC_IMPLICIT_THISCALL non_variadic|member|callable_builtin
#       ifndef BOOST_FT_CONFIGURATION_OK
#         pragma message("ATTENTION: /Gd /Gr /Gz compiler options will not")
#         pragma message("ATTENTION: work with this configuration.")
          // TODO: add documentation reference
#       endif
#       endif
#     else 
        // ...introduced in version 8
#       ifndef BOOST_FT_CC_THISCALL
#       define BOOST_FT_CC_THISCALL non_variadic|member|callable_builtin
#       endif
#     endif
#   endif

#elif defined(__GNUC__) && !defined(BOOST_INTEL_LINUX)

#   if __GNUC__ < 3
#     error "unsupported compiler version"
#   endif

#   if defined(__i386__)
#     // see http://gcc.gnu.org/bugzilla/show_bug.cgi?id=20439
#     // see http://gcc.gnu.org/bugzilla/show_bug.cgi?id=29328
#     if BOOST_WORKAROUND(__GNUC__,BOOST_TESTED_AT(4))
#       ifndef BOOST_FT_CC_IMPLICIT 
#       define BOOST_FT_CC_IMPLICIT member|callable_builtin
#       endif
#       define BOOST_FT_COMMON_X86_CCs non_member|callable_builtin
#     else
#       define BOOST_FT_COMMON_X86_CCs callable_builtin
#     endif
#   else
#     ifndef BOOST_FT_CC_IMPLICIT
#     define BOOST_FT_CC_IMPLICIT callable_builtin
#     endif
#   endif

#   // TODO: straighten out this hack
#   ifndef __cdecl
#   define __cdecl __attribute__((__cdecl__))
#   endif
#   ifndef __stdcall
#   define __stdcall __attribute__((__stdcall__))
#   endif
#   ifndef __fastcall
#   define __fastcall __attribute__((__fastcall__))
#   endif

#elif defined(__BORLANDC__)

#   if __BORLANDC__ < 0x550
#     error "unsupported compiler version"
#   elif __BORLANDC__ > 0x565
#     pragma message("WARNING: library untested with this compiler version")
#   endif

#   define BOOST_FT_COMMON_X86_CCs callable_builtin

    // syntactic specialities of cc specifier
#   define BOOST_FT_SYNTAX(result,lparen,cc_spec,type_mod,name,rparen) \
                        result() cc_spec() lparen() type_mod() name() rparen()
#else
    // only enable default calling convention
#   define BOOST_FT_CC_IMPLICIT callable_builtin
#endif


#endif

