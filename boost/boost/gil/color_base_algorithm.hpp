/*
  Copyright 2005-2007 Adobe Systems Incorporated
  Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
  or a copy at http://opensource.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#ifndef GIL_COLOR_BASE_ALGORITHM_HPP
#define GIL_COLOR_BASE_ALGORITHM_HPP

////////////////////////////////////////////////////////////////////////////////////////
/// \file               
/// \brief pixel related algorithms
/// \author Lubomir Bourdev and Hailin Jin \n
///         Adobe Systems Incorporated
/// \date   2005-2007 \n Last updated on February 16, 2007
///
////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/at.hpp>
#include "gil_config.hpp"
#include "gil_concept.hpp"
#include "utilities.hpp"

namespace boost { namespace gil {


///////////////////////////////////////
///
/// size:   Semantic channel size
///
///////////////////////////////////////

/// \defgroup ColorBaseAlgorithmSize size
/// \ingroup ColorBaseAlgorithm
/// \brief Returns an MPL integral type specifying the number of elements in a color base

/// \brief Returns an MPL integral type specifying the number of elements in a color base
/// \ingroup ColorBaseAlgorithmSize
template <typename ColorBase>
struct size : public mpl::size<typename ColorBase::layout_t::color_space_t> {};

///////////////////////////////////////
///
/// semantic_at_c:   Semantic channel accessors
///
///////////////////////////////////////

/// \defgroup ColorBaseAlgorithmSemanticAtC kth_semantic_element_type, kth_semantic_element_reference_type, kth_semantic_element_const_reference_type, semantic_at_c
/// \ingroup ColorBaseAlgorithm
/// \brief Support for accessing the elements of a color base by semantic index
/// The semantic index of an element is the index of its color in the color space. Semantic indexing allows for proper pairing of elements of color bases
/// independent on their layout. For example, red is the first semantic element of a color base regardless of whether it has an RGB layout or a BGR layout.
/// All GIL color base algorithms taking multiple color bases use semantic indexing to access their elements.

/// \brief Specifies the type of the K-th semantic element of a color base
/// \ingroup ColorBaseAlgorithmSemanticAtC
template <typename ColorBase, int K> struct kth_semantic_element_type {
    BOOST_STATIC_CONSTANT(int, semantic_index = (mpl::at_c<typename ColorBase::layout_t::channel_mapping_t,K>::type::value));
    typedef typename ColorBase::template kth_element_type<semantic_index>::type type;
};

/// \brief Specifies the return type of the mutable semantic_at_c<K>(color_base);
/// \ingroup ColorBaseAlgorithmSemanticAtC
template <typename ColorBase, int K> struct kth_semantic_element_reference_type {
    BOOST_STATIC_CONSTANT(int, semantic_index = (mpl::at_c<typename ColorBase::layout_t::channel_mapping_t,K>::type::value));
    typedef typename ColorBase::template kth_element_reference_type<semantic_index>::type type;
    static type       get(ColorBase& cb) { return at_c<semantic_index>(cb); }
};

/// \brief Specifies the return type of the constant semantic_at_c<K>(color_base);
/// \ingroup ColorBaseAlgorithmSemanticAtC
template <typename ColorBase, int K> struct kth_semantic_element_const_reference_type {
    BOOST_STATIC_CONSTANT(int, semantic_index = (mpl::at_c<typename ColorBase::layout_t::channel_mapping_t,K>::type::value));
    typedef typename ColorBase::template kth_element_const_reference_type<semantic_index>::type type;
    static type       get(const ColorBase& cb) { return at_c<semantic_index>(cb); }
};

/// \brief A mutable accessor to the K-th semantic element of a color base
/// \ingroup ColorBaseAlgorithmSemanticAtC
template <int K, typename ColorBase> inline
typename disable_if<is_const<ColorBase>,typename kth_semantic_element_reference_type<ColorBase,K>::type>::type
semantic_at_c(ColorBase& p) { 
    return kth_semantic_element_reference_type<ColorBase,K>::get(p); 
};

/// \brief A constant accessor to the K-th semantic element of a color base
/// \ingroup ColorBaseAlgorithmSemanticAtC
template <int K, typename ColorBase> inline
typename kth_semantic_element_const_reference_type<ColorBase,K>::type
semantic_at_c(const ColorBase& p) { 
    return kth_semantic_element_const_reference_type<ColorBase,K>::get(p); 
};

///////////////////////////////////////
///
/// get_color:   Named channel accessors
///
///////////////////////////////////////

/// \defgroup ColorBaseAlgorithmColor color_element_type, color_element_reference_type, color_element_const_reference_type, get_color
/// \ingroup ColorBaseAlgorithm
/// \brief Support for accessing the elements of a color base by color name

template <typename ColorBase, typename Color>
struct color_index_type : public detail::type_to_index<typename ColorBase::layout_t::color_space_t,Color> {};

/// \brief Specifies the type of the element associated with a given color tag
/// \ingroup ColorBaseAlgorithmColor
template <typename Color, typename ColorBase>
struct color_element_type : public kth_semantic_element_type<ColorBase,color_index_type<ColorBase,Color>::value> {};

/// \brief Specifies the return type of the mutable element accessor by color name, get_color(color_base, Color());
/// \ingroup ColorBaseAlgorithmColor
template <typename Color, typename ColorBase>
struct color_element_reference_type : public kth_semantic_element_reference_type<ColorBase,color_index_type<ColorBase,Color>::value> {};

/// \brief Specifies the return type of the constant element accessor by color name, get_color(color_base, Color());
/// \ingroup ColorBaseAlgorithmColor
template <typename Color, typename ColorBase>
struct color_element_const_reference_type : public kth_semantic_element_const_reference_type<ColorBase,color_index_type<ColorBase,Color>::value> {};

/// \brief Mutable accessor to the element associated with a given color name
/// \ingroup ColorBaseAlgorithmColor
template <typename CB, typename Color> 
typename color_element_reference_type<Color,CB>::type get_color(CB& cb, Color=Color()) {
    return color_element_reference_type<Color,CB>::get(cb);
}

/// \brief Constant accessor to the element associated with a given color name
/// \ingroup ColorBaseAlgorithmColor
template <typename CB, typename Color> 
typename color_element_const_reference_type<Color,CB>::type get_color(const CB& cb, Color=Color()) {
    return color_element_const_reference_type<Color,CB>::get(cb);
}

///////////////////////////////////////
///
/// element_type, element_reference_type, element_const_reference_type: Support for homogeneous color bases
///
///////////////////////////////////////

/// \defgroup ColorBaseAlgorithmHomogeneous element_type, element_reference_type, element_const_reference_type
/// \ingroup ColorBaseAlgorithm
/// \brief Types for homogeneous color bases

/// \brief Specifies the element type of a homogeneous color base
/// \ingroup ColorBaseAlgorithmHomogeneous
template <typename ColorBase>
struct element_type : public ColorBase::template kth_element_type<0> {};

/// \brief Specifies the return type of the mutable element accessor at_c of a homogeneous color base
/// \ingroup ColorBaseAlgorithmHomogeneous
template <typename ColorBase>
struct element_reference_type : public ColorBase::template kth_element_reference_type<0> {};

/// \brief Specifies the return type of the constant element accessor at_c of a homogeneous color base
/// \ingroup ColorBaseAlgorithmHomogeneous
template <typename ColorBase>
struct element_const_reference_type : public ColorBase::template kth_element_const_reference_type<0> {};


namespace detail {

// compile-time recursion for per-element operations on color bases
template <int N>
struct element_recursion {
    //static_equal
    template <typename P1,typename P2>
    static bool static_equal(const P1& p1, const P2& p2) { 
        return element_recursion<N-1>::static_equal(p1,p2) &&
               semantic_at_c<N-1>(p1)==semantic_at_c<N-1>(p2); 
    }
    //static_copy
    template <typename P1,typename P2>
    static void static_copy(const P1& p1, P2& p2) {
        element_recursion<N-1>::static_copy(p1,p2);
        semantic_at_c<N-1>(p2)=semantic_at_c<N-1>(p1);
    }
    //static_fill
    template <typename P,typename T2>
    static void static_fill(P& p, T2 v) {
        element_recursion<N-1>::static_fill(p,v);
        semantic_at_c<N-1>(p)=v;
    }
    //static_generate
    template <typename Dst,typename Op> 
    static void static_generate(Dst& dst, Op op) {
        element_recursion<N-1>::static_generate(dst,op);
        semantic_at_c<N-1>(dst)=op();
    }
    //static_for_each with one source
    template <typename P1,typename Op> 
    static Op static_for_each(P1& p1, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,op));
        op2(semantic_at_c<N-1>(p1));
        return op2;
    }
    template <typename P1,typename Op> 
    static Op static_for_each(const P1& p1, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,op));
        op2(semantic_at_c<N-1>(p1));
        return op2;
    }
    //static_for_each with two sources
    template <typename P1,typename P2,typename Op> 
    static Op static_for_each(P1& p1, P2& p2, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2));
        return op2;
    }
    template <typename P1,typename P2,typename Op> 
    static Op static_for_each(P1& p1, const P2& p2, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2));
        return op2;
    }
    template <typename P1,typename P2,typename Op> 
    static Op static_for_each(const P1& p1, P2& p2, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2));
        return op2;
    }
    template <typename P1,typename P2,typename Op> 
    static Op static_for_each(const P1& p1, const P2& p2, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2));
        return op2;
    }
    //static_for_each with three sources
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(P1& p1, P2& p2, P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(P1& p1, P2& p2, const P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(P1& p1, const P2& p2, P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(P1& p1, const P2& p2, const P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(const P1& p1, P2& p2, P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(const P1& p1, P2& p2, const P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(const P1& p1, const P2& p2, P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(const P1& p1, const P2& p2, const P3& p3, Op op) {
        Op op2(element_recursion<N-1>::static_for_each(p1,p2,p3,op));
        op2(semantic_at_c<N-1>(p1), semantic_at_c<N-1>(p2), semantic_at_c<N-1>(p3));
        return op2;
    }
    //static_transform with one source
    template <typename P1,typename Dst,typename Op> 
    static Op static_transform(P1& src, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src));
        return op2;
    }
    template <typename P1,typename Dst,typename Op> 
    static Op static_transform(const P1& src, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src));
        return op2;
    }
    //static_transform with two sources
    template <typename P1,typename P2,typename Dst,typename Op>
    static Op static_transform(P1& src1, P2& src2, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src1,src2,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src1), semantic_at_c<N-1>(src2));
        return op2;
    }
    template <typename P1,typename P2,typename Dst,typename Op>
    static Op static_transform(P1& src1, const P2& src2, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src1,src2,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src1), semantic_at_c<N-1>(src2));
        return op2;
    }
    template <typename P1,typename P2,typename Dst,typename Op>
    static Op static_transform(const P1& src1, P2& src2, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src1,src2,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src1), semantic_at_c<N-1>(src2));
        return op2;
    }
    template <typename P1,typename P2,typename Dst,typename Op>
    static Op static_transform(const P1& src1, const P2& src2, Dst& dst, Op op) {
        Op op2(element_recursion<N-1>::static_transform(src1,src2,dst,op));
        semantic_at_c<N-1>(dst)=op2(semantic_at_c<N-1>(src1), semantic_at_c<N-1>(src2));
        return op2;
    }
};

// Termination condition of the compile-time recursion for element operations on a color base
template<> struct element_recursion<0> {
    //static_equal
    template <typename P1,typename P2>
    static bool static_equal(const P1&, const P2&) { return true; }
    //static_copy
    template <typename P1,typename P2>
    static void static_copy(const P1&, const P2&) {}
    //static_fill
    template <typename P, typename T2>
    static void static_fill(const P&, T2) {}
    //static_generate
    template <typename Dst,typename Op>
    static void static_generate(const Dst&,Op){}
    //static_for_each with one source
    template <typename P1,typename Op>
    static Op static_for_each(const P1&,Op op){return op;}
    //static_for_each with two sources
    template <typename P1,typename P2,typename Op>
    static Op static_for_each(const P1&,const P2&,Op op){return op;}
    //static_for_each with three sources
    template <typename P1,typename P2,typename P3,typename Op>
    static Op static_for_each(const P1&,const P2&,const P3&,Op op){return op;}
    //static_transform with one source
    template <typename P1,typename Dst,typename Op>
    static Op static_transform(const P1&,const Dst&,Op op){return op;}
    //static_transform with two sources
    template <typename P1,typename P2,typename Dst,typename Op>
    static Op static_transform(const P1&,const P2&,const Dst&,Op op){return op;}
};

// std::min and std::max don't have the mutable overloads...
template <typename Q> inline const Q& min(const Q& x, const Q& y) { return x<y ? x : y; }
template <typename Q> inline       Q& min(      Q& x,       Q& y) { return x<y ? x : y; }
template <typename Q> inline const Q& max(const Q& x, const Q& y) { return x<y ? y : x; }
template <typename Q> inline       Q& max(      Q& x,       Q& y) { return x<y ? y : x; }


// compile-time recursion for min/max element
template <int N>
struct min_max_recur {
    template <typename P> static typename element_const_reference_type<P>::type max_(const P& p) {
        return detail::max(min_max_recur<N-1>::max_(p),semantic_at_c<N-1>(p));
    }    
    template <typename P> static typename element_reference_type<P>::type       max_(      P& p) {
        return detail::max(min_max_recur<N-1>::max_(p),semantic_at_c<N-1>(p));
    }    
    template <typename P> static typename element_const_reference_type<P>::type min_(const P& p) {
        return detail::min(min_max_recur<N-1>::min_(p),semantic_at_c<N-1>(p));
    }    
    template <typename P> static typename element_reference_type<P>::type       min_(      P& p) {
        return detail::min(min_max_recur<N-1>::min_(p),semantic_at_c<N-1>(p));
    }    
};

// termination condition of the compile-time recursion for min/max element
template <>
struct min_max_recur<1> {
    template <typename P> static typename element_const_reference_type<P>::type max_(const P& p) { return semantic_at_c<0>(p); }
    template <typename P> static typename element_reference_type<P>::type       max_(      P& p) { return semantic_at_c<0>(p); }
    template <typename P> static typename element_const_reference_type<P>::type min_(const P& p) { return semantic_at_c<0>(p); }
    template <typename P> static typename element_reference_type<P>::type       min_(      P& p) { return semantic_at_c<0>(p); }
};
}  // namespace detail


/// \defgroup ColorBaseAlgorithmMinMax static_min, static_max
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalents to std::min_element and std::max_element for homogeneous color bases
/// \{

template <typename P>
GIL_FORCEINLINE
typename element_const_reference_type<P>::type static_max(const P& p) { return detail::min_max_recur<size<P>::value>::max_(p); }

template <typename P>
GIL_FORCEINLINE
typename element_reference_type<P>::type       static_max(      P& p) { return detail::min_max_recur<size<P>::value>::max_(p); }

template <typename P>
GIL_FORCEINLINE
typename element_const_reference_type<P>::type static_min(const P& p) { return detail::min_max_recur<size<P>::value>::min_(p); }

template <typename P>
GIL_FORCEINLINE
typename element_reference_type<P>::type       static_min(      P& p) { return detail::min_max_recur<size<P>::value>::min_(p); }
/// \}

/// \defgroup ColorBaseAlgorithmEqual static_equal 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::equal. Pairs the elements semantically
/// \{

template <typename P1,typename P2>
GIL_FORCEINLINE
bool static_equal(const P1& p1, const P2& p2) { return detail::element_recursion<size<P1>::value>::static_equal(p1,p2); }

/// \}

/// \defgroup ColorBaseAlgorithmCopy static_copy 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::copy. Pairs the elements semantically
/// \{

template <typename Src,typename Dst>
GIL_FORCEINLINE
void static_copy(const Src& src, Dst& dst) {  detail::element_recursion<size<Dst>::value>::static_copy(src,dst); }

/// \}

/// \defgroup ColorBaseAlgorithmFill static_fill 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::fill.
/// \{

template <typename P,typename V>
GIL_FORCEINLINE
void static_fill(P& p, const V& v) {  detail::element_recursion<size<P>::value>::static_fill(p,v); }
/// \}

/// \defgroup ColorBaseAlgorithmGenerate static_generate 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::generate.
/// \{

template <typename P1,typename Op>
GIL_FORCEINLINE
void static_generate(P1& dst,Op op)                      { detail::element_recursion<size<P1>::value>::static_generate(dst,op); }
/// \}

/// \defgroup ColorBaseAlgorithmTransform static_transform 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::transform. Pairs the elements semantically
/// \{

//static_transform with one source
template <typename Src,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(Src& src,Dst& dst,Op op)              { return detail::element_recursion<size<Dst>::value>::static_transform(src,dst,op); }
template <typename Src,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(const Src& src,Dst& dst,Op op)              { return detail::element_recursion<size<Dst>::value>::static_transform(src,dst,op); }
//static_transform with two sources
template <typename P2,typename P3,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(P2& p2,P3& p3,Dst& dst,Op op) { return detail::element_recursion<size<Dst>::value>::static_transform(p2,p3,dst,op); }
template <typename P2,typename P3,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(P2& p2,const P3& p3,Dst& dst,Op op) { return detail::element_recursion<size<Dst>::value>::static_transform(p2,p3,dst,op); }
template <typename P2,typename P3,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(const P2& p2,P3& p3,Dst& dst,Op op) { return detail::element_recursion<size<Dst>::value>::static_transform(p2,p3,dst,op); }
template <typename P2,typename P3,typename Dst,typename Op>
GIL_FORCEINLINE
Op static_transform(const P2& p2,const P3& p3,Dst& dst,Op op) { return detail::element_recursion<size<Dst>::value>::static_transform(p2,p3,dst,op); }
/// \}

/// \defgroup ColorBaseAlgorithmForEach static_for_each 
/// \ingroup ColorBaseAlgorithm
/// \brief Equivalent to std::for_each. Pairs the elements semantically
/// \{

//static_for_each with one source
template <typename P1,typename Op>
GIL_FORCEINLINE
Op static_for_each(      P1& p1, Op op)                          { return detail::element_recursion<size<P1>::value>::static_for_each(p1,op); }
template <typename P1,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1, Op op)                          { return detail::element_recursion<size<P1>::value>::static_for_each(p1,op); }
//static_for_each with two sources
template <typename P1,typename P2,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,      P2& p2, Op op)             { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,op); }
template <typename P1,typename P2,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,const P2& p2, Op op)             { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,op); }
template <typename P1,typename P2,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,      P2& p2, Op op)             { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,op); }
template <typename P1,typename P2,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,const P2& p2, Op op)             { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,op); }
//static_for_each with three sources
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,P2& p2,P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,P2& p2,const P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,const P2& p2,P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(P1& p1,const P2& p2,const P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,P2& p2,P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,P2& p2,const P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,const P2& p2,P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
template <typename P1,typename P2,typename P3,typename Op>
GIL_FORCEINLINE
Op static_for_each(const P1& p1,const P2& p2,const P3& p3,Op op) { return detail::element_recursion<size<P1>::value>::static_for_each(p1,p2,p3,op); }
///\}

} }  // namespace boost::gil

#endif
