//-----------------------------------------------------------------------------
// boost mpl/is_sequence.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_IS_SEQUENCE_HPP_INCLUDED
#define BOOST_MPL_IS_SEQUENCE_HPP_INCLUDED

#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_tag_fwd.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"
#include "boost/mpl/aux_/config/eti.hpp"

#include "boost/type_traits/is_same.hpp"

namespace boost { namespace mpl {

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T)
    >
struct is_sequence
    : logical_not< is_same< typename begin<T>::type, non_sequence_tag > >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_sequence,(T))
};

#if defined(BOOST_MPL_MSVC_ETI_BUG)
template<> struct is_sequence<int>
    : bool_c<false>
{
};
#endif

BOOST_MPL_AUX_VOID_SPEC(1, is_sequence)

}} // namespace boost::mpl

#endif // BOOST_MPL_IS_SEQUENCE_HPP_INCLUDED
