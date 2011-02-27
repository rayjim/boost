// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels, Geodan B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/unique.hpp>
#include <boost/geometry/multi/algorithms/unique.hpp>

#include <boost/geometry/extensions/gis/io/wkt/wkt.hpp>

#include <boost/geometry/geometries/geometries.hpp>

#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>

#include <algorithms/test_unique.hpp>


template <typename P>
void test_all()
{
    // Multi point, should happen nothing, even if there are duplicate points
    test_geometry<bg::model::multi_point<P> >(
        "MULTIPOINT((0 0),(0 0),(1 1))",
        "MULTIPOINT((0 0),(0 0),(1 1))");

    test_geometry<bg::model::multi_linestring<bg::model::linestring<P> > >(
        "MULTILINESTRING((0 0,1 1,1 1),(3 3,3 3,4 4))",
        "MULTILINESTRING((0 0,1 1),(3 3,4 4))");

    typedef bg::model::multi_polygon<bg::model::polygon<P> > mp;
    test_geometry<mp>(
        "MULTIPOLYGON(((0 0,0 1,1 1,1 1,1 1,1 0,0 0,0 0)))",
        "MULTIPOLYGON(((0 0,0 1,1 1,1 0,0 0)))");

    // With holes
    test_geometry<mp>(
        "MULTIPOLYGON(((0 0,0 10,10 10,10 10,10 10,10 0,0 0,0 0)))",
        "MULTIPOLYGON(((0 0,0 10,10 10,10 0,0 0)))");

}

int test_main( int , char* [] )
{
    test_all<bg::model::d2::point_xy<int> >();
    test_all<bg::model::d2::point_xy<double> >();

#ifdef HAVE_TTMATH
    test_all<bg::model::d2::point_xy<ttmath_big> >();
#endif

    return 0;
}
