/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// serializer_map.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#  pragma warning (disable : 4786) // too long name, harmless warning
#endif

#include <set>

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/detail/basic_serializer.hpp>
#include <boost/archive/detail/basic_serializer_map.hpp>

namespace boost {
    namespace serialization {
        class extended_type_info;
    }
namespace archive {
namespace detail {

bool  
basic_serializer_map::type_info_pointer_compare::operator()(
    const basic_serializer * lhs, const basic_serializer * rhs
) const {
    return *lhs < *rhs;
}

BOOST_ARCHIVE_DECL(bool) 
basic_serializer_map::insert(const basic_serializer * bs){
    return m_map.insert(bs).second;
}

BOOST_ARCHIVE_DECL(void) 
basic_serializer_map::erase(basic_serializer * bs){
    map_type::iterator it;
    it = m_map.find(bs);
    assert(it != m_map.end());
    if(*it == bs)
        m_map.erase(it);
}

BOOST_ARCHIVE_DECL(const basic_serializer *) 
basic_serializer_map::find(
    const boost::serialization::extended_type_info & eti
) const {
    const basic_serializer_arg bs(eti);
    map_type::const_iterator it;
    it = m_map.find(& bs);
    assert(it != m_map.end());
    return *it;
}

} // namespace detail
} // namespace archive
} // namespace boost
