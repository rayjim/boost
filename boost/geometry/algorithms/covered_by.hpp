// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_COVERED_BY_HPP
#define BOOST_GEOMETRY_ALGORITHMS_COVERED_BY_HPP


#include <cstddef>

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/variant_fwd.hpp>

#include <boost/geometry/algorithms/not_implemented.hpp>
#include <boost/geometry/algorithms/within.hpp>

#include <boost/geometry/strategies/cartesian/point_in_box.hpp>
#include <boost/geometry/strategies/cartesian/box_in_box.hpp>
#include <boost/geometry/strategies/default_strategy.hpp>

namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

template
<
    typename Geometry1,
    typename Geometry2,
    typename Tag1 = typename tag<Geometry1>::type,
    typename Tag2 = typename tag<Geometry2>::type
>
struct covered_by: not_implemented<Tag1, Tag2>
{};


template <typename Point, typename Box>
struct covered_by<Point, Box, point_tag, box_tag>
{
    template <typename Strategy>
    static inline bool apply(Point const& point, Box const& box, Strategy const& strategy)
    {
        ::boost::ignore_unused_variable_warning(strategy);
        return strategy.apply(point, box);
    }
};

template <typename Box1, typename Box2>
struct covered_by<Box1, Box2, box_tag, box_tag>
{
    template <typename Strategy>
    static inline bool apply(Box1 const& box1, Box2 const& box2, Strategy const& strategy)
    {
        assert_dimension_equal<Box1, Box2>();
        ::boost::ignore_unused_variable_warning(strategy);
        return strategy.apply(box1, box2);
    }
};



template <typename Point, typename Ring>
struct covered_by<Point, Ring, point_tag, ring_tag>
{
    template <typename Strategy>
    static inline bool apply(Point const& point, Ring const& ring, Strategy const& strategy)
    {
        return detail::within::point_in_ring
            <
                Point,
                Ring,
                order_as_direction<geometry::point_order<Ring>::value>::value,
                geometry::closure<Ring>::value,
                Strategy
            >::apply(point, ring, strategy) >= 0;
    }
};

template <typename Point, typename Polygon>
struct covered_by<Point, Polygon, point_tag, polygon_tag>
{
    template <typename Strategy>
    static inline bool apply(Point const& point, Polygon const& polygon, Strategy const& strategy)
    {
        return detail::within::point_in_polygon
        <
            Point,
            Polygon,
            order_as_direction<geometry::point_order<Polygon>::value>::value,
            geometry::closure<Polygon>::value,
            Strategy
        >::apply(point, polygon, strategy) >= 0;
    }
};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


namespace resolve_strategy {

struct covered_by
{
    template <typename Geometry1, typename Geometry2, typename Strategy>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        concept::within::check
            <
                typename tag<Geometry1>::type, 
                typename tag<Geometry2>::type, 
                typename tag_cast<typename tag<Geometry2>::type, areal_tag>::type,
                Strategy
            >();
        concept::check<Geometry1 const>();
        concept::check<Geometry2 const>();
        assert_dimension_equal<Geometry1, Geometry2>();

        return dispatch::covered_by<Geometry1, Geometry2>::apply(geometry1,
                                                                 geometry2,
                                                                 strategy);
    }

    template <typename Geometry1, typename Geometry2>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             default_strategy)
    {
        typedef typename point_type<Geometry1>::type point_type1;
        typedef typename point_type<Geometry2>::type point_type2;
    
        typedef typename strategy::covered_by::services::default_strategy
            <
                typename tag<Geometry1>::type,
                typename tag<Geometry2>::type,
                typename tag<Geometry1>::type,
                typename tag_cast<typename tag<Geometry2>::type, areal_tag>::type,
                typename tag_cast
                    <
                        typename cs_tag<point_type1>::type, spherical_tag
                    >::type,
                typename tag_cast
                    <
                        typename cs_tag<point_type2>::type, spherical_tag
                    >::type,
                Geometry1,
                Geometry2
            >::type strategy_type;

        return covered_by::apply(geometry1, geometry2, strategy_type());
    }
};

} // namespace resolve_strategy


namespace resolve_variant {

template <typename Geometry1, typename Geometry2>
struct covered_by
{
    template <typename Strategy>
    static inline bool apply(Geometry1 const& geometry1,
                             Geometry2 const& geometry2,
                             Strategy const& strategy)
    {
        return resolve_strategy::covered_by
                               ::apply(geometry1, geometry2, strategy);
    }
};

template <BOOST_VARIANT_ENUM_PARAMS(typename T), typename Geometry2>
struct covered_by<boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>, Geometry2>
{
    template <typename Strategy>
    struct visitor: boost::static_visitor<bool>
    {
        Geometry2 const& m_geometry2;
        Strategy const& m_strategy;

