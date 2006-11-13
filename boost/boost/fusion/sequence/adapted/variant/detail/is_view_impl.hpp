/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_VARIANT_IS_VIEW_IMPL_13112006_0749)
#define BOOST_FUSION_VARIANT_IS_VIEW_IMPL_13112006_0749

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion {

    struct variant_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<variant_tag>
        {
            template<typename Sequence>
            struct apply : mpl::false_ {};
        };
    }
}}

#endif
