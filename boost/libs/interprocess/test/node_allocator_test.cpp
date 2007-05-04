//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/allocators/node_allocator.hpp>
#include "print_container.hpp"
#include "dummy_test_allocator.hpp"
#include "movable_int.hpp"
#include "list_test.hpp"

using namespace boost::interprocess;

//We will work with wide characters for shared memory objects
//Alias a integer node allocator type
typedef node_allocator
   <int, managed_shared_memory::segment_manager> shmem_node_allocator_t;

//Alias list types
typedef list<int, shmem_node_allocator_t>    MyShmList;

int main ()
{
   if(test::list_test<managed_shared_memory, MyShmList, true>())
      return 1;

   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
