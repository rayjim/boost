//  Copyright (c) 2001-2010 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SCHEME_OUTPUT_GENERATE_SEXPR_MAR_29_2010_1210PM)
#define SCHEME_OUTPUT_GENERATE_SEXPR_MAR_29_2010_1210PM

#include <utree/utree.hpp>
#include <output/sexpr.hpp>

namespace scheme { namespace output
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Char>
    bool generate_sexpr(std::basic_ostream<Char>& os, utree const& tree);

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char>
    bool generate_sexpr_list(std::basic_ostream<Char>& os, utree const& tree);

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char>
    bool generate_sexpr(std::basic_string<Char>& os, utree const& tree);

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char>
    bool generate_sexpr_list(std::basic_string<Char>& os, utree const& tree);
}}

#endif


