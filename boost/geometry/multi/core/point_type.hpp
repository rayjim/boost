// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Bruno Lalande 2008, 2009
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_GEOMETRY_MULTI_CORE_POINT_TYPE_HPP
#define BOOST_GEOMETRY_MULTI_CORE_POINT_TYPE_HPP


#include <boost/range/metafunctions.hpp>

#include <boost/geometry/core/point_type.hpp>
#include <boost/geometry/multi/core/tags.hpp>



namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace core_dispatch
{

template <typename MultiPoint>
struct point_type<multi_point_tag, MultiPoint>
{
    typedef typename boost::range_value<MultiPoint>::type type;
};


template <typename MultiLinestring>
struct point_type<multi_linestring_tag, MultiLinestring>
{
    typedef typename point_type<linestring_tag,
        typename boost::range_value<MultiLinestring>::type>::type type;
};



template <typename MultiPolygon>
struct point_type<multi_polygon_tag, MultiPolygon>
{
    typedef typename point_type<polygon_tag,
        typename boost::range_value<MultiPolygon>::type>::type type;
};


}
#endif


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_CORE_POINT_TYPE_HPP
