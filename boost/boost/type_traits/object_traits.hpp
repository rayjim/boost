//  (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.
//
//  defines object traits classes:
//  is_object, is_scalar, is_class, is_compound, is_pod, 
//  has_trivial_constructor, has_trivial_copy, has_trivial_assign, 
//  has_trivial_destructor, is_empty.
//

#ifndef BOOST_TT_OBJECT_TRAITS_HPP_INLCUDED
#define BOOST_TT_OBJECT_TRAITS_HPP_INLCUDED

#include "boost/type_traits/has_trivial_assign.hpp"
#include "boost/type_traits/has_trivial_constructor.hpp"
#include "boost/type_traits/has_trivial_copy.hpp"
#include "boost/type_traits/has_trivial_destructor.hpp"
#include "boost/type_traits/has_nothrow_constructor.hpp"
#include "boost/type_traits/has_nothrow_copy.hpp"
#include "boost/type_traits/has_nothrow_assign.hpp"
#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/type_traits/is_class.hpp"
#include "boost/type_traits/is_compound.hpp"
#include "boost/type_traits/is_empty.hpp"
#include "boost/type_traits/is_object.hpp"
#include "boost/type_traits/is_pod.hpp"
#include "boost/type_traits/is_scalar.hpp"
#include "boost/type_traits/is_stateless.hpp"

#endif // BOOST_TT_OBJECT_TRAITS_HPP_INLCUDED
