// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Inspired by Daryle Walker's nullbuf from his More I/O submission.

#ifndef BOOST_IOSTREAMS_NULL_HPP_INCLUDED
#define BOOST_IOSTREAMS_NULL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <iosfwd>                              // streamsize.
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/openmode.hpp> // openmodes.

namespace boost { namespace iostreams {

template<typename Ch, typename Mode>
class basic_null_device {
public:
    typedef Ch char_type;
    struct io_category
        : public Mode,
          public device_tag,
          public closable_tag
        { };
    std::streamsize read(Ch*, std::streamsize) { return 0; }
    void write(const Ch*, std::streamsize) { }
    std::streamoff seek( std::streamoff, std::ios::seekdir,
                         std::ios::openmode = 
                             std::ios::in | std::ios::out ) 
    { return -1; }
    void close(std::ios::openmode = std::ios::in | std::ios::out) { }
};

template<typename Ch>
struct basic_null_source : private basic_null_device<Ch, input> {
    using basic_null_device<Ch, input>::read;
    using basic_null_device<Ch, input>::close;
};

typedef basic_null_source<char>     null_source;
typedef basic_null_source<wchar_t>  wnull_source;

template<typename Ch>
struct basic_null_sink : private basic_null_device<Ch, output> {
    using basic_null_device<Ch, output>::write;
    using basic_null_device<Ch, output>::close;
};

typedef basic_null_sink<char>     null_sink;
typedef basic_null_sink<wchar_t>  wnull_sink;

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_NULL_HPP_INCLUDED
