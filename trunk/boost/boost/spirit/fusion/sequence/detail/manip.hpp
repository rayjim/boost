/*=============================================================================
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 1999-2003 Jaakko J�rvi
    Copyright (c) 2001-2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef FUSION_SEQUENCE_DETAIL_MANIP_HPP
#define FUSION_SEQUENCE_DETAIL_MANIP_HPP

#include <boost/config.hpp>
#include <iostream>
#include <string>
#include <vector>

// Tuple I/O manipulators

#include <boost/spirit/fusion/detail/config.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
FUSION_MSVC_ETI_WRAPPER(char_type)
FUSION_MSVC_ETI_WRAPPER(traits_type)
# define FUSION_GET_CHAR_TYPE(T) get_char_type<T>::type
# define FUSION_GET_TRAITS_TYPE(T) get_traits_type<T>::type
#else
# define FUSION_GET_CHAR_TYPE(T) typename T::char_type
# define FUSION_GET_TRAITS_TYPE(T) typename T::traits_type
#endif

#if defined (BOOST_NO_TEMPLATED_STREAMS)
#define FUSION_STRING_OF_STREAM(Stream) std::string
#else
#define FUSION_STRING_OF_STREAM(Stream)                                         \
    std::basic_string<                                                          \
        FUSION_GET_CHAR_TYPE(Stream)                                            \
      , FUSION_GET_TRAITS_TYPE(Stream)                                          \
    >
#endif

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Tag>
        int get_xalloc_index(Tag* = 0)
        {
            // each Tag will have a unique index
            static int index = std::ios::xalloc();
            return index;
        }

        template <typename Stream, typename Tag, typename T>
        struct stream_data
        {
            struct arena
            {
                ~arena()
                {
                    for (
                        typename std::vector<T*>::iterator i = data.begin()
                      ; i != data.end()
                      ; ++i)
                    {
                        delete *i;
                    }
                }

                std::vector<T*> data;
            };

            static void attach(Stream& stream, T const& data)
            {
                static arena ar; // our arena
                ar.data.push_back(new T(data));
                stream.pword(get_xalloc_index<Tag>()) = ar.data.back();
            }

            static T const* get(Stream& stream)
            {
                return (T const*)stream.pword(get_xalloc_index<Tag>());
            }
        };

        template <class Tag, class Stream>
        class string_ios_manip
        {
        public:

            typedef FUSION_STRING_OF_STREAM(Stream) string_type;

            typedef stream_data<Stream, Tag, string_type> stream_data_t;

            string_ios_manip(Stream& str_)
                : stream(str_)
            {}

            void
            set(string_type const& s)
            {
                stream_data_t::attach(stream, s);
            }

            template <typename Default>
            void
            print(Default const& default_) const
            {
                string_type const* p = stream_data_t::get(stream);
                if (p)
                    stream << *p;
                else
                    stream << default_;
            }

        private:

            Stream& stream;
        };

    } // detail

#if defined (BOOST_NO_TEMPLATED_STREAMS)
#define STD_TUPLE_DEFINE_MANIPULATOR(name)                                      \
    namespace detail                                                            \
    {                                                                           \
        struct name##_tag;                                                      \
                                                                                \
        struct name##_type                                                      \
        {                                                                       \
            typedef std::string string_type;                                    \
            string_type data;                                                   \
            name##_type(const string_type& d): data(d) {}                       \
        };                                                                      \
                                                                                \
        template <class Stream>                                                 \
        Stream& operator>>(Stream& s, const name##_type& m)                     \
        {                                                                       \
            string_ios_manip<name##_tag, Stream>(s).set(m.data);                \
            return s;                                                           \
        }                                                                       \
                                                                                \
        template <class Stream>                                                 \
        Stream& operator<<(Stream& s, const name##_type& m)                     \
        {                                                                       \
            string_ios_manip<name##_tag, Stream>(s).set(m.data);                \
            return s;                                                           \
        }                                                                       \
    }                                                                           \
                                                                                \
    detail::name##_type                                                         \
    name(const std::string& s)                                                  \
    {                                                                           \
        return detail::name##_type(s);                                          \
    }                                                                           \
                                                                                \
    detail::name##_type                                                         \
    name(const char c[])                                                        \
    {                                                                           \
        return detail::name##_type(std::string(c));                             \
    }                                                                           \
                                                                                \
    detail::name##_type                                                         \
    name(char c)                                                                \
    {                                                                           \
        return detail::name##_type(std::string(1, c));                          \
    }
#else
#define STD_TUPLE_DEFINE_MANIPULATOR(name)                                      \
    namespace detail                                                            \
    {                                                                           \
        struct name##_tag;                                                      \
                                                                                \
        template <class Char, class Traits = std::char_traits<Char> >           \
        struct name##_type                                                      \
        {                                                                       \
            typedef std::basic_string<Char, Traits> string_type;                \
            string_type data;                                                   \
            name##_type(const string_type& d): data(d) {}                       \
        };                                                                      \
                                                                                \
        template <class Stream, class Char, class Traits>                       \
        Stream& operator>>(Stream& s, const name##_type<Char,Traits>& m)        \
        {                                                                       \
            string_ios_manip<name##_tag, Stream>(s).set(m.data);                \
            return s;                                                           \
        }                                                                       \
                                                                                \
        template <class Stream, class Char, class Traits>                       \
        Stream& operator<<(Stream& s, const name##_type<Char,Traits>& m)        \
        {                                                                       \
            string_ios_manip<name##_tag, Stream>(s).set(m.data);                \
            return s;                                                           \
        }                                                                       \
    }                                                                           \
                                                                                \
    template <class Char, class Traits>                                         \
    detail::name##_type<Char, Traits>                                           \
    name(const std::basic_string<Char, Traits>& s)                              \
    {                                                                           \
        return detail::name##_type<Char, Traits>(s);                            \
    }                                                                           \
                                                                                \
    template <class Char>                                                       \
    detail::name##_type<Char>                                                   \
    name(const Char c[])                                                        \
    {                                                                           \
        return detail::name##_type<Char>(std::basic_string<Char>(c));           \
    }                                                                           \
                                                                                \
    template <class Char>                                                       \
    detail::name##_type<Char>                                                   \
    name(Char c)                                                                \
    {                                                                           \
        return detail::name##_type<Char>(std::basic_string<Char>(1, c));        \
    }
#endif

    STD_TUPLE_DEFINE_MANIPULATOR(tuple_open)
    STD_TUPLE_DEFINE_MANIPULATOR(tuple_close)
    STD_TUPLE_DEFINE_MANIPULATOR(tuple_delimiter)

#undef STD_TUPLE_DEFINE_MANIPULATOR
#undef FUSION_STRING_OF_STREAM
#undef FUSION_GET_CHAR_TYPE
#undef FUSION_GET_TRAITS_TYPE

}}

#endif
