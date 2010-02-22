/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_CLASS_DEREF_DATA_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_CLASS_DEREF_DATA_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_data_impl;

    template <>
    struct deref_data_impl<assoc_class_iterator_tag>
      : deref_impl<assoc_class_iterator_tag>
    {};
}}}

#endif
