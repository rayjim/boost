//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
//[doc_map
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <utility>

int main ()
{
   using namespace boost::interprocess;
   //Shared memory front-end that is able to construct objects
   //associated with a c-string. Erase previous shared memory with the name
   //to be used and create the memory segment at the specified address and initialize resources
   shared_memory_object::remove("MySharedMemory");

   try{
      managed_shared_memory segment
         (create_only 
         ,"MySharedMemory" //segment name
         ,65536);          //segment size in bytes

      //Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
      //so the allocator must allocate that pair.
      typedef int    KeyType;
      typedef float  MappedType;
      typedef std::pair<int, float> ValueType;

      //Alias an STL compatible allocator of for the map.
      //This allocator will allow to place containers
      //in managed shared memory segments
      typedef allocator<ValueType, managed_shared_memory::segment_manager> 
         ShmemAllocator;

      //Alias a map of ints that uses the previous STL-like allocator.
      //Note that the third parameter argument is the ordering function
      //of the map, just like with std::map, used to compare the keys.
      typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

      //Initialize the shared memory STL-compatible allocator
      ShmemAllocator alloc_inst (segment.get_segment_manager());

      //Construct a shared memory map.
      //Note that the first parameter is the comparison function,
      //and the second one the allocator.
      //This the same signature as std::map's constructor taking an allocator
      MyMap *mymap = 
         segment.construct<MyMap>("MyMap")      //object name
                                    (std::less<int>() //first  ctor parameter
                                    ,alloc_inst);     //second ctor parameter

      //Insert data in the map
      for(int i = 0; i < 100; ++i){
         mymap->insert(std::pair<int, float>(i, (float)i));
      }
   }
   catch(...){
      shared_memory_object::remove("MySharedMemory");
      throw;
   }
   shared_memory_object::remove("MySharedMemory");
   return 0;
}
//]
#include <boost/interprocess/detail/config_end.hpp>
