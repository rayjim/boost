//-----------------------------------------------------------------------------
// boost mpl/vector/aux_/begin_end.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED

#include "boost/mpl/aux_/config/vector.hpp"

#if defined(BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL)

#   include "boost/mpl/begin_end_fwd.hpp"
#   include "boost/mpl/integral_c.hpp"
#   include "boost/mpl/vector/aux_/iterator.hpp"
#   include "boost/mpl/vector/aux_/tag.hpp"

namespace boost {
namespace mpl {

template<>
struct begin_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
    {
        typedef vector_iterator<
              Vector
            , integral_c<long,0>
            > type;
    };
};

template<>
struct end_traits< aux::vector_tag >
{
    template< typename Vector > struct algorithm
    {
        typedef vector_iterator<
              Vector
            , typename Vector::size
            > type;
    };
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_TYPEOF_BASED_VECTOR_IMPL

#endif // BOOST_MPL_VECTOR_AUX_BEGIN_END_HPP_INCLUDED
