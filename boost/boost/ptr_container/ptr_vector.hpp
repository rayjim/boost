//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2005. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#ifndef BOOST_PTR_CONTAINER_PTR_VECTOR_HPP
#define BOOST_PTR_CONTAINER_PTR_VECTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <vector>
#include <boost/ptr_container/ptr_sequence_adapter.hpp>

namespace boost
{

    template
    < 
        class T, 
        class CloneAllocator = heap_clone_allocator,
        class Allocator      = std::allocator<void*>
    >
    class ptr_vector : public 
        ptr_sequence_adapter< T, 
                              std::vector<void*,Allocator>, 
                              CloneAllocator >
    {  
        typedef ptr_sequence_adapter< T, 
                                      std::vector<void*,Allocator>, 
                                      CloneAllocator > 
            base_class;

        typedef ptr_vector<T,CloneAllocator,Allocator> this_type;
        
    public:

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))         
        BOOST_PTR_CONTAINER_DEFINE_NON_INHERITED_MEMBERS( this_type, 
                                                          base_class );
#else
        BOOST_PTR_CONTAINER_DEFINE_NON_INHERITED_MEMBERS( ptr_vector, 
                                                          base_class );
#endif        

        ptr_vector( BOOST_DEDUCED_TYPENAME base_class::size_type n,
                    const allocator_type& alloc = allocator_type() )
          : base_class(alloc)
        {
            this->c_private().reserve( n );
        }


    };

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename T, typename CA, typename A >
    inline ptr_vector<T,CA,A>* new_clone( const ptr_vector<T,CA,A>& r )
    {
        return r.clone().release();
    }

    /////////////////////////////////////////////////////////////////////////
    // swap

    template< typename T, typename CA, typename A >
    inline void swap( ptr_vector<T,CA,A>& l, ptr_vector<T,CA,A>& r )
    {
        l.swap(r);
    }
    
}

#endif
