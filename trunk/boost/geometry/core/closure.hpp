// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2011 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2011 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2011 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_CORE_CLOSURE_HPP
#define BOOST_GEOMETRY_CORE_CLOSURE_HPP


#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/core/tag.hpp>
#include <boost/geometry/core/tags.hpp>

namespace boost { namespace geometry
{


/*!
\brief Enumerates options for defining if polygons are open or closed
\ingroup enum
\details The enumeration closure_selector describes options for if a polygon is
    open or closed. In a closed polygon the very first point (per ring) should be
    equal to the very last point.
    The specific closing property of a polygon type is defined by the closure metafunction.
    The closure metafunction defines a value, which is one of the values enumerated
    in the closure_selector

\qbk{
[heading See also]
[link geometry.reference.core.closure The closure metafunction]
}
*/
enum closure_selector
{
    open = 0,
    closed = 1,
    closure_undertermined = -1
};

namespace traits
{

/*!
    \brief Traits class indicating if points within a
        ring or (multi)polygon are closed (last point == first point),
        open or not known.
    \ingroup traits
    \par Geometries:
        - ring
    \tparam G geometry
*/
template <typename G>
struct closure
{
    static const closure_selector value = closed;
};


} // namespace traits


#ifndef DOXYGEN_NO_DETAIL
namespace core_detail { namespace closure
{

struct closed
{
    static const closure_selector value = geometry::closed;
};


/// Metafunction to define the minimum size of a ring:
/// 3 for open rings, 4 for closed rings
template <closure_selector Closure>
struct minimum_ring_size {};

template <>
struct minimum_ring_size<geometry::closed> : boost::mpl::int_<4> {};

template <>
struct minimum_ring_size<geometry::open> : boost::mpl::int_<3> {};


}} // namespace detail::point_order
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace core_dispatch
{

template <typename Tag, typename Geometry>
struct closure
{
    BOOST_MPL_ASSERT_MSG
        (
            false, NOT_IMPLEMENTED_FOR_THIS_GEOMETRY_TYPE
            , (types<Geometry>)
        );
};

template <typename Box>
struct closure<point_tag, Box> : public core_detail::closure::closed {};

template <typename Box>
struct closure<box_tag, Box> : public core_detail::closure::closed {};

template <typename Box>
struct closure<segment_tag, Box> : public core_detail::closure::closed {};

template <typename LineString>
struct closure<linestring_tag, LineString> : public core_detail::closure::closed {};


template <typename Ring>
struct closure<ring_tag, Ring>
{
    static const closure_selector value = geometry::traits::closure<Ring>::value;
};

// Specialization for polygon: the closure is the closure of its rings
template <typename Polygon>
struct closure<polygon_tag, Polygon>
{
    static const closure_selector value = core_dispatch::closure
        <
            ring_tag,
            typename ring_type<polygon_tag, Polygon>::type
        >::value ;
};


} // namespace core_dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
\brief Meta-function which defines closure of a geometry type
\ingroup core
\details

\qbk{
[heading See also]
[link geometry.reference.enumerations.order_selector The order_selector enumeration]
}
*/
template <typename Geometry>
struct closure
{
    static const closure_selector value = core_dispatch::closure
        <
            typename tag<Geometry>::type,
            typename boost::remove_const<Geometry>::type
        >::value;
};


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_CORE_CLOSURE_HPP
