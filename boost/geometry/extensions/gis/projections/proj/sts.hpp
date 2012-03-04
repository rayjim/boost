#ifndef BOOST_GEOMETRY_PROJECTIONS_STS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_STS_HPP

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
    namespace detail { namespace sts{ 

            struct par_sts
            {
                double C_x, C_y, C_p;
                int tan_mode;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_sts_spheroid : public base_t_fi<base_sts_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_sts m_proj_parm;

                inline base_sts_spheroid(const Parameters& par)
                    : base_t_fi<base_sts_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double c;
                
                    xy_x = this->m_proj_parm.C_x * lp_lon * cos(lp_lat);
                    xy_y = this->m_proj_parm.C_y;
                    lp_lat *= this->m_proj_parm.C_p;
                    c = cos(lp_lat);
                    if (this->m_proj_parm.tan_mode) {
                        xy_x *= c * c;
                        xy_y *= tan(lp_lat);
                    } else {
                        xy_x /= c;
                        xy_y *= sin(lp_lat);
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double c;
                    
                    xy_y /= this->m_proj_parm.C_y;
                    c = cos(lp_lat = this->m_proj_parm.tan_mode ? atan(xy_y) : aasin(xy_y));
                    lp_lat /= this->m_proj_parm.C_p;
                    lp_lon = xy_x / (this->m_proj_parm.C_x * cos(lp_lat));
                    if (this->m_proj_parm.tan_mode)
                        lp_lon /= c * c;
                    else
                        lp_lon *= c;
                }
            };

            template <typename Parameters>
            void setup(Parameters& par, par_sts& proj_parm, double p, double q, int mode) 
            {
                boost::ignore_unused_variable_warning(par);
                boost::ignore_unused_variable_warning(proj_parm);
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
                proj_parm.C_x = q / p;
                proj_parm.C_y = p;
                proj_parm.C_p = 1/ q;
                proj_parm.tan_mode = mode;
            }


            // Kavraisky V
            template <typename Parameters>
            void setup_kav5(Parameters& par, par_sts& proj_parm)
            {
                setup(par, proj_parm, 1.50488, 1.35439, 0);
            }

            // Quartic Authalic
            template <typename Parameters>
            void setup_qua_aut(Parameters& par, par_sts& proj_parm)
            {
                setup(par, proj_parm, 2., 2., 0);
            }

            // McBryde-Thomas Flat-Polar Sine (No. 1)
            template <typename Parameters>
            void setup_mbt_s(Parameters& par, par_sts& proj_parm)
            {
                setup(par, proj_parm, 1.48875, 1.36509, 0);
            }

            // Foucaut
            template <typename Parameters>
            void setup_fouc(Parameters& par, par_sts& proj_parm)
            {
                setup(par, proj_parm, 2., 2., 1);
            }

        }} // namespace detail::sts
    #endif // doxygen 

    /*!
        \brief Kavraisky V projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_kav5.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct kav5_spheroid : public detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>
    {
        inline kav5_spheroid(const Parameters& par) : detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::sts::setup_kav5(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Quartic Authalic projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_qua_aut.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct qua_aut_spheroid : public detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>
    {
        inline qua_aut_spheroid(const Parameters& par) : detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::sts::setup_qua_aut(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief McBryde-Thomas Flat-Polar Sine (No. 1) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_mbt_s.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct mbt_s_spheroid : public detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>
    {
        inline mbt_s_spheroid(const Parameters& par) : detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::sts::setup_mbt_s(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Foucaut projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_fouc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct fouc_spheroid : public detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>
    {
        inline fouc_spheroid(const Parameters& par) : detail::sts::base_sts_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::sts::setup_fouc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class kav5_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<kav5_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class qua_aut_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<qua_aut_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class mbt_s_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<mbt_s_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class fouc_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<fouc_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void sts_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("kav5", new kav5_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("qua_aut", new qua_aut_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("mbt_s", new mbt_s_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("fouc", new fouc_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_STS_HPP

