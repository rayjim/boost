// Copyright (C) 2003, Fernando Luis Cacciola Carballal.
// Copyright (C) 2007, Tobias Schwinger.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef BOOST_UTILITY_INPLACE_FACTORY_04APR2007_HPP
#ifndef BOOST_PP_IS_ITERATING

#include <boost/utility/detail/in_place_factory_prefix.hpp>

namespace boost {

class in_place_factory_base {} ;

#define  BOOST_PP_ITERATION_LIMITS (0, BOOST_MAX_INPLACE_FACTORY_ARITY)
#define  BOOST_PP_FILENAME_1 <boost/utility/in_place_factory.hpp>
#include BOOST_PP_ITERATE()

} // namespace boost

#include <boost/utility/detail/in_place_factory_suffix.hpp>

#define BOOST_UTILITY_INPLACE_FACTORY_04APR2007_HPP
#else
#define N BOOST_PP_ITERATION()

#if N
template< BOOST_PP_ENUM_PARAMS(N, class A) >
#endif
class BOOST_PP_CAT(in_place_factory,N)
  : 
  public in_place_factory_base
{
public:

  explicit BOOST_PP_CAT(in_place_factory,N)
      ( BOOST_PP_ENUM_BINARY_PARAMS(N,A,const& a) )
#if N > 0
    : BOOST_PP_ENUM(N, BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT, _)
#endif
  {}

  template<class T>
  void* apply(void* address) const
  {
    return new(address) T( BOOST_PP_ENUM_PARAMS(N, m_a) );
  }

  template<class T>
  void* apply(void* address, std::size_t n) const
  {
    for(char* next = address = this->BOOST_NESTED_TEMPLATE apply<T>(address);
        !! --n;)
      this->BOOST_NESTED_TEMPLATE apply<T>(next = next+sizeof(T));
    return address; 
  }

  BOOST_PP_REPEAT(N, BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL, _)
};

#if N > 0
template< BOOST_PP_ENUM_PARAMS(N, class A) >
inline BOOST_PP_CAT(in_place_factory,N)< BOOST_PP_ENUM_PARAMS(N, A) >
in_place( BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a) )
{
  return BOOST_PP_CAT(in_place_factory,N)< BOOST_PP_ENUM_PARAMS(N, A) >
      ( BOOST_PP_ENUM_PARAMS(N, a) );
}
#else
inline in_place_factory0 in_place()
{
  return in_place_factory0();
}
#endif

#undef N
#endif
#endif

