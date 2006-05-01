/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary_oarchive.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <ostream>

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/array/binary_oarchive.hpp>

#include <boost/archive/impl/basic_binary_oprimitive.ipp>
#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/archive_pointer_oserializer.ipp>

namespace boost {
namespace archive {

// explicitly instantiate for this type of binary stream
template class basic_binary_oprimitive<
    array::binary_oarchive, 
    char, 
    std::char_traits<char> 
>;
template class basic_binary_oarchive<array::binary_oarchive> ;
template class binary_oarchive_impl<
    array::binary_oarchive, 
    char, 
    std::char_traits<char> 
>;
template class detail::archive_pointer_oserializer<array::binary_oarchive> ;

} // namespace archive
} // namespace boost
