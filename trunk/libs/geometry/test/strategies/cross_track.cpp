// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2011 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2011 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2011 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>


#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/algorithms/distance.hpp>

#include <boost/geometry/strategies/spherical/distance_haversine.hpp>
#include <boost/geometry/strategies/spherical/distance_cross_track.hpp>

#include <boost/geometry/strategies/concepts/distance_concept.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/segment.hpp>




template <typename Point>
void test_distance(
            typename bg::coordinate_type<Point>::type const& lon1, 
            typename bg::coordinate_type<Point>::type const& lat1,
            typename bg::coordinate_type<Point>::type const& lon2, 
            typename bg::coordinate_type<Point>::type const& lat2,
            typename bg::coordinate_type<Point>::type const& lon3, 
            typename bg::coordinate_type<Point>::type const& lat3,
            typename bg::coordinate_type<Point>::type const& radius, 
            typename bg::coordinate_type<Point>::type const& expected, 
            typename bg::coordinate_type<Point>::type const& tolerance)
{
    typedef bg::strategy::distance::cross_track
        <
            Point,
            Point
        > strategy_type;
    typedef typename bg::strategy::distance::services::return_type
        <
            strategy_type
        >::type return_type;


    BOOST_CONCEPT_ASSERT
        (
            (bg::concept::PointSegmentDistanceStrategy<strategy_type>)
        );


    Point p1, p2, p3;
    bg::assign(p1, lon1, lat1);
    bg::assign(p2, lon2, lat2);
    bg::assign(p3, lon3, lat3);


    strategy_type strategy;
    return_type d = strategy.apply(p1, p2, p3);

    BOOST_CHECK_CLOSE(radius * d, expected, tolerance);

    // Test specifying radius explicitly
    strategy_type strategy_radius(radius);
    d = strategy_radius.apply(p1, p2, p3);
    BOOST_CHECK_CLOSE(d, expected, tolerance);


    // Test the "default strategy" registration
    bg::model::referring_segment<Point const> segment(p2, p3);
    d = bg::distance(p1, segment);
    BOOST_CHECK_CLOSE(radius * d, expected, tolerance);
}



template <typename Point>
void test_all()
{
    typename bg::coordinate_type<Point>::type const average_earth_radius = 6372795.0;

    // distance (Paris <-> Amsterdam/Barcelona), 
    // with coordinates rounded as below ~87 km
    // should be is equal
    // to distance (Paris <-> Barcelona/Amsterdam)
    typename bg::coordinate_type<Point>::type const p_to_ab = 86.798321 * 1000.0;
    test_distance<Point>(2, 48, 4, 52, 2, 41, average_earth_radius, p_to_ab, 0.1);
    test_distance<Point>(2, 48, 2, 41, 4, 52, average_earth_radius, p_to_ab, 0.1);
}


int test_main(int, char* [])
{
    test_all<bg::model::point<double, 2, bg::cs::spherical<bg::degree> > >();

#if defined(HAVE_TTMATH)
    typedef ttmath::Big<1,4> tt;
    //test_all<bg::model::point<tt, 2, bg::cs::spherical<bg::degree> > >();
#endif

    return 0;
}
