#ifndef BOOST_GEOMETRY_PROJECTIONS_LAGRNG_HPP
#define BOOST_GEOMETRY_PROJECTIONS_LAGRNG_HPP

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
    namespace detail { namespace lagrng{ 
            static const double TOL = 1e-10;

            struct par_lagrng
            {
                double    hrw;
                double    rw;
                double    a1;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_lagrng_spheroid : public base_t_f<base_lagrng_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_lagrng m_proj_parm;

                inline base_lagrng_spheroid(const Parameters& par)
                    : base_t_f<base_lagrng_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double v, c;
                
                    if (fabs(fabs(lp_lat) - HALFPI) < TOL) {
                        xy_x = 0;
                        xy_y = lp_lat < 0 ? -2. : 2.;
                    } else {
                        lp_lat = sin(lp_lat);
                        v = this->m_proj_parm.a1 * pow((1. + lp_lat)/(1. - lp_lat), this->m_proj_parm.hrw);
                        if ((c = 0.5 * (v + 1./v) + cos(lp_lon *= this->m_proj_parm.rw)) < TOL)
                            throw proj_exception();;
                        xy_x = 2. * sin(lp_lon) / c;
                        xy_y = (v - 1./v) / c;
                    }
                }
            };

            // Lagrange
            template <typename Parameters>
            void setup_lagrng(Parameters& par, par_lagrng& proj_parm)
            {
                double phi1;
                if ((proj_parm.rw = pj_param(par.params, "dW").f) <= 0) throw proj_exception(-27);
                proj_parm.hrw = 0.5 * (proj_parm.rw = 1. / proj_parm.rw);
                phi1 = pj_param(par.params, "rlat_1").f;
                if (fabs(fabs(phi1 = sin(phi1)) - 1.) < TOL) throw proj_exception(-22);
                proj_parm.a1 = pow((1. - phi1)/(1. + phi1), proj_parm.hrw);
                par.es = 0.;
                // par.fwd = s_forward;
            }

        }} // namespace detail::lagrng
    #endif // doxygen 

    /*!
        \brief Lagrange projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
         - no inverse
         - W=
        \par Example
        \image html ex_lagrng.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct lagrng_spheroid : public detail::lagrng::base_lagrng_spheroid<Geographic, Cartesian, Parameters>
    {
        inline lagrng_spheroid(const Parameters& par) : detail::lagrng::base_lagrng_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::lagrng::setup_lagrng(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class lagrng_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_f<lagrng_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void lagrng_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("lagrng", new lagrng_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_LAGRNG_HPP

