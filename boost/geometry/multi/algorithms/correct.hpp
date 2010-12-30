// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_ALGORITHMS_CORRECT_HPP
#define BOOST_GEOMETRY_MULTI_ALGORITHMS_CORRECT_HPP


#include <boost/range/metafunctions.hpp>

#include <boost/geometry/algorithms/correct.hpp>
#include <boost/geometry/multi/algorithms/detail/modify.hpp>

#include <boost/geometry/multi/core/tags.hpp>


namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template <typename MultiPoint>
struct correct<multi_point_tag, MultiPoint> 
    : detail::correct::correct_nop<MultiPoint> 
{};


template <typename MultiLineString>
struct correct<multi_linestring_tag, MultiLineString> 
    : detail::correct::correct_nop<MultiLineString>
{};


template <typename Geometry>
struct correct<multi_polygon_tag, Geometry>
    : detail::multi_modify
        <
            Geometry,
            detail::correct::correct_polygon
                <
                    typename boost::range_value<Geometry>::type
                >
        >
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_ALGORITHMS_CORRECT_HPP
