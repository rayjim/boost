// Copyright (C) 2002 Ronald Garcia
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.
//

#ifndef BOOST_MULTI_ARRAY_CONCEPT_CHECKS_RG110101_HPP
#define BOOST_MULTI_ARRAY_CONCEPT_CHECKS_RG110101_HPP

//
// concept-checks.hpp - Checks out Const MultiArray and MultiArray
// concepts
//

#include "boost/concept_check.hpp"
#include "boost/iterator/iterator_concepts.hpp"

namespace boost {
namespace detail {
namespace multi_array {

  //
  // idgen_helper -
  //   This is a helper for generating index_gen instantiations with
  //   the right type in order to test the call to
  //   operator[](index_gen).  Since one would normally write:
  //      A[ indices[range1][range2] ]; // or
  //      B[ indices[index1][index2][range1] ];
  //   idgen helper allows us to generate the "indices" type by
  //   creating it through recursive calls.
  template <std::size_t N>
  struct idgen_helper {

    template <typename Array, typename IdxGen, typename Call_Type>
    static void call(Array& a, const IdxGen& idgen, Call_Type c) {
      typedef typename Array::index_range index_range;
      typedef typename Array::index index;
      idgen_helper<N-1>::call(a,idgen[c],c);
    }
  };

  template <>
  struct idgen_helper<0> {

    template <typename Array, typename IdxGen, typename Call_Type>
    static void call(Array& a, const IdxGen& idgen, Call_Type) {
      typedef typename Array::index_range index_range;
      typedef typename Array::index index;
      a[ idgen ];
    }
  };


  template <typename Array, std::size_t NumDims >
  struct ConstMultiArrayConcept
  {
    void constraints() {
    //    function_requires< CopyConstructibleConcept<Array> >();
    function_requires< boost_concepts::ForwardTraversalConcept<iterator> >();
    function_requires< boost_concepts::ReadableIteratorConcept<iterator> >();
    function_requires< boost_concepts::ForwardTraversalConcept<const_iterator> >();
    function_requires< boost_concepts::ReadableIteratorConcept<const_iterator> >();

      // RG - a( CollectionArchetype) when available...
      a[ id ];
      // Test slicing, keeping only the first dimension, losing the rest
      idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      cit = a.begin();
      cit = a.end();
      crit = a.rbegin();
      crit = a.rend();
      eltp = a.origin();
    }

    typedef typename Array::value_type value_type;
    typedef typename Array::reference reference;
    typedef typename Array::const_reference const_reference;
    typedef typename Array::size_type size_type;
    typedef typename Array::difference_type difference_type;
    typedef typename Array::iterator iterator;
    typedef typename Array::const_iterator const_iterator;
    typedef typename Array::reverse_iterator reverse_iterator;
    typedef typename Array::const_reverse_iterator const_reverse_iterator;
    typedef typename Array::element element;
    typedef typename Array::index index;
    typedef typename Array::index_gen index_gen;
    typedef typename Array::index_range index_range;
    typedef typename Array::extent_gen extent_gen;
    typedef typename Array::extent_range extent_range;

    Array a;
    size_type st;
    const size_type* stp;
    index id;
    const index* idp;
    const_iterator cit;
    const_reverse_iterator crit;
    const element* eltp;
    index_gen idgen;
    index_range range;
  };


  template <typename Array, std::size_t NumDims >
  struct MutableMultiArrayConcept
  {
    void constraints() {
      //    function_requires< CopyConstructibleConcept<Array> >();

      function_requires< boost_concepts::ForwardTraversalConcept<iterator> >();
      function_requires< boost_concepts::ReadableIteratorConcept<iterator> >();
      function_requires< boost_concepts::WritableIteratorConcept<iterator> >();
      function_requires< boost_concepts::ForwardTraversalConcept<const_iterator> >();
      function_requires< boost_concepts::ReadableIteratorConcept<const_iterator> >();
      
      // RG - a( CollectionArchetype) when available...
      value_type vt = a[ id ];

      // Test slicing, keeping only the first dimension, losing the rest
      idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      it = a.begin();
      it = a.end();
      rit = a.rbegin();
      rit = a.rend();
      eltp = a.origin();
      const_constraints(a);
    }

    void const_constraints(const Array& a) {

      //      value_type vt = a[ id ];

      // Test slicing, keeping only the first dimension, losing the rest
      idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      cit = a.begin();
      cit = a.end();
      crit = a.rbegin();
      crit = a.rend();
      eltp = a.origin();
    }

    typedef typename Array::value_type value_type;
    typedef typename Array::reference reference;
    typedef typename Array::const_reference const_reference;
    typedef typename Array::size_type size_type;
    typedef typename Array::difference_type difference_type;
    typedef typename Array::iterator iterator;
    typedef typename Array::const_iterator const_iterator;
    typedef typename Array::reverse_iterator reverse_iterator;
    typedef typename Array::const_reverse_iterator const_reverse_iterator;
    typedef typename Array::element element;
    typedef typename Array::index index;
    typedef typename Array::index_gen index_gen;
    typedef typename Array::index_range index_range;
    typedef typename Array::extent_gen extent_gen;
    typedef typename Array::extent_range extent_range;

    Array a;
    size_type st;
    const size_type* stp;
    index id;
    const index* idp;
    iterator it;
    const_iterator cit;
    reverse_iterator rit;
    const_reverse_iterator crit;
    const element* eltp;
    index_gen idgen;
    index_range range;
  };


} // namespace multi_array
} // namespace detail
} // namespace boost


#endif // BOOST_MULTI_ARRAY_CONCEPT_CHECKS_RG110101_HPP
