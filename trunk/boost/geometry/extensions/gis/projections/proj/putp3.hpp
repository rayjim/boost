#ifndef BOOST_GEOMETRY_PROJECTIONS_PUTP3_HPP
#define BOOST_GEOMETRY_PROJECTIONS_PUTP3_HPP

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

#include <boost/concept_check.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace putp3{ 
            static const double C = 0.79788456;
            static const double RPISQ = 0.1013211836;

            struct par_putp3
            {
                double    A;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_putp3_spheroid : public base_t_fi<base_putp3_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_putp3 m_proj_parm;

                inline base_putp3_spheroid(const Parameters& par)
                    : base_t_fi<base_putp3_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    xy_x = C * lp_lon * (1. - this->m_proj_parm.A * lp_lat * lp_lat);
                    xy_y = C * lp_lat;
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    lp_lat = xy_y / C;
                    lp_lon = xy_x / (C * (1. - this->m_proj_parm.A * lp_lat * lp_lat));
                }
            };

            template <typename Parameters>
            void setup(Parameters& par, par_putp3& proj_parm) 
            {
                boost::ignore_unused_variable_warning(par);
                boost::ignore_unused_variable_warning(proj_parm);
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }


            // Putnins P3
            template <typename Parameters>
            void setup_putp3(Parameters& par, par_putp3& proj_parm)
            {
                proj_parm.A = 4. * RPISQ;
                setup(par, proj_parm);
            }

            // Putnins P3'
            template <typename Parameters>
            void setup_putp3p(Parameters& par, par_putp3& proj_parm)
            {
                proj_parm.A = 2. * RPISQ;
                setup(par, proj_parm);
            }

        }} // namespace detail::putp3
    #endif // doxygen 

    /*!
        \brief Putnins P3 projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_putp3.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct putp3_spheroid : public detail::putp3::base_putp3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline putp3_spheroid(const Parameters& par) : detail::putp3::base_putp3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::putp3::setup_putp3(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Putnins P3' projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - no inverse
         - Spheroid
        \par Example
        \image html ex_putp3p.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct putp3p_spheroid : public detail::putp3::base_putp3_spheroid<Geographic, Cartesian, Parameters>
    {
        inline putp3p_spheroid(const Parameters& par) : detail::putp3::base_putp3_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::putp3::setup_putp3p(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class putp3_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<putp3_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class putp3p_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<putp3p_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void putp3_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("putp3", new putp3_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("putp3p", new putp3p_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_PUTP3_HPP

