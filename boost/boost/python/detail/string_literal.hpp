// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef STRING_LITERAL_DWA2002629_HPP
# define STRING_LITERAL_DWA2002629_HPP

# include <cstddef>
# include <boost/type.hpp>
# include <boost/type_traits/array_traits.hpp>
# include <boost/type_traits/same_traits.hpp>

namespace boost { namespace python { namespace detail { 

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T>
struct is_string_literal
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

#  if !defined(__MWERKS__) || __MWERKS__ > 0x2407
template <std::size_t n>
struct is_string_literal<char const[n]>
{
    BOOST_STATIC_CONSTANT(bool, value = true);
};
#  else
// CWPro7 has trouble with the array type deduction above
template <class T, std::size_t n>
struct is_string_literal<T[n]>
    : is_same<T, char const>
{
};
#  endif 
# else
template <bool is_array = true>
struct string_literal_helper
{
    typedef char (&yes_string_literal)[1];
    typedef char (&no_string_literal)[2];

    template <class T>
    struct apply
    {
        typedef apply<T> self;
        static T x;
        static yes_string_literal check(char const*);
        static no_string_literal check(char*);
        static no_string_literal check(void const volatile*);
        
        BOOST_STATIC_CONSTANT(
            bool, value = sizeof(self::check(x)) == sizeof(yes_string_literal));
    };
};

template <>
struct string_literal_helper<false>
{
    template <class T>
    struct apply
    {
        BOOST_STATIC_CONSTANT(bool, value = false);
    };
};

template <class T>
struct is_string_literal
    : string_literal_helper<is_array<T>::value>::apply<T>
{
};
# endif

}}} // namespace boost::python::detail

#endif // STRING_LITERAL_DWA2002629_HPP
