// Copyright 2005 Alexander Nasonov.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILE_boost_type_traits_integral_promotion_hpp_INCLUDED
#define FILE_boost_type_traits_integral_promotion_hpp_INCLUDED

#include <boost/config.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_volatile.hpp>

// Should be the last #include
#include <boost/type_traits/detail/type_trait_def.hpp>
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost {

namespace type_traits { namespace detail {

// 4.5/2
BOOST_TT_AUX_BOOL_TRAIT_DEF1(need_promotion, T, boost::is_enum<T>::value)

// 4.5/1
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, char              , true)
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, signed char       , true)
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, unsigned char     , true)
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, signed short int  , true)
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, unsigned short int, true)


// Specializations for non-standard types.
// Type is promoted if it's smaller then int.

#define BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(T) \
    BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, T, (sizeof(T) < sizeof(int)))

// Same set of integral types as in boost/type_traits/is_integral.hpp.
// Please, keep in sync.
#if (defined(BOOST_MSVC) && (BOOST_MSVC < 1300)) \
    || (defined(BOOST_INTEL_CXX_VERSION) && defined(_MSC_VER) && (BOOST_INTEL_CXX_VERSION <= 600)) \
    || (defined(__BORLANDC__) && (__BORLANDC__ == 0x600) && (_MSC_VER < 1300))
// TODO: common macro for this #if. Or better yet, PP SEQ of non-standard types.
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(__int8          )
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(unsigned __int8 )
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(__int16         )
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(unsigned __int16)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(__int32         )
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(unsigned __int32)
#ifdef __BORLANDC__
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(unsigned __int64)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(         __int64)
#endif
#endif

#if defined(BOOST_HAS_LONG_LONG)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(boost::ulong_long_type)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(boost::long_long_type )
#elif defined(BOOST_HAS_MS_INT64)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(unsigned __int64)
BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE(         __int64)
#endif

#undef BOOST_TT_AUX_PROMOTE_NONSTANDARD_TYPE


#ifndef BOOST_NO_INTRINSIC_WCHAR_T
// 4.5/2
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, wchar_t, true)
#endif

// 4.5/3 (integral bit-field) is not supported.

// 4.5/4
BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(need_promotion, bool, true)


// Get promoted type by index and cv qualifiers.

template<int Index, int IsConst, int IsVolatile> struct promote_from_index;

#define BOOST_TT_AUX_PROMOTE_FROM_INDEX(N,T)                                   \
    template<> struct promote_from_index<N,0,0> { typedef T type; };           \
    template<> struct promote_from_index<N,0,1> { typedef T volatile type; };  \
    template<> struct promote_from_index<N,1,0> { typedef T const type; };     \
    template<> struct promote_from_index<N,1,1> { typedef T const volatile type; };


BOOST_TT_AUX_PROMOTE_FROM_INDEX(1, int          )
BOOST_TT_AUX_PROMOTE_FROM_INDEX(2, unsigned int )
BOOST_TT_AUX_PROMOTE_FROM_INDEX(3, long         )
BOOST_TT_AUX_PROMOTE_FROM_INDEX(4, unsigned long)


// WARNING: integral promotions to non-standard types
// long long and __int64 are not defined by the standard.
// Additional specialisations and overloads shouldn't
// introduce ambiguity, though.

#if defined(BOOST_HAS_LONG_LONG)
BOOST_TT_AUX_PROMOTE_FROM_INDEX(5, boost::long_long_type )
BOOST_TT_AUX_PROMOTE_FROM_INDEX(6, boost::ulong_long_type)
#elif defined(BOOST_HAS_MS_INT64)
BOOST_TT_AUX_PROMOTE_FROM_INDEX(7, __int64         )
BOOST_TT_AUX_PROMOTE_FROM_INDEX(8, unsigned __int64)
#endif

#undef BOOST_TT_AUX_PROMOTE_FROM_INDEX


// Define BOOST_TT_AUX_PROMOTED_INDEX_TESTER:
#if !defined(BOOST_MSVC)

template<int N>
struct sized_type_for_promotion
{
    typedef char (&type)[N];
};

#define BOOST_TT_AUX_PROMOTED_INDEX_TESTER(I,T) \
    sized_type_for_promotion<I>::type promoted_index_tester(T);

#else

#define BOOST_TT_AUX_PROMOTED_INDEX_TESTER(I,T) \
    char (&promoted_index_tester(T))[I];

#endif

BOOST_TT_AUX_PROMOTED_INDEX_TESTER(1, int          )
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(2, unsigned int )
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(3, long         )
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(4, unsigned long)

#if defined(BOOST_HAS_LONG_LONG)
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(5, boost::long_long_type )
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(6, boost::ulong_long_type)
#elif defined(BOOST_HAS_MS_INT64)
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(7, __int64         )
BOOST_TT_AUX_PROMOTED_INDEX_TESTER(8, unsigned __int64)
#endif

#undef BOOST_TT_AUX_PROMOTED_INDEX_TESTER


// Get an index of promoted type for type T.
// Precondition: need_promotion<T>
template<class T>
struct promoted_index
{
    static T testee; // undefined
    BOOST_STATIC_CONSTANT(int, value = sizeof(promoted_index_tester(+testee)) );
    // Unary plus promotes testee                    LOOK HERE ---> ^
};

template<class T>
struct integral_promotion_impl
{
    typedef BOOST_DEDUCED_TYPENAME promote_from_index<
        (boost::type_traits::detail::promoted_index<T>::value)
      , (boost::is_const<T>::value)
      , (boost::is_volatile<T>::value)
      >::type type;
};

template<class T>
struct integral_promotion
  : boost::mpl::eval_if<
        need_promotion<T>
      , integral_promotion_impl<T>
      , boost::mpl::identity<T>
      >
{
};

} }

BOOST_TT_AUX_TYPE_TRAIT_DEF1(
      integral_promotion
    , T
    , BOOST_DEDUCED_TYPENAME
        boost::type_traits::detail::integral_promotion<T>::type
    )
}

#include <boost/type_traits/detail/bool_trait_undef.hpp>
#include <boost/type_traits/detail/type_trait_undef.hpp>

#endif // #ifndef FILE_boost_type_traits_integral_promotion_hpp_INCLUDED