        visitor(Geometry2 const& geometry2, Strategy const& strategy)
        : m_geometry2(geometry2), m_strategy(strategy) {}

        template <typename Geometry1>
        bool operator()(Geometry1 const& geometry1) const
        {
            return covered_by<Geometry1, Geometry2>
                   ::apply(geometry1, m_geometry2, m_strategy);
        }
    };

    template <typename Strategy>
    static inline bool
    apply(boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> const& geometry1,
          Geometry2 const& geometry2,
          Strategy const& strategy)
    {
        return boost::apply_visitor(visitor<Strategy>(geometry2, strategy), geometry1);
    }
};

template <typename Geometry1, BOOST_VARIANT_ENUM_PARAMS(typename T)>
struct covered_by<Geometry1, boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >
{
    template <typename Strategy>
    struct visitor: boost::static_visitor<bool>
    {
        Geometry1 const& m_geometry1;
        Strategy const& m_strategy;

        visitor(Geometry1 const& geometry1, Strategy const& strategy)
        : m_geometry1(geometry1), m_strategy(strategy) {}

        template <typename Geometry2>
        bool operator()(Geometry2 const& geometry2) const
        {
            return covered_by<Geometry1, Geometry2>
                   ::apply(m_geometry1, geometry2, m_strategy);
        }
    };

    template <typename Strategy>
    static inline bool
    apply(Geometry1 const& geometry1,
          boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> const& geometry2,
          Strategy const& strategy)
    {
        return boost::apply_visitor(visitor<Strategy>(geometry1, strategy), geometry2);
    }
};

template <
    BOOST_VARIANT_ENUM_PARAMS(typename T1),
    BOOST_VARIANT_ENUM_PARAMS(typename T2)
>
struct covered_by<
    boost::variant<BOOST_VARIANT_ENUM_PARAMS(T1)>,
    boost::variant<BOOST_VARIANT_ENUM_PARAMS(T2)>
>
{
    template <typename Strategy>
    struct visitor: boost::static_visitor<bool>
    {
        Strategy const& m_strategy;

        visitor(Strategy const& strategy): m_strategy(strategy) {}

        template <typename Geometry1, typename Geometry2>
        bool operator()(Geometry1 const& geometry1,
                        Geometry2 const& geometry2) const
        {
            return covered_by<Geometry1, Geometry2>
                   ::apply(geometry1, geometry2, m_strategy);
        }
    };

    template <typename Strategy>
    static inline bool
    apply(boost::variant<BOOST_VARIANT_ENUM_PARAMS(T1)> const& geometry1,
          boost::variant<BOOST_VARIANT_ENUM_PARAMS(T2)> const& geometry2,
          Strategy const& strategy)
    {
        return boost::apply_visitor(visitor<Strategy>(strategy), geometry1, geometry2);
    }
};

} // namespace resolve_variant


/*!
\brief \brief_check12{is inside or on border}
\ingroup covered_by
\details \details_check12{covered_by, is inside or on border}.
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\param geometry1 \param_geometry which might be inside or on the border of the second geometry
\param geometry2 \param_geometry which might cover the first geometry
\return true if geometry1 is inside of or on the border of geometry2,
    else false
\note The default strategy is used for covered_by detection

\qbk{[include reference/algorithms/covered_by.qbk]}

 */
template<typename Geometry1, typename Geometry2>
inline bool covered_by(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    return resolve_variant::covered_by<Geometry1, Geometry2>
                          ::apply(geometry1, geometry2, default_strategy());
}

/*!
\brief \brief_check12{is inside or on border} \brief_strategy
\ingroup covered_by
\details \details_check12{covered_by, is inside or on border}, \brief_strategy. \details_strategy_reasons
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\param geometry1 \param_geometry which might be inside or on the border of the second geometry
\param geometry2 \param_geometry which might cover the first geometry
\param strategy strategy to be used
\return true if geometry1 is inside of or on the border of geometry2,
    else false

\qbk{distinguish,with strategy}
\qbk{[include reference/algorithms/covered_by.qbk]}

*/
template<typename Geometry1, typename Geometry2, typename Strategy>
inline bool covered_by(Geometry1 const& geometry1, Geometry2 const& geometry2,
        Strategy const& strategy)
{
    return resolve_variant::covered_by<Geometry1, Geometry2>
                          ::apply(geometry1, geometry2, strategy);
}

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_COVERED_BY_HPP
