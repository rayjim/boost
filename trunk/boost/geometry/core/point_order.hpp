// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_CORE_POINT_ORDER_HPP
#define BOOST_GEOMETRY_CORE_POINT_ORDER_HPP


#include <boost/mpl/assert.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/tags.hpp>

namespace boost { namespace geometry
{


enum order_selector { clockwise = 1, counterclockwise = 2, order_undetermined = 0 };

namespace traits
{

/*!
    \brief Traits class indicating the order of contained points within a
        ring or (multi)polygon, clockwise, counter clockwise or not known.
    \ingroup traits
    \par Geometries:
        - ring
    \tparam G geometry
*/
template <typename G>
struct point_order
{
    static const order_selector value = clockwise;
};


} // namespace traits


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace point_order
{

struct clockwise
{
    static const order_selector value = geometry::clockwise;
};


}} // namespace detail::point_order
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace core_dispatch
{

template <typename Tag, typename Geometry>
struct point_order
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_GEOMETRY_TYPE
            , (types<Geometry>)
        );
};

template <typename Point>
struct point_order<point_tag, Point> 
    : public detail::point_order::clockwise {};

template <typename Segment>
struct point_order<segment_tag, Segment> 
    : public detail::point_order::clockwise {};


template <typename Box>
struct point_order<box_tag, Box> 
    : public detail::point_order::clockwise {};

template <typename LineString>
struct point_order<linestring_tag, LineString>  
    : public detail::point_order::clockwise {};


template <typename Ring>
struct point_order<ring_tag, Ring>
{
    static const order_selector value = geometry::traits::point_order<Ring>::value;
};

// Specialization for polygon: the order is the order of its rings
template <typename Polygon>
struct point_order<polygon_tag, Polygon>
{
    static const order_selector value = core_dispatch::point_order
        <
            ring_tag,
            typename ring_type<polygon_tag, Polygon>::type
        >::value ;
};

} // namespace core_dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
    \brief Meta-function which defines point order of any geometry
    \ingroup core
*/
template <typename Geometry>
struct point_order
{
    static const order_selector value = core_dispatch::point_order
        <
            typename tag<Geometry>::type,
            typename boost::remove_const<Geometry>::type
        >::value;
};

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_CORE_POINT_ORDER_HPP
