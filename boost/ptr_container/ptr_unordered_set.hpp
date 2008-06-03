//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#ifndef BOOST_PTR_CONTAINER_PTR_UNORDERED_SET_HPP
#define BOOST_PTR_CONTAINER_PTR_UNORDERED_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/ptr_container/indirect_fun.hpp>
#include <boost/ptr_container/ptr_set_adapter.hpp>
#include <boost/unordered_set.hpp>

namespace boost
{

    template
    < 
        class Key, 
        class Hash           = boost::hash<Key>,
        class Pred           = std::equal_to<Key>,
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_unordered_set : 
        public ptr_set_adapter< Key, 
                                boost::unordered_set<void*,void_ptr_indirect_fun<Hash,Key>,
                                                     void_ptr_indirect_fun<Pred,Key>,Allocator>,
                                CloneAllocator, false >
    {
        typedef ptr_set_adapter< Key, 
                                 boost::unordered_set<void*,void_ptr_indirect_fun<Hash,Key>,
                                 void_ptr_indirect_fun<Pred,Key>,Allocator>,
                                 CloneAllocator, false >
             base_type;

        typedef ptr_unordered_set<Key,Hash,Pred,CloneAllocator,Allocator> this_type;

    public:
        typedef typename base_type::size_type size_type;
        
    public:
        ptr_unordered_set()
        {}

        explicit ptr_unordered_set( size_type n )
        : base_type( n, ptr_container_detail::unordered_associative_container_tag() )
        { }
        
        ptr_unordered_set( size_type n,
                           const Hash& comp,
                           const Pred& pred   = Pred(),                                         
                           const Allocator& a = Allocator() )
         : base_type( n, comp, pred, a ) 
        { }

        // @todo: add const with two iterator arguments, and use
        //        tags to by-pass default arguments
        
        template< typename InputIterator >
        ptr_unordered_set( InputIterator first, InputIterator last,
                           const Hash& comp   = Hash(),
                           const Pred& pred   = Pred(),
                           const Allocator& a = Allocator() )
         : base_type( first, last, comp, pred, a ) 
        { }

        BOOST_PTR_CONTAINER_DEFINE_RELEASE_AND_CLONE( ptr_unordered_set,
                                                      base_type,
                                                      this_type );
        BOOST_PTR_CONTAINER_DEFINE_COPY_CONSTRUCTORS( ptr_unordered_set, base_type )
                
    };
        
        
        
    template
    < 
        class Key, 
        class Hash           = boost::hash<Key>,
        class Pred           = std::equal_to<Key>,
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_unordered_multiset : 
          public ptr_set_adapter< Key, 
                                boost::unordered_multiset<void*,void_ptr_indirect_fun<Hash,Key>,
                                                                void_ptr_indirect_fun<Pred,Key>,Allocator>,
                                            CloneAllocator, false >
    {
      typedef ptr_set_adapter< Key, 
              boost::unordered_multiset<void*,void_ptr_indirect_fun<Hash,Key>,
                                        void_ptr_indirect_fun<Pred,Key>,Allocator>,
                                        CloneAllocator, false >
              base_type;
        typedef ptr_unordered_multiset<Key,Hash,Pred,CloneAllocator,Allocator> this_type;

    public:
        typedef typename base_type::size_type size_type;
        
    public:
        explicit ptr_unordered_multiset( size_type n )
         : base_type( n, ptr_container_detail::unordered_associative_container_tag() ) 
        { }

        ptr_unordered_multiset( size_type n,
                                const Hash& comp,
                                const Pred& pred   = Pred(),                                         
                                const Allocator& a = Allocator() )
         : base_type( n, comp, pred, a ) 
        { }

        template< typename InputIterator >
        ptr_unordered_multiset( InputIterator first, InputIterator last,
                                const Hash& comp   = Hash(),
                                const Pred& pred   = Pred(),
                                const Allocator& a = Allocator() )
         : base_type( first, last, comp, pred, a ) 
        { }

        BOOST_PTR_CONTAINER_DEFINE_RELEASE_AND_CLONE( ptr_unordered_multiset, 
                                                      base_type,
                                                      this_type );   
        BOOST_PTR_CONTAINER_DEFINE_COPY_CONSTRUCTORS( ptr_unordered_multiset, 
                                                      base_type );     

    };

    /////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename K, typename C, typename CA, typename A >
    inline ptr_unordered_set<K,C,CA,A>* new_clone( const ptr_unordered_set<K,C,CA,A>& r )
    {
        return r.clone().release();
    }

    template< typename K, typename C, typename CA, typename A >
    inline ptr_unordered_multiset<K,C,CA,A>* new_clone( const ptr_unordered_multiset<K,C,CA,A>& r )
    {
        return r.clone().release();
    }
    
    /////////////////////////////////////////////////////////////////////////
    // swap

    template< typename K, typename C, typename CA, typename A >
    inline void swap( ptr_unordered_set<K,C,CA,A>& l, ptr_unordered_set<K,C,CA,A>& r )
    {
        l.swap(r);
    }

    template< typename K, typename C, typename CA, typename A >
    inline void swap( ptr_unordered_multiset<K,C,CA,A>& l, ptr_unordered_multiset<K,C,CA,A>& r )
    {
        l.swap(r);
    }

}

#endif
