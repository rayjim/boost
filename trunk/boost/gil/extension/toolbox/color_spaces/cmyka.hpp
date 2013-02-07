/*
    Copyright 2012 Christian Henning
    Use, modification and distribution are subject to the Boost Software License,
    Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
*/

/*************************************************************************************************/

#ifndef BOOST_GIL_EXTENSION_TOOLBOX_COLOR_SPACES_CMYKA_HPP
#define BOOST_GIL_EXTENSION_TOOLBOX_COLOR_SPACES_CMYKA_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file cmyka.hpp
/// \brief Support for cmyka color space.
/// \author Christian Henning \n
///
/// \date 2012 \n
///
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/rgba.hpp>
#include <boost/gil/cmyk.hpp>

#include <boost/gil/color_convert.hpp>
#include <boost/gil/typedefs.hpp>

namespace boost{ namespace gil {

/// \ingroup ColorSpaceModel
typedef mpl::vector5<cyan_t,magenta_t,yellow_t,black_t,alpha_t> cmyka_t;

/// \ingroup LayoutModel
typedef layout<cmyka_t> cmyka_layout_t;


GIL_DEFINE_ALL_TYPEDEFS(8,  cmyka)
GIL_DEFINE_ALL_TYPEDEFS(8s, cmyka)
GIL_DEFINE_ALL_TYPEDEFS(16, cmyka)
GIL_DEFINE_ALL_TYPEDEFS(16s,cmyka)
GIL_DEFINE_ALL_TYPEDEFS(32 ,cmyka)
GIL_DEFINE_ALL_TYPEDEFS(32s,cmyka)
GIL_DEFINE_ALL_TYPEDEFS(32f,cmyka)

///// \ingroup ColorConvert
///// \brief Converting CMYKA to any pixel type. Note: Supports homogeneous pixels only.
//template <typename C2>
//struct default_color_converter_impl<cmyka_t,C2> {
//    template <typename P1, typename P2>
//    void operator()(const P1& src, P2& dst) const {
//        typedef typename channel_type<P1>::type T1;
//        default_color_converter_impl<cmyk_t,C2>()(
//            pixel<T1,cmyk_layout_t>(channel_multiply(get_color(src,cyan_t()),  get_color(src,alpha_t())), 
//                                    channel_multiply(get_color(src,magenta_t()),get_color(src,alpha_t())), 
//                                    channel_multiply(get_color(src,yellow_t()), get_color(src,alpha_t())),
//									channel_multiply(get_color(src,black_t()), get_color(src,alpha_t())))
//            ,dst);
//    }
//};
template <>
struct default_color_converter_impl<cmyka_t,rgba_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        typedef typename channel_type<P1>::type T1;
        default_color_converter_impl<cmyk_t,rgba_t>()(
            pixel<T1,cmyk_layout_t>(get_color(src,cyan_t()), 
                                    get_color(src,magenta_t()), 
                                    get_color(src,yellow_t()),
									get_color(src,black_t()))
            ,dst);
    }
};

/// \ingroup ColorConvert
/// \brief Unfortunately CMYKA to CMYKA must be explicitly provided - otherwise we get ambiguous specialization error.
template <>
struct default_color_converter_impl<cmyka_t,cmyka_t> {
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const {
        static_for_each(src,dst,default_channel_converter());
    }
};

} // namespace gil
} // namespace boost

#endif // BOOST_GIL_EXTENSION_TOOLBOX_COLOR_SPACES_CMYKA_HPP
