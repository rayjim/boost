// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PARAMETER_NAME_060806_HPP
# define BOOST_PARAMETER_NAME_060806_HPP

# include <boost/parameter/keyword.hpp>
# include <boost/parameter/value_type.hpp>
# include <boost/detail/workaround.hpp>
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/stringize.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/tuple/eat.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/mpl/placeholders.hpp>

# if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
# include <boost/preprocessor/detail/split.hpp>
// From Paul Mensonides
#  define BOOST_PARAMETER_IS_BINARY(x) \
    BOOST_PP_SPLIT(1, BOOST_PARAMETER_IS_BINARY_C x BOOST_PP_COMMA() 0) \
    /**/
#  define BOOST_PARAMETER_IS_BINARY_C(x,y) \
    ~, 1 BOOST_PP_RPAREN() \
    BOOST_PP_TUPLE_EAT(2) BOOST_PP_LPAREN() ~ \
    /**/
# else
#  include <boost/preprocessor/detail/is_binary.hpp>
#  define BOOST_PARAMETER_IS_BINARY(x) BOOST_PP_IS_BINARY(x)
# endif

# if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
#  define BOOST_PARAMETER_NAME_OBJECT(tag, name)                    \
    static ::boost::parameter::keyword<tag>& name                   \
       = ::boost::parameter::keyword<tag>::get();
# else
#  define BOOST_PARAMETER_NAME_OBJECT(tag, name)                    \
    namespace                                                       \
    {                                                               \
       ::boost::parameter::keyword<tag>& name                       \
       = ::boost::parameter::keyword<tag>::get();                   \
    }
# endif

# define BOOST_PARAMETER_BASIC_NAME(tag_namespace, tag, name)       \
    namespace tag_namespace                                         \
    {                                                               \
      struct tag                                                    \
      {                                                             \
          static char const* keyword_name()                         \
          {                                                         \
              return BOOST_PP_STRINGIZE(tag);                       \
          }                                                         \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _;                                                      \
                                                                    \
          typedef boost::parameter::value_type<                     \
              boost::mpl::_2, tag, boost::parameter::void_          \
          > _1;                                                     \
      };                                                            \
    }                                                               \
    BOOST_PARAMETER_NAME_OBJECT(tag_namespace::tag, name)

# define BOOST_PARAMETER_COMPLEX_NAME_TUPLE1(tag,namespace)         \
    (tag, namespace), ~

# define BOOST_PARAMETER_COMPLEX_NAME_TUPLE(name)                   \
    BOOST_PP_TUPLE_ELEM(2, 0, (BOOST_PARAMETER_COMPLEX_NAME_TUPLE1 name))

# define BOOST_PARAMETER_COMPLEX_NAME_TAG(name)                     \
    BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PARAMETER_COMPLEX_NAME_TUPLE(name))

# define BOOST_PARAMETER_COMPLEX_NAME_NAMESPACE(name)               \
    BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PARAMETER_COMPLEX_NAME_TUPLE(name))

# define BOOST_PARAMETER_COMPLEX_NAME(name)                         \
    BOOST_PARAMETER_BASIC_NAME(                                     \
        BOOST_PARAMETER_COMPLEX_NAME_NAMESPACE(name)                \
      , BOOST_PP_TUPLE_EAT(2) name                                  \
      , BOOST_PARAMETER_COMPLEX_NAME_TAG(name)                      \
    )                                                               \
/**/

# define BOOST_PARAMETER_SIMPLE_NAME(name)                          \
    BOOST_PARAMETER_BASIC_NAME(tag, name, BOOST_PP_CAT(_, name))

# define BOOST_PARAMETER_NAME(name)                                 \
    BOOST_PP_IIF(                                                   \
        BOOST_PARAMETER_IS_BINARY(name)                             \
      , BOOST_PARAMETER_COMPLEX_NAME                                \
      , BOOST_PARAMETER_SIMPLE_NAME                                 \
    )(name)                                                         \
/**/


# define BOOST_PARAMETER_TEMPLATE_KEYWORD(name)                     \
    namespace tag                                                   \
    {                                                               \
      struct name;                                                  \
    }                                                               \
    template <class T>                                              \
    struct name                                                     \
      : boost::parameter::template_keyword<tag::name, T>            \
    {};                                                             \
/**/

#endif // BOOST_PARAMETER_NAME_060806_HPP

