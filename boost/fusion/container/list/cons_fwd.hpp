/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_CONS_FWD_HPP_INCLUDED)
#define BOOST_FUSION_CONS_FWD_HPP_INCLUDED

namespace boost { namespace fusion
{
    struct nil;

    template <typename Car, typename Cdr = nil>
    struct cons;
}}

#endif

