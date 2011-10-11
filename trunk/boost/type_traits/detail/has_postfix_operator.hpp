//  (C) Copyright 2009-2011 Frederic Bron, Robert Stewart, Steven Watanabe & Roman Perepelitsa.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#include <boost/config.hpp>
#include <boost/type_traits/ice.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

// avoid warnings
#if defined(__GNUC__) && ((__GNUC__==3 && __GNUC_MINOR__>=1) || (__GNUC__>3))
#   pragma GCC system_header
#elif defined(BOOST_MSVC)
#   pragma warning ( push )
#   pragma warning ( disable : 4244 4913 )
#endif

namespace boost {
namespace detail {

// This namespace ensures that argument-dependent name lookup does not mess things up.
namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl) {

// 1. a function to have an instance of type T without requiring T to be default
// constructible
template <typename T> T &make();


// 2. checks if the operator returns void or not
// conditions: Lhs!=void

// we first redefine "operator," so that we have no compilation error if
// operator BOOST_TT_TRAIT_OP returns void and we can use the return type of
// (lhs BOOST_TT_TRAIT_OP, returns_void_t()) to deduce if
// operator BOOST_TT_TRAIT_OP returns void or not:
// - operator BOOST_TT_TRAIT_OP returns void   -> (lhs BOOST_TT_TRAIT_OP, returns_void_t()) returns returns_void_t
// - operator BOOST_TT_TRAIT_OP returns !=void -> (lhs BOOST_TT_TRAIT_OP, returns_void_t()) returns int
struct returns_void_t { };
template <typename T> int operator,(const T&, returns_void_t);
template <typename T> int operator,(const volatile T&, returns_void_t);

// this intermediate trait has member value of type bool:
// - value==true -> operator BOOST_TT_TRAIT_OP returns void
// - value==false -> operator BOOST_TT_TRAIT_OP does not return void
template < typename Lhs >
struct operator_returns_void {
   // overloads of function returns_void make the difference
   // yes_type and no_type have different size by construction
   static ::boost::type_traits::yes_type returns_void(returns_void_t);
   static ::boost::type_traits::no_type returns_void(int);
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(::boost::type_traits::yes_type)==sizeof(returns_void((make<Lhs>() BOOST_TT_TRAIT_OP,returns_void_t())))));
   //BOOST_STATIC_CONSTANT(bool, value = (::boost::is_same< bool volatile, Lhs >::value));
};


// 3. checks if the return type is Ret or Ret==dont_care
// conditions: Lhs!=void

struct dont_care { };

template < typename Lhs, typename Ret, bool Returns_void >
struct operator_returns_Ret;

template < typename Lhs >
struct operator_returns_Ret < Lhs, dont_care, true > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs >
struct operator_returns_Ret < Lhs, dont_care, false > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs >
struct operator_returns_Ret < Lhs, void, true > {
   BOOST_STATIC_CONSTANT(bool, value = true);
};

template < typename Lhs >
struct operator_returns_Ret < Lhs, void, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Lhs, typename Ret >
struct operator_returns_Ret < Lhs, Ret, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// otherwise checks if it is convertible to Ret using the sizeof trick
// based on overload resolution
// condition: Ret!=void and Ret!=dont_care and the operator does not return void
template < typename Lhs, typename Ret >
struct operator_returns_Ret < Lhs, Ret, false > {
   static ::boost::type_traits::yes_type is_convertible_to_Ret(Ret); // this version is preferred for types convertible to Ret
   static ::boost::type_traits::no_type is_convertible_to_Ret(...); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(is_convertible_to_Ret(make<Lhs>() BOOST_TT_TRAIT_OP))==sizeof(::boost::type_traits::yes_type)));
};


// 4. we provide our operator definition for types that do not have one already

// a type returned from operator BOOST_TT_TRAIT_OP when no such operator is
// found in the type's own namespace (our own operator is used) so that we have
// a means to know that our operator was used
struct no_operator { };

// this class allows implicit conversions and makes the following operator
// definition less-preferred than any other such operators that might be found
// via argument-dependent name lookup
struct any { template <class T> any(T const&); };

// when operator BOOST_TT_TRAIT_OP is not available, this one is used
no_operator operator BOOST_TT_TRAIT_OP (const any&, int);


// 5. checks for operator existence
// condition: Lhs!=void

// checks if our definition of operator BOOST_TT_TRAIT_OP is used or an other
// existing one;
// this is done with redefinition of "operator," that returns no_operator or has_operator
struct has_operator { };
no_operator operator,(no_operator, has_operator);

template < typename Lhs >
struct operator_exists {
   static ::boost::type_traits::yes_type check(has_operator); // this version is preferred when operator exists
   static ::boost::type_traits::no_type check(no_operator); // this version is used otherwise

   BOOST_STATIC_CONSTANT(bool, value = (sizeof(check(((make<Lhs>() BOOST_TT_TRAIT_OP),make<has_operator>())))==sizeof(::boost::type_traits::yes_type)));
};


// 5. main trait: to avoid any compilation error, this class behaves
// differently when operator BOOST_TT_TRAIT_OP(Lhs) is forbidden by the
// standard.
// Forbidden_if is a bool that is:
// - true when the operator BOOST_TT_TRAIT_OP(Lhs) is forbidden by the standard
//   (would yield compilation error if used)
// - false otherwise
template < typename Lhs, typename Ret, bool Forbidden_if >
struct trait_impl1;

template < typename Lhs, typename Ret >
struct trait_impl1 < Lhs, Ret, true > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template < typename Lhs, typename Ret >
struct trait_impl1 < Lhs, Ret, false > {
   BOOST_STATIC_CONSTANT(bool,
      value = (
         ::boost::type_traits::ice_and<
            operator_exists < Lhs >::value,
            operator_returns_Ret < Lhs, Ret, operator_returns_void < Lhs >::value >::value
         >::value
      )
   );
};

// specialization needs to be declared for the special void case
template < typename Ret >
struct trait_impl1 < void, Ret, false > {
   BOOST_STATIC_CONSTANT(bool, value = false);
};

// defines some typedef for convenience
template < typename Lhs, typename Ret >
struct trait_impl {
   typedef typename ::boost::remove_reference<Lhs>::type Lhs_noref;
   typedef typename ::boost::remove_cv<Lhs_noref>::type Lhs_nocv;
   typedef typename ::boost::remove_cv< typename ::boost::remove_reference< typename ::boost::remove_pointer<Lhs_noref>::type >::type >::type Lhs_noptr;
   BOOST_STATIC_CONSTANT(bool, value = (trait_impl1 < Lhs_noref, Ret, BOOST_TT_FORBIDDEN_IF >::value));
};

} // namespace impl
} // namespace detail

// this is the accessible definition of the trait to end user
BOOST_TT_AUX_BOOL_TRAIT_DEF2(BOOST_TT_TRAIT_NAME, Lhs, Ret=::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::dont_care, (::boost::detail::BOOST_JOIN(BOOST_TT_TRAIT_NAME,_impl)::trait_impl< Lhs, Ret >::value))

} // namespace boost

#if defined(BOOST_MSVC)
#   pragma warning ( pop )
#endif

#include <boost/type_traits/detail/bool_trait_undef.hpp>
