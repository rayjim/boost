// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_FOR_EACH_HPP
#define BOOST_GEOMETRY_ALGORITHMS_FOR_EACH_HPP


#include <algorithm>

#include <boost/range.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/geometry/core/exterior_ring.hpp>
#include <boost/geometry/core/interior_rings.hpp>
#include <boost/geometry/core/tag_cast.hpp>

#include <boost/geometry/geometries/concepts/check.hpp>

#include <boost/geometry/geometries/segment.hpp>

#include <boost/geometry/util/add_const_if_c.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace for_each
{


template <typename Point, typename Functor>
struct fe_point_per_point
{
    static inline Functor apply(Point& point, Functor f)
    {
        f(point);
        return f;
    }
};


template <typename Point, typename Functor>
struct fe_point_per_segment
{
    static inline Functor apply(Point& , Functor f)
    {
        // TODO: if non-const, we should extract the points from the segment
        // and call the functor on those two points
        return f;
    }
};


template <typename Range, typename Functor>
struct fe_range_per_point
{
    static inline Functor apply(Range& range, Functor f)
    {
        return (std::for_each(boost::begin(range), boost::end(range), f));
    }
};


template <typename Range, typename Functor>
struct fe_range_per_segment
{
    static inline Functor apply(Range& range, Functor f)
    {
        typedef typename add_const_if_c
            <
                is_const<Range>::value,
                typename point_type<Range>::type
            >::type point_type;

        BOOST_AUTO_TPL(it, boost::begin(range));
        BOOST_AUTO_TPL(previous, it++);
        while(it != boost::end(range))
        {
            model::referring_segment<point_type> s(*previous, *it);
            f(s);
            previous = it++;
        }

        return f;
    }
};


template <typename Polygon, typename Functor>
struct fe_polygon_per_point
{
    static inline Functor apply(Polygon& poly, Functor f)
    {
        typedef fe_range_per_point
                <
                    typename ring_type<Polygon>::type,
                    Functor
                > per_ring;

        f = per_ring::apply(exterior_ring(poly), f);

        typename interior_return_type<Polygon>::type rings
                    = interior_rings(poly);
        for (BOOST_AUTO_TPL(it, boost::begin(rings)); it != boost::end(rings); ++it)
        {
            f = per_ring::apply(*it, f);
        }

        return f;
    }

};


template <typename Polygon, typename Functor>
struct fe_polygon_per_segment
{
    static inline Functor apply(Polygon& poly, Functor f)
    {
        typedef fe_range_per_segment
            <
                typename ring_type<Polygon>::type,
                Functor
            > per_ring;

        f = per_ring::apply(exterior_ring(poly), f);

        typename interior_return_type<Polygon>::type rings
                    = interior_rings(poly);
        for (BOOST_AUTO_TPL(it, boost::begin(rings)); it != boost::end(rings); ++it)
        {
            f = per_ring::apply(*it, f);
        }

        return f;
    }

};


}} // namespace detail::for_each
#endif // DOXYGEN_NO_DETAIL


#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Tag,
    typename Geometry,
    typename Functor
>
struct for_each_point {};


template <typename Point, typename Functor>
struct for_each_point<point_tag, Point, Functor>
    : detail::for_each::fe_point_per_point<Point, Functor>
{};


template <typename Linestring, typename Functor>
struct for_each_point<linestring_tag, Linestring, Functor>
    : detail::for_each::fe_range_per_point<Linestring, Functor>
{};


template <typename Ring, typename Functor>
struct for_each_point<ring_tag, Ring, Functor>
    : detail::for_each::fe_range_per_point<Ring, Functor>
{};


template <typename Polygon, typename Functor>
struct for_each_point<polygon_tag, Polygon, Functor>
    : detail::for_each::fe_polygon_per_point<Polygon, Functor>
{};


template
<
    typename Tag,
    typename Geometry,
    typename Functor
>
struct for_each_segment {};

template <typename Point, typename Functor>
struct for_each_segment<point_tag, Point, Functor>
    : detail::for_each::fe_point_per_segment<Point, Functor>
{};


template <typename Linestring, typename Functor>
struct for_each_segment<linestring_tag, Linestring, Functor>
    : detail::for_each::fe_range_per_segment<Linestring, Functor>
{};


template <typename Ring, typename Functor>
struct for_each_segment<ring_tag, Ring, Functor>
    : detail::for_each::fe_range_per_segment<Ring, Functor>
{};


template <typename Polygon, typename Functor>
struct for_each_segment<polygon_tag, Polygon, Functor>
    : detail::for_each::fe_polygon_per_segment<Polygon, Functor>
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


/*!
\brief \brf_for_each{point}
\details \det_for_each{point}
\ingroup for_each
\param geometry \param_geometry
\param f \par_for_each_f{point}
\tparam Geometry \tparam_geometry
\tparam Functor \tparam_functor

\qbk{[include reference/algorithms/for_each_point.qbk]}
\qbk{[heading Example]}
\qbk{[for_each_point] [for_each_point_output]}
\qbk{[for_each_point_const] [for_each_point_const_output]}
*/
template<typename Geometry, typename Functor>
inline Functor for_each_point(Geometry& geometry, Functor f)
{
    concept::check<Geometry>();

    return dispatch::for_each_point
        <
            typename tag_cast<typename tag<Geometry>::type, multi_tag>::type,
            Geometry,
            Functor
        >::apply(geometry, f);
}


/*!
\brief \brf_for_each{segment}
\details \det_for_each{segment}
\ingroup for_each
\param geometry \param_geometry
\param f \par_for_each_f{segment}
\tparam Geometry \tparam_geometry
\tparam Functor \tparam_functor

\qbk{[include reference/algorithms/for_each_segment.qbk]}
\qbk{[heading Example]}
\qbk{[for_each_segment_const] [for_each_segment_const_output]}
*/
template<typename Geometry, typename Functor>
inline Functor for_each_segment(Geometry& geometry, Functor f)
{
    concept::check<Geometry>();

    return dispatch::for_each_segment
        <
            typename tag_cast<typename tag<Geometry>::type, multi_tag>::type,
            Geometry,
            Functor
        >::apply(geometry, f);
}


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_FOR_EACH_HPP
