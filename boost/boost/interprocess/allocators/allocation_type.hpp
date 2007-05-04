///////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_TYPE_COMMAND_HPP
#define BOOST_INTERPROCESS_TYPE_COMMAND_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

namespace boost {
namespace interprocess {

/// @cond
enum allocation_type_v
{   
   // constants for allocation commands
   allocate_new_v   = 0x01,
   expand_fwd_v     = 0x02,
   expand_bwd_v     = 0x04,
//   expand_both    = expand_fwd | expand_bwd,
//   expand_or_new  = allocate_new | expand_both,
   shrink_in_place_v = 0x08,
   nothrow_allocation_v = 0x10
};

typedef int allocation_type;
/// @endcond
static const allocation_type allocate_new       = (allocation_type)allocate_new_v;
static const allocation_type expand_fwd         = (allocation_type)expand_fwd_v;
static const allocation_type expand_bwd         = (allocation_type)expand_bwd_v;
static const allocation_type shrink_in_place    = (allocation_type)shrink_in_place_v;
static const allocation_type nothrow_allocation = (allocation_type)nothrow_allocation_v;

}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_TYPE_COMMAND_HPP

