#ifndef _PROJECTIONS_PROJECTION_HPP
#define _PROJECTIONS_PROJECTION_HPP

// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2007-2009, Geodan, Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <string>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>

namespace boost { namespace geometry { namespace projection
{

/*!
    \brief projection virtual base class
    \details class containing virtual methods
    \ingroup projection
    \tparam LL latlong point type
    \tparam XY xy point type
*/

template <typename LL, typename XY>
class projection
{
    protected :
        // see comment above
        //typedef typename geometry::coordinate_type<LL>::type LL_T;
        //typedef typename geometry::coordinate_type<XY>::type XY_T;
        typedef double LL_T;
        typedef double XY_T;

    public :
        /// Forward projection, from Latitude-Longitude to Cartesian
        virtual bool forward(LL const& lp, XY& xy) const = 0;

        /// Inverse projection, from Cartesian to Latitude-Longitude
        virtual bool inverse(XY const& xy, LL& lp) const = 0;

        /// Forward projection using lon / lat and x / y separately
        virtual void fwd(LL_T& lp_lon, LL_T& lp_lat, XY_T& xy_x, XY_T& xy_y) const = 0;

        /// Inverse projection using x / y and lon / lat
        virtual void inv(XY_T& xy_x, XY_T& xy_y, LL_T& lp_lon, LL_T& lp_lat) const = 0;

        /// Returns name of projection
        virtual std::string name() const = 0;

        /// Returns parameters of projection
        virtual parameters params() const = 0;

        virtual ~projection() {}

};

}}} // namespace boost::geometry::projection



#endif

