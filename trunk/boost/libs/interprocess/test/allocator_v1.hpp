///////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_ALLOCATOR_V1_HPP
#define BOOST_INTERPROCESS_ALLOCATOR_V1_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/allocators/allocation_type.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/detail/version_type.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <memory>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

/*!\file
   Describes an allocator_v1 that allocates portions of fixed size
   memory buffer (shared memory, mapped file...)
*/

namespace boost {
namespace interprocess {
namespace test {

/*!An STL compatible allocator_v1 that uses a segment manager as 
   memory source. The internal pointer type will of the same type (raw, smart) as
   "typename SegmentManager::void_pointer" type. This allows
   placing the allocator_v1 in shared memory, memory mapped-files, etc...*/
template<class T, class SegmentManager>
class allocator_v1 
{
 private:
   /*!Self type*/
   typedef allocator_v1<T, SegmentManager>   self_t;

   /*!Segment manager*/
   typedef SegmentManager                 segment_manager;

   /*!Pointer to void */
   typedef typename segment_manager::void_pointer  aux_pointer_t;

   /*!Typedef to const void pointer */
   typedef typename 
      detail::template pointer_to_other
         <aux_pointer_t, const void>::type   cvoid_ptr;

   /*!Pointer to the allocator_v1*/
   typedef typename detail::pointer_to_other
      <cvoid_ptr, segment_manager>::type     alloc_ptr_t;

   /*!Not assignable from related allocator_v1*/
   template<class T2, class SegmentManager2>
   allocator_v1& operator=(const allocator_v1<T2, SegmentManager2>&);

   /*!Not assignable from other allocator_v1*/
   allocator_v1& operator=(const allocator_v1&);

   /*!Pointer to the allocator_v1*/
   alloc_ptr_t mp_mngr;

 public:
   typedef T                                    value_type;
   typedef typename detail::pointer_to_other
      <cvoid_ptr, T>::type                      pointer;
   typedef typename detail::
      pointer_to_other<pointer, const T>::type  const_pointer;
   typedef typename workaround::random_it
      <value_type>::reference                   reference;
   typedef typename workaround::random_it
      <value_type>::const_reference             const_reference;
   typedef std::size_t                          size_type;
   typedef std::ptrdiff_t                       difference_type;

   /*!Obtains an allocator_v1 of other type*/
   template<class T2>
   struct rebind
   {   
      typedef allocator_v1<T2, SegmentManager>     other;
   };

   /*!Returns the segment manager. Never throws*/
   segment_manager* get_segment_manager()const
   {  return detail::get_pointer(mp_mngr);   }

   /*!Returns address of mutable object. Never throws*/
   pointer address(reference value) const
   {  return pointer(boost::addressof(value));  }

   /*!Returns address of non mutable object. Never throws*/
   const_pointer address(const_reference value) const
   {  return const_pointer(boost::addressof(value));  }

   /*!Constructor from the segment manager. Never throws*/
   allocator_v1(segment_manager *segment_mngr) 
      : mp_mngr(segment_mngr) { }

   /*!Constructor from other allocator_v1. Never throws*/
   allocator_v1(const allocator_v1 &other) 
      : mp_mngr(other.get_segment_manager()){ }

   /*!Constructor from related allocator_v1. Never throws*/
   template<class T2>
   allocator_v1(const allocator_v1<T2, SegmentManager> &other) 
      : mp_mngr(other.get_segment_manager()){}

   /*!Allocates memory for an array of count elements. 
      Throws boost::interprocess::bad_alloc if there is no enough memory*/
   pointer allocate(size_type count, cvoid_ptr hint = 0)
   {  return pointer((value_type*)mp_mngr->allocate(count*sizeof(value_type)));  }

   /*!Deallocates memory previously allocated. Never throws*/
   void deallocate(const pointer &ptr, size_type)
   {  mp_mngr->deallocate(detail::get_pointer(ptr));  }

   /*!Construct object, calling constructor. 
      Throws if T(const T&) throws*/
   void construct(const pointer &ptr, const_reference value)
   {  new(detail::get_pointer(ptr)) value_type(value);  }

   /*!Destroys object. Throws if object's destructor throws*/
   void destroy(const pointer &ptr)
   {  BOOST_ASSERT(ptr != 0); (*ptr).~value_type();  }

   /*!Returns the number of elements that could be allocated. Never throws*/
   size_type max_size() const
   {  return mp_mngr->get_size();   }

   /*!Swap segment manager. Does not throw. If each allocator_v1 is placed in
      different memory segments, the result is undefined.*/
   friend void swap(self_t &alloc1, self_t &alloc2)
   {  detail::do_swap(alloc1.mp_mngr, alloc2.mp_mngr);   }
};

/*!Equality test for same type of allocator_v1*/
template<class T, class SegmentManager> inline
bool operator==(const allocator_v1<T , SegmentManager>  &alloc1, 
                const allocator_v1<T, SegmentManager>  &alloc2)
   {  return alloc1.get_segment_manager() == alloc2.get_segment_manager(); }

/*!Inequality test for same type of allocator_v1*/
template<class T, class SegmentManager> inline
bool operator!=(const allocator_v1<T, SegmentManager>  &alloc1, 
                const allocator_v1<T, SegmentManager>  &alloc2)
   {  return alloc1.get_segment_manager() != alloc2.get_segment_manager(); }

}  //namespace test {
}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_ALLOCATOR_V1_HPP

