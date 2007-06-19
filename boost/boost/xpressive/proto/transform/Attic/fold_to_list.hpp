///////////////////////////////////////////////////////////////////////////////
/// \file fold_to_list.hpp
/// A higher-level transform that uses the fold, branch and list transforms
/// to fold a binary tree into a fusion cons-list.
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_FOLD_TO_LIST_HPP_EAN_12_17_2006
#define BOOST_PROTO_TRANSFORM_FOLD_TO_LIST_HPP_EAN_12_17_2006

#include <boost/xpressive/proto/detail/prefix.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/xpressive/proto/transform/fold_tree.hpp>
#include <boost/xpressive/proto/transform/list.hpp>
#include <boost/xpressive/proto/detail/suffix.hpp>

namespace boost { namespace proto { namespace transform
{

    /// fold_to_list
    /// TODO Find a cleaner interface
    template<typename Grammar>
    struct fold_to_list
      : fold_tree<
            typename Grammar::proto_tag
          , transform::list<typename Grammar::proto_arg0>
          , fusion::nil
        >
    {
        BOOST_MPL_ASSERT((
            is_same<
                typename Grammar::proto_arg0
              , typename Grammar::proto_arg1
            >
        ));
    };

    /// reverse_fold_to_list
    /// TODO Find a cleaner interface
    template<typename Grammar>
    struct reverse_fold_to_list
      : reverse_fold_tree<
            typename Grammar::proto_tag
          , transform::list<typename Grammar::proto_arg0>
          , fusion::nil
        >
    {
        BOOST_MPL_ASSERT((
            is_same<
                typename Grammar::proto_arg0
              , typename Grammar::proto_arg1
            >
        ));
    };

}}}

#endif
