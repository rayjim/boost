/**
 * Pointer Container Library
 * (C) Thorsten Ottosen 2003-2005
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_PTR_SET_HPP
#define BOOST_PTR_CONTAINER_PTR_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/ptr_container/indirect_fun.hpp>
#include <boost/ptr_container/ptr_set_adapter.hpp>
#include <set>

namespace boost
{

    template
    < 
        class Key, 
        class Compare        = std::less<Key>,
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_set : 
        public ptr_set_adapter< Key, 
                                std::set<void*,void_ptr_indirect_fun<Compare,Key>,Allocator>,
                                CloneAllocator >
    {
        typedef ptr_set_adapter< Key, std::set<void*,void_ptr_indirect_fun<Compare,Key>,Allocator>,
                                 CloneAllocator >
             base_type;
                  
    public:
        explicit ptr_set( const Compare& comp = Compare(),
                          const Allocator& a = Allocator() ) 
         : base_type( comp, a ) 
        { }
        
        template< typename InputIterator >
        ptr_set( InputIterator first, InputIterator last, 
                 const Compare& comp = Compare(),
                 const Allocator& a = Allocator() ) 
         : base_type( first, last, comp, a )
        { }

        BOOST_PTR_CONTAINER_DEFINE_RELEASE_AND_CLONE( ptr_set, 
                                                        base_type );
        
    };
        
        
        
    template
    < 
        class Key, 
        class Compare        = std::less<Key>,
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_multiset : 
        public ptr_multiset_adapter< Key,
                                     std::multiset<void*,void_ptr_indirect_fun<Compare,Key>,Allocator>,
                                     CloneAllocator >
    {
        typedef ptr_multiset_adapter< Key,
                                      std::multiset<void*,void_ptr_indirect_fun<Compare,Key>,Allocator>,
                                      CloneAllocator >
              base_type;
    public:
        explicit ptr_multiset( const Compare&   comp = Compare(),
                               const Allocator& a    = Allocator() )
         : base_type( comp, a ) 
        { }
        
        template< typename InputIterator >
        ptr_multiset( InputIterator first, InputIterator last,
                      const Compare& comp = Compare(),
                      const Allocator& a  = Allocator() )
         : base_type( first, last, comp, a ) 
        { }
        
        BOOST_PTR_CONTAINER_DEFINE_RELEASE_AND_CLONE( ptr_multiset, 
                                                        base_type );
                                                                            
    };

    /////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename K, typename C, typename CA, typename A >
    inline ptr_set<K,C,CA,A>* new_clone( const ptr_set<K,C,CA,A>& r )
    {
        return r.clone().release();
    }

    template< typename K, typename C, typename CA, typename A >
    inline ptr_multiset<K,C,CA,A>* new_clone( const ptr_multiset<K,C,CA,A>& r )
    {
        return r.clone().release();
    }
    
    /////////////////////////////////////////////////////////////////////////
    // swap

    template< typename K, typename C, typename CA, typename A >
    inline void swap( ptr_set<K,C,CA,A>& l, ptr_set<K,C,CA,A>& r )
    {
        l.swap(r);
    }

    template< typename K, typename C, typename CA, typename A >
    inline void swap( ptr_multiset<K,C,CA,A>& l, ptr_multiset<K,C,CA,A>& r )
    {
        l.swap(r);
    }

}

#endif
