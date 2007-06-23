/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2007
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_ASSERT_HPP
#define BOOST_INTRUSIVE_DETAIL_ASSERT_HPP 

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#if !defined(BOOST_INTRUSIVE_INVARIANT_ASSERT)
#include <boost/assert.hpp>
#define BOOST_INTRUSIVE_INVARIANT_ASSERT BOOST_ASSERT
#endif

#if !defined(BOOST_INTRUSIVE_SAFE_MODE_CONTAINER_INSERTION_ASSERT)
#include <boost/assert.hpp>
#define BOOST_INTRUSIVE_SAFE_MODE_CONTAINER_INSERTION_ASSERT BOOST_ASSERT
#endif

#if !defined(BOOST_INTRUSIVE_SAFE_MODE_HOOK_DESTRUCTOR_ASSERT)
#include <boost/assert.hpp>
#define BOOST_INTRUSIVE_SAFE_MODE_HOOK_DESTRUCTOR_ASSERT BOOST_ASSERT
#endif

#endif //BOOST_INTRUSIVE_DETAIL_ASSERT_HPP
