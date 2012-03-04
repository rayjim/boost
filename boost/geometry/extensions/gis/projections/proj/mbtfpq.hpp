#ifndef BOOST_GEOMETRY_PROJECTIONS_MBTFPQ_HPP
#define BOOST_GEOMETRY_PROJECTIONS_MBTFPQ_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace mbtfpq{ 
            static const int NITER = 20;
            static const double EPS = 1e-7;
            static const double ONETOL = 1.000001;
            static const double C = 1.70710678118654752440;
            static const double RC = 0.58578643762690495119;
            static const double FYC = 1.87475828462269495505;
            static const double RYC = 0.53340209679417701685;
            static const double FXC = 0.31245971410378249250;
            static const double RXC = 3.20041258076506210122;


            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_mbtfpq_spheroid : public base_t_fi<base_mbtfpq_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_mbtfpq_spheroid(const Parameters& par)
                    : base_t_fi<base_mbtfpq_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double th1, c;
                    int i;
                
                    c = C * sin(lp_lat);
                    for (i = NITER; i; --i) {
                        lp_lat -= th1 = (sin(.5*lp_lat) + sin(lp_lat) - c) /
                            (.5*cos(.5*lp_lat)  + cos(lp_lat));
                        if (fabs(th1) < EPS) break;
                    }
                    xy_x = FXC * lp_lon * (1.0 + 2. * cos(lp_lat)/cos(0.5 * lp_lat));
                    xy_y = FYC * sin(0.5 * lp_lat);
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double t;
                
                    lp_lat = RYC * xy_y;
                    if (fabs(lp_lat) > 1.) {
                        if (fabs(lp_lat) > ONETOL)    throw proj_exception();
                        else if (lp_lat < 0.) { t = -1.; lp_lat = -PI; }
                        else { t = 1.; lp_lat = PI; }
                    } else
                        lp_lat = 2. * asin(t = lp_lat);
                    lp_lon = RXC * xy_x / (1. + 2. * cos(lp_lat)/cos(0.5 * lp_lat));
                    lp_lat = RC * (t + sin(lp_lat));
                    if (fabs(lp_lat) > 1.)
                        if (fabs(lp_lat) > ONETOL)    throw proj_exception();
                        else            lp_lat = lp_lat < 0. ? -HALFPI : HALFPI;
                    else
                        lp_lat = asin(lp_lat);
                }
            };

            // McBryde-Thomas Flat-Polar Quartic
            template <typename Parameters>
            void setup_mbtfpq(Parameters& par)
            {
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }

        }} // namespace detail::mbtfpq
    #endif // doxygen 

    /*!
        \brief McBryde-Thomas Flat-Polar Quartic projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
        \par Example
        \image html ex_mbtfpq.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct mbtfpq_spheroid : public detail::mbtfpq::base_mbtfpq_spheroid<Geographic, Cartesian, Parameters>
    {
        inline mbtfpq_spheroid(const Parameters& par) : detail::mbtfpq::base_mbtfpq_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::mbtfpq::setup_mbtfpq(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class mbtfpq_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<mbtfpq_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void mbtfpq_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("mbtfpq", new mbtfpq_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_MBTFPQ_HPP

