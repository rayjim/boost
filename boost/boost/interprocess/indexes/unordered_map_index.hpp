//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gazta�aga 2005-2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_UNORDERED_MAP_INDEX_HPP
#define BOOST_INTERPROCESS_UNORDERED_MAP_INDEX_HPP

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <functional>
#include <utility>
#include <boost/unordered_map.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/allocators/private_node_allocator.hpp>

/*!\file
   Describes index adaptor of boost::unordered_map container, to use it
   as name/shared memory index
*/

namespace boost { namespace interprocess {

/*!Helper class to define typedefs from IndexTraits*/
template <class MapConfig>
struct unordered_map_index_aux
{
   typedef typename MapConfig::key_type                  key_type;
   typedef typename MapConfig::mapped_type               mapped_type;
   typedef std::equal_to<key_type>                       key_equal;
   typedef std::pair<const key_type, mapped_type>        value_type;
   typedef private_node_allocator
            <value_type,
               64,
               typename MapConfig::segment_manager>     allocator_type;
    struct hasher
      : std::unary_function<key_type, std::size_t>
    {
        std::size_t operator()(const key_type &val) const
        {
            typedef typename key_type::char_type    char_type;
            const char_type *beg = detail::get_pointer(val.mp_str),
                            *end = beg + val.m_len;
            return boost::hash_range(beg, end);
        }
    };
   typedef unordered_map<key_type,  mapped_type, hasher,
                         key_equal, allocator_type>      index_t;
};

/*!Index type based in unordered_map. Just derives from unordered_map and
   defines the interface needed by the shared named object creation class*/
template <class MapConfig>
class unordered_map_index
   //Derive class from unordered_map specialization
   : public unordered_map_index_aux<MapConfig>::index_t
{
   typedef unordered_map_index_aux<MapConfig>   index_aux;
   typedef typename index_aux::index_t          base_type;
   typedef typename MapConfig::segment_manager segment_manager;

 public:
   /*!Constructor. Takes a pointer to the
      segment manager. Can throw*/
   unordered_map_index(segment_manager *segment_mngr)
      : base_type(0,
                  typename index_aux::hasher(),
                  typename index_aux::key_equal(),
                  segment_mngr){}

   /*!This reserves memory to optimize the insertion of n
      elements in the index*/
   void reserve(std::size_t n)
      {  base_type::rehash(n);  }
};

/*!Trait classes to detect if an index is a node
   index. This allows more efficient operations
   when deallocating named objects.*/
template<class MapConfig>
struct is_node_index
   <boost::interprocess::unordered_map_index<MapConfig> >
{
   enum {   value = true };
};

}}   //namespace boost { namespace interprocess {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_UNORDERED_MAP_INDEX_HPP
