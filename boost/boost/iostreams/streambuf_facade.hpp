// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_STREAMBUF_FACADE_HPP_INCLUDED
#define BOOST_IOSTREAMS_STREAMBUF_FACADE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <boost/iostreams/detail/disable_warnings.hpp>  // MSVC.

#include <memory>                                       // allocator.
#include <boost/iostreams/detail/forward.hpp>
#include <boost/iostreams/detail/param_type.hpp>
#include <boost/iostreams/detail/streambuf/direct_streambuf.hpp>
#include <boost/iostreams/detail/streambuf/indirect_streambuf.hpp>
#include <boost/iostreams/traits.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace iostreams {

namespace detail {

template<typename T, typename Tr, typename Alloc, typename Mode>
struct streambuf_facade_traits {
    typedef typename 
            mpl::if_<
                is_convertible<BOOST_IOSTREAMS_CATEGORY(T), direct_tag>,
                direct_streambuf<T, Tr>,
                indirect_streambuf<T, Tr, Alloc, Mode>
            >::type type;
};

} // End namespace detail.

template< typename T, 
          typename Tr = 
              std::char_traits<
                  BOOST_DEDUCED_TYPENAME io_char<T>::type 
              >,
          typename Alloc = std::allocator<BOOST_IOSTREAMS_CHAR_TYPE(T)>,
          typename Mode = BOOST_DEDUCED_TYPENAME io_mode<T>::type >
class streambuf_facade
    : public detail::streambuf_facade_traits<T, Tr, Alloc, Mode>::type
{
private:
    BOOST_STATIC_ASSERT((is_convertible<BOOST_IOSTREAMS_CATEGORY(T), Mode>::value));
    typedef typename 
            detail::streambuf_facade_traits<
                T, Tr, Alloc, Mode
            >::type                               base_type;
    typedef T                                     policy_type;
public:
    typedef BOOST_IOSTREAMS_CHAR_TYPE(T)          char_type;
    BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr)
public:
    streambuf_facade() { }
    // BEGIN DEBUG
    streambuf_facade( typename detail::param_type<T>::type t, 
                      std::streamsize buffer_size = -1, 
                      std::streamsize pback_size = -1 ) 
    { this->open_impl(detail::wrap(t), buffer_size, pback_size); }
    streambuf_facade( const reference_wrapper<T>& ref,
                      std::streamsize buffer_size = -1, 
                      std::streamsize pback_size = -1 ) 
    { this->open_impl(ref , buffer_size, pback_size); }
    void open( typename detail::param_type<T>::type t, 
               std::streamsize buffer_size = -1, 
               std::streamsize pback_size = -1 ) 
    { this->open_impl(detail::wrap(t), buffer_size, pback_size); }
    void open( const reference_wrapper<T>& ref , 
               std::streamsize buffer_size = -1, 
               std::streamsize pback_size = -1 ) 
    { this->open_impl(ref , buffer_size, pback_size); }
    template<typename U0> 
    streambuf_facade(U0 u0) { this->open_impl (T(u0)); }
    template<typename U0, typename U1> 
    streambuf_facade(U0 u0, U1 u1) { this->open_impl (T(u0, u1)); }
    template<typename U0, typename U1, typename U2> 
    streambuf_facade(U0 u0, U1 u1, U2 u2) { this->open_impl (T(u0, u1, u2)); }
    template<typename U0> 
    void open(U0 u0) { this->open_impl (T(u0)); }
    template<typename U0, typename U1> 
    void open(U0 u0, U1 u1) { this->open_impl (T(u0, u1)); }
    template<typename U0, typename U1, typename U2> 
    void open(U0 u0, U1 u1, U2 u2) { this->open_impl (T(u0, u1, u2)); }
    // END DEBUG
    //BOOST_IOSTREAMS_FORWARD( streambuf_facade, open_impl, T,
    //                         BOOST_IOSTREAMS_PUSH_PARAMS(),
    //                         BOOST_IOSTREAMS_PUSH_ARGS() )
private:
    void open_impl(const T& t BOOST_IOSTREAMS_PUSH_PARAMS())
        {   // Used for forwarding.
            if (this->is_open()) 
                throw std::ios_base::failure("already open");
            base_type::open(t BOOST_IOSTREAMS_PUSH_ARGS()); 
        } 
};

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/enable_warnings.hpp>  // MSVC.

#endif // #ifndef BOOST_IOSTREAMS_STREAMBUF_FACADE_HPP_INCLUDED
