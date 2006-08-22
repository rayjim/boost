/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_IS_SEQUENCE_IMPL_20060228_1946)
#define BOOST_FUSION_IS_SEQUENCE_IMPL_20060228_1946

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct example_sequence_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_sequence_impl;

        template<>
        struct is_sequence_impl<example_sequence_tag>
        {
            template<typename T>
            struct apply : mpl::true_ {};
        };
    }
}}

#endif
