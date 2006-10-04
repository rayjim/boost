//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TR1_TUPLE_HPP_INCLUDED
#  define BOOST_TR1_TUPLE_HPP_INCLUDED
#  include <boost/tr1/detail/config.hpp>

#ifdef BOOST_HAS_TR1_TUPLE

#  include BOOST_TR1_HEADER(tuple)

#else

#if defined(BOOST_TR1_USE_OLD_TUPLE)

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/type_traits/integral_constant.hpp>

namespace std{ namespace tr1{

using ::boost::tuple;

// [6.1.3.2] Tuple creation functions
using ::boost::tuples::ignore;
using ::boost::make_tuple;
using ::boost::tie;

// [6.1.3.3] Tuple helper classes
template <class T> 
struct tuple_size 
   : public ::boost::integral_constant
   < ::std::size_t, ::boost::tuples::length<T>::value>
{};

template < int I, class T>
struct tuple_element
{
   typedef typename boost::tuples::element<I,T>::type type;
};

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
// [6.1.3.4] Element access
using ::boost::get;
#endif

} } // namespaces

#else

#include <boost/fusion/tuple.hpp>
#include <boost/fusion/sequence/adapted/std_pair.hpp>

namespace std{ namespace tr1{

using ::boost::fusion::tuple;

// [6.1.3.2] Tuple creation functions
using ::boost::fusion::ignore;
using ::boost::fusion::make_tuple;
using ::boost::fusion::tie;
using ::boost::fusion::get;

// [6.1.3.3] Tuple helper classes
using ::boost::fusion::tuple_size;
using ::boost::fusion::tuple_element;

}}

#endif

#endif

#endif
