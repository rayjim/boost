// (C) Copyright 2002-2008, Fernando Luis Cacciola Carballal.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// 21 Ago 2002 (Created) Fernando Cacciola
// 16 Jan 2008 (Worked around compiler bugs, added initialized_value) Fernando Cacciola, Niels Dekker
//
#ifndef BOOST_UTILITY_VALUE_INIT_21AGO2002_HPP
#define BOOST_UTILITY_VALUE_INIT_21AGO2002_HPP

// Note: The implementation of boost::value_initialized had to deal with the
// fact that various compilers haven't fully implemented value-initialization.
// The constructor of boost::value_initialized<T> works around these compiler
// issues, by clearing the bytes of T, before constructing the T object it
// contains. More details on these issues are at libs/utility/value_init.htm

#include <boost/aligned_storage.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/type_traits/cv_traits.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <cstring>
#include <new>

namespace boost {

template<class T>
class value_initialized
{
  private :
    struct wrapper
    {
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x592))
      typename
#endif 
      remove_const<T>::type data;
    };

    mutable aligned_storage<sizeof(wrapper), alignment_of<wrapper>::value> x;

  public :

    value_initialized()
    {
      std::memset(x.address(), 0, sizeof(x));
#ifdef BOOST_MSVC
#pragma warning(push)
#if _MSC_VER >= 1310
// When using MSVC 7.1 or higher, the following placement new expression may trigger warning C4345:
// "behavior change: an object of POD type constructed with an initializer of the form ()
// will be default-initialized".  There is no need to worry about this, though.
#pragma warning(disable: 4345)
#endif
#endif
      new (x.address()) wrapper();
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
    }

    value_initialized(value_initialized const & arg)
    {
      new (x.address()) wrapper( *static_cast<wrapper const *>(arg.x.address()) );
    }

    value_initialized & operator=(value_initialized const & arg)
    {
      T & this_data = this->data();
      T const & arg_data = arg.data();
      this_data = arg_data;
      return *this;
    }

    ~value_initialized()
    {
      static_cast<wrapper *>(x.address())->wrapper::~wrapper();
    }

    T& data() const
    {
      return static_cast<wrapper *>(x.address())->data;
    }

    operator T&() const { return this->data(); }

} ;



template<class T>
T const& get ( value_initialized<T> const& x )
{
  return x.data() ;
}
template<class T>
T& get ( value_initialized<T>& x )
{
  return x.data() ;
}


class initialized_value
{
  public :
    
    template <class T> operator T() const
    {
      return get( value_initialized<T>() );
    }
};


} // namespace boost


#endif
