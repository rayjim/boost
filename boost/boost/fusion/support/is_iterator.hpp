/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_IS_ITERATOR_05062005_1219)
#define FUSION_IS_ITERATOR_05062005_1219

#include <boost/type_traits/is_base_and_derived.hpp>

namespace boost { namespace fusion
{
    struct iterator_root;

    template <typename T>
    struct is_fusion_iterator : is_base_and_derived<iterator_root, T> {};
}}

#endif
