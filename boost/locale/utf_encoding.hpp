//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_LOCALE_ENCODING_HPP_INCLUDED
#define BOOST_LOCALE_ENCODING_HPP_INCLUDED

#include <boost/locale/utf.hpp>
#include <boost/locale/encoding_erros.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4275 4251 4231 4660)
#endif


namespace boost {
    namespace locale {
        namespace conv {
            ///
            /// \addtogroup codepage Character conversion functions
            ///
            /// @{

            ///
            /// Convert a Unicode text in range [begin,end) to other Unicode encoding
            ///
            /// This function does not require linking with Boost.Locale library
            ///
            template<typename CharOut,typename CharIn>
            std::basic_string<CharOut>
            utf_to_utf(CharIn const *begin,CharIn const *end,method_type how = default_method)
            {
                std::basic_string<CharOut> result;
                result.reserve(end-begin);
                std::back_insert_iterator<std::basic_string<CharOut> > inserter(result);
                utf::code_point c;
                while(begin!=end) {
                    c=utf::utf_traits<CharIn>::template decode(begin,end);
                    if(c==utf::illegal || c==utf::incomplete) {
                        if(how==stop)
                            throw conversion_error();
                    }
                    else {
                        utf::utf_traits<CharOut>::template encode(c,inserter);
                    }
                }
                return result;
            }

            ///
            /// Convert a Unicode NUL terminated string \a str other Unicode encoding
            ///
            /// This function does not require linking with Boost.Locale library
            ///
            template<typename CharOut,typename CharIn>
            std::basic_string<CharOut>
            utf_to_utf(CharIn const *str,method_type how = default_method)
            {
                CharIn const *end = str;
                while(*end)
                    end++;
                return utf_to_utf<CharOut,CharIn>(str,end,how);
            }


            ///
            /// Convert a Unicode string \a str other Unicode encoding
            ///
            /// This function does not require linking with Boost.Locale library
            ///
            template<typename CharOut,typename CharIn>
            std::basic_string<CharOut>
            utf_to_utf(std::basic_string<CharIn> const &str,method_type how = default_method)
            {
                return utf_to_utf<CharOut,CharIn>(str.c_str(),str.c_str()+str.size(),how);
            }


            /// @}

        } // conv

    } // locale
} // boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif

// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

