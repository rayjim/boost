
#ifndef BOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED
#define BOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED

// + file: boost/mpl/aux_/insert_impl.hpp
// + last modified: 05/may/03

// Copyright (c) 2002-03
// David Abrahams, Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/set/aux_/has_key_impl.hpp"
#include "boost/mpl/set/aux_/item.hpp"
#include "boost/mpl/set/aux_/tag.hpp"
#include "boost/mpl/insert_fwd.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/base.hpp"
#include "boost/mpl/apply_if.hpp"

#include "boost/type_traits/is_same.hpp"

namespace boost {
namespace mpl {

template<>
struct insert_traits< aux::set_tag >
{
    template< typename Set, typename T, typename unused_ > struct algorithm
        : apply_if< 
              has_key_impl<aux::set_tag>::apply<Set,T>
            , apply_if< 
                  is_same< T,typename Set::last_masked > 
                , base<Set>
                , identity<Set>
                >
            , identity< s_item<T,Set> >
            >
/*
    : eval< if_<
          has_key<Set,T>
        , if_< is_same< T,typename Set::last_masked >, base< arg<Set> >, Set >
        , Set
        > >
*/
    {
    };
};

}}

#endif // BOOST_MPL_SET_AUX_INSERT_IMPL_HPP_INCLUDED
