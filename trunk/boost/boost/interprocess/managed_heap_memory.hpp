//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_MANAGED_HEAP_MEMORY_HPP
#define BOOST_INTERPROCESS_MANAGED_HEAP_MEMORY_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <vector>
#include <boost/interprocess/detail/managed_memory_impl.hpp>
#include <boost/detail/no_exceptions_support.hpp>

/*!\file
   Describes a named heap memory allocation user class. 
*/

namespace boost {
namespace interprocess {

/// @cond
namespace detail {

   //!This class defines an operator() that creates a heap memory
   //!of the requested size. The rest of the parameters are
   //!passed in the constructor. The class a template parameter
   //!to be used with create_from_file/create_from_istream functions
   //!of basic_named_object classes
   class heap_mem_creator_t
   {
      public:
      heap_mem_creator_t(std::vector<char> &heapmem)
      : m_heapmem(heapmem){}

      void *operator()(std::size_t size)
      {
         BOOST_TRY{
            m_heapmem.resize(size, char(0));
         }
         BOOST_CATCH(...){
            return 0;
         }
         BOOST_CATCH_END
         return &m_heapmem[0];
      }      
      private:
      std::vector<char> &m_heapmem;
   };
}  //namespace detail {
/// @endcond

//!A basic heap memory named object creation class. Initializes the 
//!heap memory segment. Inherits all basic functionality from 
//!basic_managed_memory_impl<CharType, AllocationAlgorithm, IndexType>*/
template
      <
         class CharType, 
         class AllocationAlgorithm, 
         template<class IndexConfig> class IndexType
      >
class basic_managed_heap_memory 
   : public detail::basic_managed_memory_impl <CharType, AllocationAlgorithm, IndexType>
{
   /// @cond
   private:

   typedef detail::basic_managed_memory_impl 
      <CharType, AllocationAlgorithm, IndexType>             base_t;
   /// @endcond

   public: //functions

   //!Constructor. Never throws.
   basic_managed_heap_memory(){}

   //!Destructor. Calls priv_close. Never throws.
   ~basic_managed_heap_memory()
   {  this->priv_close();  }

   //!Creates heap memory and initializes the segment manager.
   //!This can throw.
   basic_managed_heap_memory(std::size_t size)
      :  m_heapmem(size, char(0))
   {
      if(!base_t::create_impl(&m_heapmem[0], size)){
         this->priv_close();
         throw interprocess_exception();
      }
   }

   //!Moves the ownership of "moved"'s managed memory to *this. Does not throw
   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   basic_managed_heap_memory
      (detail::moved_object<basic_managed_heap_memory> &moved)
   {  this->swap(moved.get());   }
   #else
   basic_managed_heap_memory(basic_managed_heap_memory &&moved)
   {  this->swap(moved);   }
   #endif

   //!Moves the ownership of "moved"'s managed memory to *this. Does not throw
   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   basic_managed_heap_memory &operator=
      (detail::moved_object<basic_managed_heap_memory> &moved)
   {  this->swap(moved.get());   return *this;  }
   #else
   basic_managed_heap_memory &operator=
      (basic_managed_heap_memory &&moved)
   {  this->swap(moved);   return *this;  }
   #endif

   //!Tries to resize internal heap memory so that
   //!we have room for more objects. 
   //!WARNING: If memory is reallocated, all the objects will 
   //!be binary-copied to the new buffer. To be able to use
   //!this function, all pointers constructed in this buffer
   //!must be offset pointers. Otherwise, the result is undefined.
   //!Returns true if the growth has been successful, so you will
   //!have some extra bytes to allocate new objects. If returns 
   //!false, the heap allocation has failed.
   bool grow(std::size_t extra_bytes)
   {  
      //If memory is reallocated, data will
      //be automatically copied
      BOOST_TRY{
         m_heapmem.resize(m_heapmem.size()+extra_bytes);
      }
      BOOST_CATCH(...){
         return false;
      }
      BOOST_CATCH_END

      //Grow always works
      base_t::close_impl();
      base_t::open_impl(&m_heapmem[0], m_heapmem.size());
      base_t::grow(extra_bytes);
      return true;
   }

   //!Swaps the ownership of the managed heap memories managed by *this and other.
   //!Never throws.
   void swap(basic_managed_heap_memory &other)
   {
      base_t::swap(other);
      m_heapmem.swap(other.m_heapmem);
   }

   /// @cond
   private:
   //!Frees resources. Never throws.
   void priv_close()
   {  
      base_t::destroy_impl();
      std::vector<char>().swap(m_heapmem);
   }

   std::vector<char>  m_heapmem;
   /// @endcond
};

}  //namespace interprocess {

}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_MANAGED_HEAP_MEMORY_HPP

