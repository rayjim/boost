// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Contains constants used by library.

#ifndef BOOST_IOSTREAMS_CONSTANTS_HPP_INCLUDED
#define BOOST_IOSTREAMS_CONSTANTS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#ifndef BOOST_IOSTREAMS_DEFAULT_BUFFER_SIZE
# define BOOST_IOSTREAMS_DEFAULT_BUFFER_SIZE 4096
#endif

#ifndef BOOST_IOSTREAMS_DEFAULT_PBACK_BUFFER_SIZE
# define BOOST_IOSTREAMS_DEFAULT_PBACK_BUFFER_SIZE 4
#endif

#ifndef BOOST_IOSTREAMS_DEFAULT_FILTER_BUFFER_SIZE
# define BOOST_IOSTREAMS_DEFAULT_FILTER_BUFFER_SIZE 128
#endif

#include <boost/detail/workaround.hpp>
#include <boost/iostreams/detail/config/wide_streams.hpp>

#if !BOOST_WORKAROUND(__MWERKS__, <= 0x3003) && \
    !defined(BOOST_IOSTREAMS_NO_STREAM_TEMPLATES) \
    /**/
# include <iosfwd>
#else
# include <ios>
#endif

namespace boost { namespace iostreams {

const std::streamsize default_buffer_size = 
    BOOST_IOSTREAMS_DEFAULT_BUFFER_SIZE; 
const std::streamsize default_pback_buffer_size = 
    BOOST_IOSTREAMS_DEFAULT_PBACK_BUFFER_SIZE;
const std::streamsize default_filter_buffer_size = 
    BOOST_IOSTREAMS_DEFAULT_FILTER_BUFFER_SIZE;

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_CONSTANTS_HPP_INCLUDED
