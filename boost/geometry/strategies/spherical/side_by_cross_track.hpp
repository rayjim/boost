// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_STRATEGIES_SPHERICAL_SIDE_BY_CROSS_TRACK_HPP
#define BOOST_GEOMETRY_STRATEGIES_SPHERICAL_SIDE_BY_CROSS_TRACK_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/radian_access.hpp>

#include <boost/geometry/util/select_coordinate_type.hpp>
#include <boost/geometry/util/math.hpp>

#include <boost/geometry/strategies/side.hpp>
//#include <boost/geometry/strategies/concepts/side_concept.hpp>


namespace boost { namespace geometry
{


namespace strategy { namespace side
{

/// Calculate course (bearing) between two points. Might be moved to a "course formula" ...
template <typename Point>
static inline double course(Point const& p1, Point const& p2)
{
    // http://williams.best.vwh.net/avform.htm#Crs
    double dlon = get_as_radian<0>(p2) - get_as_radian<0>(p1);
    double cos_p2lat = cos(get_as_radian<1>(p2));

    // "An alternative formula, not requiring the pre-computation of d"
    return atan2(sin(dlon) * cos_p2lat,
        cos(get_as_radian<1>(p1)) * sin(get_as_radian<1>(p2))
        - sin(get_as_radian<1>(p1)) * cos_p2lat * cos(dlon));
}




// Check at which side of a segment a point lies
// from a Great Circle segment between two points:
// left of segment (> 0), right of segment (< 0), on segment (0)
template <typename CalculationType>
struct side_by_cross_track
{

    // Types can be all three different. Therefore it is
    // not implemented (anymore) as "segment"

    template <typename P1, typename P2, typename P>
    static inline int apply(P1 const& p1, P2 const& p2, P const& p)
    {
        typedef typename boost::mpl::if_c
            <
                boost::is_void<CalculationType>::type::value,
                typename select_most_precise
                    <
                        typename select_most_precise
                            <
                                typename coordinate_type<P1>::type,
                                typename coordinate_type<P2>::type
                            >::type,
                        typename coordinate_type<P>::type
                    >::type,
                CalculationType
            >::type coordinate_type;

        // Calculate the distance using the Haversine formula.
        // That is also applicable on the spherical earth. A radius is not necessary.

        double d1 = 0.001; // m_strategy.apply(sp1, p);
        double crs_AD = course(p1, p);
        double crs_AB = course(p1, p2);
        double XTD = geometry::math::abs(asin(sin(d1) * sin(crs_AD - crs_AB)));

        return math::equals(XTD, 0) ? 0 : XTD > 0 ? 1 : -1;
        //return s > 0 ? 1 : s < 0 ? -1 : 0;
    }
};

}} // namespace strategy::side


#ifndef DOXYGEN_NO_STRATEGY_SPECIALIZATIONS
template <typename CalculationType>
struct strategy_side<spherical_tag, CalculationType>
{
    typedef strategy::side::side_by_cross_track<CalculationType> type;
};
#endif


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_SPHERICAL_SIDE_BY_CROSS_TRACK_HPP
