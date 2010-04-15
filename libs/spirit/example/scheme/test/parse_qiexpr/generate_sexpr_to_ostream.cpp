//  Copyright (c) 2001-2010 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <output/generate_sexpr.hpp>
#include <output/generate_sexpr_impl.hpp>

#include <fstream>

///////////////////////////////////////////////////////////////////////////////
// explicit template instantiation for the function parse_sexpr 
namespace scheme { namespace output
{
    template bool generate_sexpr(BOOST_TYPEOF(std::cout)&, utree const& result);
}}

///////////////////////////////////////////////////////////////////////////////
// this is needed if grammar debugging is on
#if defined(BOOST_SPIRIT_DEBUG)
namespace boost { namespace spirit { namespace traits
{
    void print_attribute(std::ostream& out, scheme::utree const& val)
    {
        scheme::output::generate_sexpr(out, val);
    }
}}}
#endif
