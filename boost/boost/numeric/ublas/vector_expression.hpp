//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef VECTOR_EXPRESSION_H
#define VECTOR_EXPRESSION_H

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/exception.hpp>
#include <boost/numeric/ublas/functional.hpp>
#include <boost/numeric/ublas/noalias.hpp>

// Expression templates based on ideas of Todd Veldhuizen and Geoffrey Furnish
// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numeric { namespace ublas {

    // Base class for linear algebra expression's
    template<class T>
    class expression_base:
        private nonassignable {
        typedef const T const_value_type;
    public:
    };


    template<class T>
    struct scalar_expression:
        public expression_base<T> {
        typedef T value_type;
    };

    template<class T>
    class scalar_value:
        public scalar_expression<T> {
    public:
        typedef T value_type;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        scalar_value ():
            t_ () {}
        BOOST_UBLAS_INLINE
        scalar_value (const value_type &t):
            t_ (t) {}

        BOOST_UBLAS_INLINE
        operator value_type () const {
            return t_;
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const scalar_value &sv) const {
            return this == &sv;    // shortcut for &t_ == &sv.t_
        }

    private:
        value_type t_;
    };

    template<class T>
    class scalar_const_reference:
        public scalar_expression<T> {
    public:
        typedef T value_type;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        scalar_const_reference ():
            t_ (nil_) {}
        BOOST_UBLAS_INLINE
        scalar_const_reference (const value_type &t):
            t_ (t) {}

        BOOST_UBLAS_INLINE
        operator value_type () const {
            return t_;
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const scalar_const_reference &sr) const {
            return &t_ == &sr.t_;
        }

    private:
        const value_type &t_;
        static const value_type nil_;
    };

    template<class T>
    const typename scalar_const_reference<T>::value_type scalar_const_reference<T>::nil_
        = BOOST_UBLAS_TYPENAME scalar_const_reference<T>::value_type ();


    // Base class for the Barton Nackman trick
    template<class E>
    class vector_expression:
        private nonassignable {
        //FIXME        public expression_base<typename E::value_type> {
    public:
        BOOST_STATIC_CONSTANT (unsigned, complexity = 0);
        typedef E expression_type;
        typedef vector_tag type_category;
        typedef abstract_tag simd_category;
 
        typedef noalias_proxy<E> noalias_proxy_type;
        typedef const vector_range<const E> const_vector_range_type;
        typedef vector_range<E> vector_range_type;
        typedef const vector_slice<const E> const_vector_slice_type;
        typedef vector_slice<E> vector_slice_type;
        typedef const vector_indirect<const E> const_vector_indirect_type;
        typedef vector_indirect<E> vector_indirect_type;

        // This class could define an common interface for all
        // statically derived expression type classes.
        // Due to a compiler deficiency - one can not reference class typedefs of E
        // on MSVC 6.0 (error C2027) - we only implement the casts.

        BOOST_UBLAS_INLINE
        const expression_type &operator () () const {
            return *static_cast<const expression_type *> (this);
        }
        BOOST_UBLAS_INLINE
        expression_type &operator () () {
            return *static_cast<expression_type *> (this);
        }

        BOOST_UBLAS_INLINE
        noalias_proxy_type noalias () {
            return noalias_proxy_type (operator () ());
        }
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_INLINE
        const_vector_range_type operator () (const range &r) const {
            return const_vector_range_type (operator () (), r);
        }
        BOOST_UBLAS_INLINE
        vector_range_type operator () (const range &r) {
            return vector_range_type (operator () (), r);
        }
        BOOST_UBLAS_INLINE
        const_vector_slice_type operator () (const slice &s) const {
            return const_vector_slice_type (operator () (), s);
        }
        BOOST_UBLAS_INLINE
        vector_slice_type operator () (const slice &s) {
            return vector_slice_type (operator () (), s);
        }
        template<class A>
        BOOST_UBLAS_INLINE
        const_vector_indirect_type operator () (const indirect_array<A> &ia) const {
            return const_vector_indirect_type (operator () (), ia);
        }
        template<class A>
        BOOST_UBLAS_INLINE
        vector_indirect_type operator () (const indirect_array<A> &ia) {
            return vector_indirect_type (operator () (), ia);
        }
#else
        BOOST_UBLAS_INLINE
        const_vector_range_type project (const range &r) const {
            return const_vector_range_type (operator () (), r);
        }
        BOOST_UBLAS_INLINE
        vector_range_type project (const range &r) {
            return vector_range_type (operator () (), r);
        }
        BOOST_UBLAS_INLINE
        const_vector_slice_type project (const slice &s) const {
            return const_vector_slice_type (operator () (), s);
        }
        BOOST_UBLAS_INLINE
        vector_slice_type project (const slice &s) {
            return vector_slice_type (operator () (), s);
        }
        template<class A>
        BOOST_UBLAS_INLINE
        const_vector_indirect_type project (const indirect_array<A> &ia) const {
            return const_vector_indirect_type (operator () (), ia);
        }
        template<class A>
        BOOST_UBLAS_INLINE
        vector_indirect_type project (const indirect_array<A> &ia) {
            return vector_indirect_type (operator () (), ia);
        }
#endif
    };

    template<class E>
    class vector_reference:
        public vector_expression<vector_reference<E> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<vector_reference<E> >::operator ();
#endif
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename E::value_type value_type;
#ifndef BOOST_UBLAS_CT_REFERENCE_BASE_TYPEDEFS
        typedef typename E::const_reference const_reference;
        typedef typename E::reference reference;
#else
        typedef typename E::const_reference const_reference;
        typedef typename boost::mpl::if_<boost::is_const<E>,
                                          typename E::const_reference,
                                          typename E::reference>::type reference;
#endif
    private:
        typedef vector_reference<E> self_type;
    public:
        typedef E refered_type;
        typedef const self_type const_closure_type;
        typedef const_closure_type closure_type;
        typedef typename E::storage_category storage_category;
        typedef typename E::simd_category simd_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_reference ():
            e_ (nil_) {}
        BOOST_UBLAS_INLINE
        vector_reference (refered_type &e):
            e_ (e) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return expression ().size ();
        }

#ifndef BOOST_UBLAS_NESTED_CLASS_DR45
    private:
#endif
        // Expression accessors - const correct
        BOOST_UBLAS_INLINE
        const refered_type &expression () const {
            return e_;
        }
        BOOST_UBLAS_INLINE
        refered_type &expression () {
            return e_;
        }

    public:
        // Element access
#ifndef BOOST_UBLAS_REFERENCE_CONST_MEMBER
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return expression () (i);
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            return expression () (i);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return expression () [i];
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            return expression () [i];
        }
#else
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) const {
            return expression () (i);
        }

        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) const {
            return expression () [i];
        }
#endif

        // Assignment
        BOOST_UBLAS_INLINE
        vector_reference &operator = (const vector_reference &v) {
            expression ().operator = (v);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &operator = (const vector_expression<AE> &ae) {
            expression ().operator = (ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &assign (const vector_expression<AE> &ae) {
            expression ().assign (ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &operator += (const vector_expression<AE> &ae) {
            expression ().operator += (ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &plus_assign (const vector_expression<AE> &ae) {
            expression ().plus_assign (ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &operator -= (const vector_expression<AE> &ae) {
            expression ().operator -= (ae);
            return *this;
        }
        template<class AE>
        BOOST_UBLAS_INLINE
        vector_reference &minus_assign (const vector_expression<AE> &ae) {
            expression ().minus_assign (ae);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        vector_reference &operator *= (const AT &at) {
            expression ().operator *= (at);
            return *this;
        }
        template<class AT>
        BOOST_UBLAS_INLINE
        vector_reference &operator /= (const AT &at) {
            expression ().operator /= (at);
            return *this;
        }

        // Swapping
        BOOST_UBLAS_INLINE
        void swap (vector_reference &v) {
            if (!same_closure (*this, v)) {
                expression ().swap (v.expression ());
            }
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const vector_reference &vr) const {
            return &(*this).e_ == &vr.e_;
        }

        // Iterator types
#ifndef BOOST_UBLAS_CT_REFERENCE_BASE_TYPEDEFS
        typedef typename E::const_iterator const_iterator;
        typedef typename E::iterator iterator;
#else
        typedef typename E::const_iterator const_iterator;
        typedef typename boost::mpl::if_<boost::is_const<E>,
                                          typename E::const_iterator,
                                          typename E::iterator>::type iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            return expression ().find (i);
        }
        BOOST_UBLAS_INLINE
        iterator find (size_type i) {
            return expression ().find (i);
        }

        // Iterator is the iterator of the referenced expression.

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return expression ().begin ();
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return expression ().end ();
        }

        BOOST_UBLAS_INLINE
        iterator begin () {
            return expression ().begin ();
        }
        BOOST_UBLAS_INLINE
        iterator end () {
            return expression ().end ();
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<iterator, value_type, reference> reverse_iterator;
#else
        typedef reverse_iterator_base<iterator> reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        reverse_iterator rbegin () {
            return reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        reverse_iterator rend () {
            return reverse_iterator (begin ());
        }

    private:
        refered_type &e_;
        static refered_type nil_;
    };

    template<class E>
    typename vector_reference<E>::refered_type vector_reference<E>::nil_
#ifdef BOOST_UBLAS_STATIC_OLD_INIT
        = BOOST_UBLAS_TYPENAME vector_reference<E>::refered_type ();
#endif
    ;

    template<class E, class F>
    class vector_unary:
        public vector_expression<vector_unary<E, F> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<vector_unary<E, F> >::operator ();
#endif
        typedef typename boost::mpl::if_<boost::is_same<F,
                                                         scalar_identity<typename E::value_type> >,
                                          E,
                                          const E>::type expression_type;
        typedef typename E::size_type size_type;
        typedef typename E::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef value_type const_reference;
        typedef typename boost::mpl::if_<boost::is_same<F,
                                                         scalar_identity<value_type> >,
                                          typename E::reference,
                                          value_type>::type reference;
    private:
        typedef const value_type *const_pointer;
        typedef F functor_type;
        typedef typename boost::mpl::if_<boost::is_const<expression_type>,
                                          typename E::const_closure_type,
                                          typename E::closure_type>::type expression_closure_type;
        typedef vector_unary<E, F> self_type;
    public:
        typedef const self_type const_closure_type;
        typedef self_type closure_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_unary ():
            e_ () {}
        BOOST_UBLAS_INLINE
        // ISSUE may be used as mutable expression.
        // vector_unary (const expression_type &e):
        vector_unary (expression_type &e):
            e_ (e) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return e_.size ();
        }

#ifndef BOOST_UBLAS_NESTED_CLASS_DR45
    private:
#endif
        // Expression accessors
        BOOST_UBLAS_INLINE
        const expression_closure_type &expression () const {
            return e_;
        }

    public:
        // Element access
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return functor_type () (e_ (i));
        }
        BOOST_UBLAS_INLINE
        reference operator () (size_type i) {
            BOOST_STATIC_ASSERT ((boost::is_same<functor_type, scalar_identity<value_type > >::value));
            return e_ (i);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return functor_type () (e_ [i]);
        }
        BOOST_UBLAS_INLINE
        reference operator [] (size_type i) {
            BOOST_STATIC_ASSERT ((boost::is_same<functor_type, scalar_identity<value_type > >::value));
            return e_ [i];
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const vector_unary &vu) const {
            return (*this).expression ().same_closure (vu.expression ());
        }

        // Iterator types
    private:
        typedef typename E::const_iterator const_iterator_type;

    public:
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<const_closure_type, typename const_iterator_type::iterator_category> const_iterator;
        typedef const_iterator iterator;
#else
        class const_iterator;
        typedef const_iterator iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            const_iterator_type it (e_.find (i));
            return const_iterator (*this, it.index ());
#else
            return const_iterator (*this, e_.find (i));
#endif
        }

        // Iterator enhances the iterator of the referenced expression
        // with the unary functor.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_unary>,
#ifndef BOOST_UBLAS_NO_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename E::const_iterator::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename E::const_iterator::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename E::const_iterator::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename vector_unary::difference_type difference_type;
            typedef typename vector_unary::value_type value_type;
            typedef typename vector_unary::const_reference reference;
            typedef typename vector_unary::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &vu, const const_iterator_type &it):
                container_const_reference<self_type> (vu), it_ (it) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return it_ - it.it_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            const_reference operator * () const {
                return functor_type () (*it_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it_ = it.it_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return it_ == it.it_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return it_ < it.it_;
            }

        private:
            const_iterator_type it_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0); 
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        expression_closure_type e_;
    };

    template<class E, class F>
    struct vector_unary_traits {
        typedef vector_unary<E, F> expression_type;
//FIXME
// #ifndef BOOST_UBLAS_SIMPLE_ET_DEBUG
        typedef expression_type result_type;
// #else
//         typedef vector<typename F::result_type> result_type;
// #endif
    };

    // (- v) [i] = - v [i]
    template<class E> 
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_negate<typename E::value_type> >::result_type
    operator - (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_negate<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (conj v) [i] = conj (v [i])
    template<class E> 
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_conj<typename E::value_type> >::result_type
    conj (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_conj<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (real v) [i] = real (v [i])
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_real<typename E::value_type> >::result_type
    real (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_real<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (imag v) [i] = imag (v [i])
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_imag<typename E::value_type> >::result_type
    imag (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_imag<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (trans v) [i] = v [i]
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<const E, scalar_identity<typename E::value_type> >::result_type
    trans (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<const E, scalar_identity<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_identity<typename E::value_type> >::result_type
    trans (vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_identity<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // (herm v) [i] = conj (v [i])
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_unary_traits<E, scalar_conj<typename E::value_type> >::result_type
    herm (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_unary_traits<E, scalar_conj<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E1, class E2, class F>
    class vector_binary:
        public vector_expression<vector_binary<E1, E2, F> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<vector_binary<E1, E2, F> >::operator ();
#endif
        typedef typename promote_traits<typename E1::size_type, typename E2::size_type>::promote_type size_type;
        typedef typename promote_traits<typename E1::difference_type, typename E2::difference_type>::promote_type difference_type;
        typedef typename F::result_type value_type;
        typedef value_type const_reference;
        typedef const_reference reference;
    private:
        typedef const value_type *const_pointer;
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E1::const_closure_type expression1_closure_type;
        typedef typename E2::const_closure_type expression2_closure_type;
        typedef vector_binary<E1, E2, F> self_type;
    public:
        typedef const self_type const_closure_type;
        typedef const_closure_type closure_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_binary ():
            e1_ (), e2_ () {}
        BOOST_UBLAS_INLINE
        vector_binary (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const { 
            return BOOST_UBLAS_SAME (e1_.size (), e2_.size ()); 
        }

    private:
        // Expression accesors
        BOOST_UBLAS_INLINE
        const expression1_closure_type &expression1 () const {
            return e1_;
        }
        BOOST_UBLAS_INLINE
        const expression2_closure_type &expression2 () const {
            return e2_;
        }

    public:
        // Element access
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return functor_type () (e1_ (i), e2_ (i));
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return functor_type () (e1_ [i], e2_ [i]);
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const vector_binary &vb) const {
            return (*this).expression1 ().same_closure (vb.expression1 ()) &&
                   (*this).expression2 ().same_closure (vb.expression2 ());
        }

        // Iterator types
    private:
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;
    public:
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef typename iterator_restrict_traits<typename const_iterator1_type::iterator_category,
                                                  typename const_iterator2_type::iterator_category>::iterator_category iterator_category;
        typedef indexed_const_iterator<const_closure_type, iterator_category> const_iterator;
        typedef const_iterator iterator;
#else
        class const_iterator;
        typedef const_iterator iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
            const_iterator1_type it1 (e1_.find (i));
            const_iterator1_type it1_end (e1_.find (size ()));
            const_iterator2_type it2 (e2_.find (i));
            const_iterator2_type it2_end (e2_.find (size ()));
            i = (std::min) (it1 != it1_end ? it1.index () : size (),
                          it2 != it2_end ? it2.index () : size ());
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            return const_iterator (*this, i);
#else
            return const_iterator (*this, i, it1, it1_end, it2, it2_end);
#endif
        }

        // Iterator merges the iterators of the referenced expressions and
        // enhances them with the binary functor.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_binary>,
#ifndef BOOST_UBLAS_NO_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename iterator_restrict_traits<typename E1::const_iterator::iterator_category,
                                                                          typename E2::const_iterator::iterator_category>::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename iterator_restrict_traits<typename E1::const_iterator::iterator_category,
                                                                                 typename E2::const_iterator::iterator_category>::iterator_category,
                                               const_iterator, value_type> {
#endif
        public:
            typedef typename iterator_restrict_traits<typename E1::const_iterator::iterator_category,
                                                      typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename vector_binary::difference_type difference_type;
            typedef typename vector_binary::value_type value_type;
            typedef typename vector_binary::const_reference reference;
            typedef typename vector_binary::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), i_ (), it1_ (), it1_end_ (), it2_ (), it2_end_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &vb, size_type i,
                            const const_iterator1_type &it1, const const_iterator1_type &it1_end,
                            const const_iterator2_type &it2, const const_iterator2_type &it2_end):
                container_const_reference<self_type> (vb), i_ (i), it1_ (it1), it1_end_ (it1_end), it2_ (it2), it2_end_ (it2_end) {}

            // Dense specializations
            BOOST_UBLAS_INLINE
            void increment (dense_random_access_iterator_tag) {
                ++ i_, ++ it1_, ++ it2_;
            }
            BOOST_UBLAS_INLINE
            void decrement (dense_random_access_iterator_tag) {
                -- i_, -- it1_, -- it2_;
            }
            BOOST_UBLAS_INLINE
            value_type dereference (dense_random_access_iterator_tag) const {
                return functor_type () (*it1_, *it2_);
            }

            // Packed specializations
            BOOST_UBLAS_INLINE
            void increment (packed_random_access_iterator_tag) {
                if (it1_ != it1_end_)
                    if (it1_.index () <= i_)
                        ++ it1_;
                if (it2_ != it2_end_)
                    if (it2_.index () <= i_)
                        ++ it2_;
                ++ i_;
            }
            BOOST_UBLAS_INLINE
            void decrement (packed_random_access_iterator_tag) {
                if (it1_ != it1_end_)
                    if (i_ <= it1_.index ())
                        -- it1_;
                if (it2_ != it2_end_)
                    if (i_ <= it2_.index ())
                        -- it2_;
                -- i_;
            }
            BOOST_UBLAS_INLINE
            value_type dereference (packed_random_access_iterator_tag) const {
                value_type t1 = value_type (0);
                if (it1_ != it1_end_)
                    if (it1_.index () == i_)
                        t1 = *it1_;
                value_type t2 = value_type (0);
                if (it2_ != it2_end_)
                    if (it2_.index () == i_)
                        t2 = *it2_;
                return functor_type () (t1, t2);
            }

            // Sparse specializations
            BOOST_UBLAS_INLINE
            void increment (sparse_bidirectional_iterator_tag) {
                size_type index1 = (*this) ().size ();
                if (it1_ != it1_end_) {
                    if  (it1_.index () <= i_)
                        ++ it1_;
                    if (it1_ != it1_end_)
                        index1 = it1_.index ();
                }
                size_type index2 = (*this) ().size ();
                if (it2_ != it2_end_) {
                    if (it2_.index () <= i_)
                        ++ it2_;
                    if (it2_ != it2_end_)
                        index2 = it2_.index ();
                }
                i_ = (std::min) (index1, index2);
            }
            BOOST_UBLAS_INLINE
            void decrement (sparse_bidirectional_iterator_tag) {
                size_type index1 = (*this) ().size ();
                if (it1_ != it1_end_) {
                    if (i_ <= it1_.index ())
                        -- it1_;
                    if (it1_ != it1_end_)
                        index1 = it1_.index ();
                }
                size_type index2 = (*this) ().size ();
                if (it2_ != it2_end_) {
                    if (i_ <= it2_.index ())
                        -- it2_;
                    if (it2_ != it2_end_)
                        index2 = it2_.index ();
                }
                i_ = (std::max) (index1, index2);
            }
            BOOST_UBLAS_INLINE
            value_type dereference (sparse_bidirectional_iterator_tag) const {
                value_type t1 = value_type (0);
                if (it1_ != it1_end_)
                    if (it1_.index () == i_)
                        t1 = *it1_;
                value_type t2 = value_type (0);
                if (it2_ != it2_end_)
                    if (it2_.index () == i_)
                        t2 = *it2_;
                return functor_type () (t1, t2);
            }

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                increment (iterator_category ());
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                decrement (iterator_category ());
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                i_ += n, it1_ += n, it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                i_ -= n, it1_ -= n, it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return index () - it.index ();
            }

            // Dereference
            BOOST_UBLAS_INLINE
            const_reference operator * () const {
                return dereference (iterator_category ());
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return i_;
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                i_ = it.i_;
                it1_ = it.it1_;
                it1_end_ = it.it1_end_;
                it2_ = it.it2_;
                it2_end_ = it.it2_end_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return index () == it.index ();
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                return index () < it.index ();
            }

        private:
            size_type i_;
            const_iterator1_type it1_;
            const_iterator1_type it1_end_;
            const_iterator2_type it2_;
            const_iterator2_type it2_end_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        expression1_closure_type e1_;
        expression2_closure_type e2_;
    };

    template<class E1, class E2, class F>
    struct vector_binary_traits {
        typedef vector_binary<E1, E2, F> expression_type;
#ifndef BOOST_UBLAS_SIMPLE_ET_DEBUG
        typedef expression_type result_type; 
#else
        typedef vector<typename F::result_type> result_type;
#endif
    };

    // (v1 + v2) [i] = v1 [i] + v2 [i]
    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_binary_traits<E1, E2, scalar_plus<typename E1::value_type, 
                                                      typename E2::value_type> >::result_type
    operator + (const vector_expression<E1> &e1,
                const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_traits<E1, E2, scalar_plus<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                              BOOST_UBLAS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // (v1 - v2) [i] = v1 [i] - v2 [i]
    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_binary_traits<E1, E2, scalar_minus<typename E1::value_type,
                                                       typename E2::value_type> >::result_type
    operator - (const vector_expression<E1> &e1,
                const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_traits<E1, E2, scalar_minus<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                               BOOST_UBLAS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // (v1 * v2) [i] = v1 [i] * v2 [i]
    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_binary_traits<E1, E2, scalar_multiplies<typename E1::value_type,
                                                            typename E2::value_type> >::result_type
    element_prod (const vector_expression<E1> &e1,
                  const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_traits<E1, E2, scalar_multiplies<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                    BOOST_UBLAS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    // (v1 / v2) [i] = v1 [i] / v2 [i]
    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_binary_traits<E1, E2, scalar_divides<typename E1::value_type,
                                                         typename E2::value_type> >::result_type
    element_div (const vector_expression<E1> &e1,
                 const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_traits<E1, E2, scalar_divides<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                 BOOST_UBLAS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E1, class E2, class F>
    class vector_binary_scalar1:
        public vector_expression<vector_binary_scalar1<E1, E2, F> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<vector_binary_scalar1<E1, E2, F> >::operator ();
#endif
        typedef typename E2::size_type size_type;
        typedef typename E2::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef value_type const_reference;
        typedef const_reference reference;
    private:
        typedef const value_type *const_pointer;
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef E1 expression1_closure_type;
        typedef typename E2::const_closure_type expression2_closure_type;
        typedef vector_binary_scalar1<E1, E2, F> self_type;
    public:
        typedef const self_type const_closure_type;
        typedef const_closure_type closure_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_binary_scalar1 ():
            e1_ (), e2_ () {}
        BOOST_UBLAS_INLINE
        vector_binary_scalar1 (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return e2_.size ();
        }

    private:
        // Expression accesors
        BOOST_UBLAS_INLINE
        const expression1_closure_type &expression1 () const {
            return e1_;
        }
        BOOST_UBLAS_INLINE
        const expression2_closure_type &expression2 () const {
            return e2_;
        }

    public:
        // Element access
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return functor_type () (e1_, e2_ (i));
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return functor_type () (e1_, e2_ [i]);
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const vector_binary_scalar1 &vbs1) const {
            return (*this).expression1 ().same_closure (vbs1.expression1 ()) &&
                   (*this).expression2 ().same_closure (vbs1.expression2 ());
        }

        // Iterator types
    private:
        typedef typename E1::value_type const_iterator1_type;
        typedef typename E2::const_iterator const_iterator2_type;

    public:
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<const_closure_type, typename const_iterator2_type::iterator_category> const_iterator;
        typedef const_iterator iterator;
#else
        class const_iterator;
        typedef const_iterator iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            const_iterator2_type it (e2_.find (i));
            return const_iterator (*this, it.index ());
#else
            return const_iterator (*this, e1_, e2_.find (i));
#endif
        }

        // Iterator enhances the iterator of the referenced vector expression
        // with the binary functor.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_binary_scalar1>,
#ifndef BOOST_UBLAS_NO_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename E2::const_iterator::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename E2::const_iterator::iterator_category,
                                               const_iterator, value_type> {
#endif                                               
        public:
            typedef typename E2::const_iterator::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename vector_binary_scalar1::difference_type difference_type;
            typedef typename vector_binary_scalar1::value_type value_type;
            typedef typename vector_binary_scalar1::const_reference reference;
            typedef typename vector_binary_scalar1::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &vbs, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (vbs), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it2_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it2_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it2_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ - it.it2_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            const_reference operator * () const {
                return functor_type () (it1_, *it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it2_.index ();
            }

            // Assignment 
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ == it.it2_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it1_ == it.it1_, external_logic ());
                return it2_ < it.it2_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0); 
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ()); 
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        expression1_closure_type e1_;
        expression2_closure_type e2_;
    };

    template<class T1, class E2, class F>
    struct vector_binary_scalar1_traits {
        typedef vector_binary_scalar1<scalar_const_reference<T1>, E2, F> expression_type;
#ifndef BOOST_UBLAS_SIMPLE_ET_DEBUG
        typedef expression_type result_type;
#else
        typedef vector<typename F::result_type> result_type;
#endif
    };

    // (t * v) [i] = t * v [i]
    template<class T1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_binary_scalar1_traits<T1, E2, scalar_multiplies<T1, typename E2::value_type> >::result_type
    operator * (const T1 &e1,
                const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_scalar1_traits<T1, E2, scalar_multiplies<T1, BOOST_UBLAS_TYPENAME E2::value_type> >::expression_type expression_type;
        return expression_type (e1, e2 ());
    }

    template<class E1, class E2, class F>
    class vector_binary_scalar2:
        public vector_expression<vector_binary_scalar2<E1, E2, F> > {
    public:
#ifndef BOOST_UBLAS_NO_PROXY_SHORTCUTS
        BOOST_UBLAS_USING vector_expression<vector_binary_scalar2<E1, E2, F> >::operator ();
#endif
        typedef typename E1::size_type size_type;
        typedef typename E1::difference_type difference_type;
        typedef typename F::result_type value_type;
        typedef value_type const_reference;
        typedef const_reference reference;
    private:
        typedef const value_type *const_pointer;
        typedef const_pointer pointer;
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E1::const_closure_type expression1_closure_type;
        typedef E2 expression2_closure_type;
        typedef vector_binary_scalar2<E1, E2, F> self_type;
    public:
        typedef const self_type const_closure_type;
        typedef const_closure_type closure_type;
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_binary_scalar2 ():
            e1_ (), e2_ () {}
        BOOST_UBLAS_INLINE
        vector_binary_scalar2 (const expression1_type &e1, const expression2_type &e2):
            e1_ (e1), e2_ (e2) {}

        // Accessors
        BOOST_UBLAS_INLINE
        size_type size () const {
            return e1_.size (); 
        }

    private:
        // Expression accesors
        BOOST_UBLAS_INLINE
        const expression1_closure_type &expression1 () const {
            return e1_;
        }
        BOOST_UBLAS_INLINE
        const expression2_closure_type &expression2 () const {
            return e2_;
        }

    public:
        // Element access
        BOOST_UBLAS_INLINE
        const_reference operator () (size_type i) const {
            return functor_type () (e1_ (i), e2_);
        }

        BOOST_UBLAS_INLINE
        const_reference operator [] (size_type i) const {
            return functor_type () (e1_ [i], e2_);
        }

        // Closure comparison
        BOOST_UBLAS_INLINE
        bool same_closure (const vector_binary_scalar2 &vbs2) const {
            return (*this).expression1 ().same_closure (vbs2.expression1 ()) &&
                   (*this).expression2 ().same_closure (vbs2.expression2 ());
        }

        // Iterator types
    private:
        typedef typename E1::const_iterator const_iterator1_type;
        typedef typename E2::value_type const_iterator2_type;

    public:
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
        typedef indexed_const_iterator<const_closure_type, typename const_iterator2_type::iterator_category> const_iterator;
        typedef const_iterator iterator;
#else
        class const_iterator;
        typedef const_iterator iterator;
#endif

        // Element lookup
        BOOST_UBLAS_INLINE
        const_iterator find (size_type i) const {
#ifdef BOOST_UBLAS_USE_INDEXED_ITERATOR
            const_iterator1_type it (e1_.find (i));
            return const_iterator (*this, it.index ());
#else
            return const_iterator (*this, e1_.find (i), e2_);
#endif
        }

        // Iterator enhances the iterator of the referenced vector expression
        // with the binary functor.

#ifndef BOOST_UBLAS_USE_INDEXED_ITERATOR
        class const_iterator:
            public container_const_reference<vector_binary_scalar2>,
#ifndef BOOST_UBLAS_NO_ITERATOR_BASE_TRAITS
            public iterator_base_traits<typename E1::const_iterator::iterator_category>::template
                        iterator_base<const_iterator, value_type>::type {
#else
            public random_access_iterator_base<typename E1::const_iterator::iterator_category,
                                               const_iterator, value_type> {
#endif                                               
        public:
            typedef typename E1::const_iterator::iterator_category iterator_category;
#ifdef BOOST_MSVC_STD_ITERATOR
            typedef const_reference reference;
#else
            typedef typename vector_binary_scalar2::difference_type difference_type;
            typedef typename vector_binary_scalar2::value_type value_type;
            typedef typename vector_binary_scalar2::const_reference reference;
            typedef typename vector_binary_scalar2::const_pointer pointer;
#endif

            // Construction and destruction
            BOOST_UBLAS_INLINE
            const_iterator ():
                container_const_reference<self_type> (), it1_ (), it2_ () {}
            BOOST_UBLAS_INLINE
            const_iterator (const self_type &vbs, const const_iterator1_type &it1, const const_iterator2_type &it2):
                container_const_reference<self_type> (vbs), it1_ (it1), it2_ (it2) {}

            // Arithmetic
            BOOST_UBLAS_INLINE
            const_iterator &operator ++ () {
                ++ it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -- () {
                -- it1_;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator += (difference_type n) {
                it1_ += n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            const_iterator &operator -= (difference_type n) {
                it1_ -= n;
                return *this;
            }
            BOOST_UBLAS_INLINE
            difference_type operator - (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ - it.it1_;
            }

            // Dereference
            BOOST_UBLAS_INLINE
            const_reference operator * () const {
                return functor_type () (*it1_, it2_);
            }

            // Index
            BOOST_UBLAS_INLINE
            size_type index () const {
                return it1_.index ();
            }

            // Assignment
            BOOST_UBLAS_INLINE
            const_iterator &operator = (const const_iterator &it) {
                container_const_reference<self_type>::assign (&it ());
                it1_ = it.it1_;
                it2_ = it.it2_;
                return *this;
            }

            // Comparison
            BOOST_UBLAS_INLINE
            bool operator == (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ == it.it1_;
            }
            BOOST_UBLAS_INLINE
            bool operator < (const const_iterator &it) const {
                BOOST_UBLAS_CHECK ((*this) ().same_closure (it ()), external_logic ());
                // FIXME: we shouldn't compare floats
                // BOOST_UBLAS_CHECK (it2_ == it.it2_, external_logic ());
                return it1_ < it.it1_;
            }

        private:
            const_iterator1_type it1_;
            const_iterator2_type it2_;
        };
#endif

        BOOST_UBLAS_INLINE
        const_iterator begin () const {
            return find (0);
        }
        BOOST_UBLAS_INLINE
        const_iterator end () const {
            return find (size ());
        }

        // Reverse iterator

#ifdef BOOST_MSVC_STD_ITERATOR
        typedef reverse_iterator_base<const_iterator, value_type, const_reference> const_reverse_iterator;
#else
        typedef reverse_iterator_base<const_iterator> const_reverse_iterator;
#endif

        BOOST_UBLAS_INLINE
        const_reverse_iterator rbegin () const {
            return const_reverse_iterator (end ());
        }
        BOOST_UBLAS_INLINE
        const_reverse_iterator rend () const {
            return const_reverse_iterator (begin ());
        }

    private:
        expression1_closure_type e1_;
        expression2_closure_type e2_;
    };

    template<class E1, class T2, class F>
    struct vector_binary_scalar2_traits {
        typedef vector_binary_scalar2<E1, scalar_const_reference<T2>, F> expression_type;
#ifndef BOOST_UBLAS_SIMPLE_ET_DEBUG
        typedef expression_type result_type;
#else
        typedef vector<typename F::result_type> result_type;
#endif
    };

    // (v * t) [i] = v [i] * t
    template<class E1, class T2>
    BOOST_UBLAS_INLINE
    typename vector_binary_scalar2_traits<E1, T2, scalar_multiplies<typename E1::value_type, T2> >::result_type
    operator * (const vector_expression<E1> &e1,
                const T2 &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_scalar2_traits<E1, T2, scalar_multiplies<BOOST_UBLAS_TYPENAME E1::value_type, T2> >::expression_type expression_type;
        return expression_type (e1 (), e2);
    }

    // (v / t) [i] = v [i] / t
    template<class E1, class T2>
    BOOST_UBLAS_INLINE
    typename vector_binary_scalar2_traits<E1, T2, scalar_divides<typename E1::value_type, T2> >::result_type
    operator / (const vector_expression<E1> &e1,
                const T2 &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_binary_scalar2_traits<E1, T2, scalar_divides<BOOST_UBLAS_TYPENAME E1::value_type, T2> >::expression_type expression_type;
        return expression_type (e1 (), e2);
    }

    template<class E, class F>
    class vector_scalar_unary:
        public scalar_expression<typename F::result_type> {
    public:
        typedef typename F::size_type size_type;
        typedef typename F::difference_type difference_type;
        typedef typename F::result_type value_type;
    private:
        typedef E expression_type;
        typedef F functor_type;
        typedef typename E::const_closure_type expression_closure_type;
        typedef typename E::const_iterator::iterator_category iterator_category;
    public:
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_scalar_unary ():
            e_ () {}
        BOOST_UBLAS_INLINE
        vector_scalar_unary (const expression_type &e):
            e_ (e) {}

#ifndef BOOST_UBLAS_NESTED_CLASS_DR45
    private:
#endif
        // Expression accessors
        BOOST_UBLAS_INLINE
        const expression_closure_type &expression () const {
            return e_;
        }

    public:
        BOOST_UBLAS_INLINE
        operator value_type () const {
            return evaluate (iterator_category ());
        }

    private:
        // Dense random access specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (dense_random_access_iterator_tag) const {
#ifdef BOOST_UBLAS_USE_INDEXING
            return functor_type () (e_);
#elif BOOST_UBLAS_USE_ITERATING
            difference_type size = e_.size ();
            return functor_type () (size, e_.begin ());
#else
            difference_type size = e_.size ();
            if (size >= BOOST_UBLAS_ITERATOR_THRESHOLD)
                return functor_type () (size, e_.begin ());
            else
                return functor_type () (e_);
#endif
        }

        // Packed bidirectional specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (packed_random_access_iterator_tag) const {
            return functor_type () (e_.begin (), e_.end ());
        }

        // Sparse bidirectional specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (sparse_bidirectional_iterator_tag) const {
            return functor_type () (e_.begin (), e_.end ());
        }

    private:
        expression_closure_type e_;
    };

    template<class E, class F>
    struct vector_scalar_unary_traits {
        typedef vector_scalar_unary<E, F> expression_type;
#if !defined (BOOST_UBLAS_SIMPLE_ET_DEBUG) && defined (BOOST_UBLAS_USE_SCALAR_ET)
// FIXME don't define USE_SCALAR_ET other then for testing
// They do not work for complex types
         typedef expression_type result_type;
#else
         typedef typename F::result_type result_type;
#endif
    };

    // sum v = sum (v [i])
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_scalar_unary_traits<E, vector_sum<typename E::value_type> >::result_type
    sum (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_unary_traits<E, vector_sum<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // real: norm_1 v = sum (abs (v [i]))
    // complex: norm_1 v = sum (abs (real (v [i])) + abs (imag (v [i])))
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_1<typename E::value_type> >::result_type
    norm_1 (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_unary_traits<E, vector_norm_1<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // real: norm_2 v = sqrt (sum (v [i] * v [i]))
    // complex: norm_2 v = sqrt (sum (v [i] * conj (v [i])))
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_2<typename E::value_type> >::result_type
    norm_2 (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_unary_traits<E, vector_norm_2<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // real: norm_inf v = max (abs (v [i]))
    // complex: norm_inf v = max (max (abs (real (v [i])), abs (imag (v [i]))))
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_scalar_unary_traits<E, vector_norm_inf<typename E::value_type> >::result_type
    norm_inf (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_unary_traits<E, vector_norm_inf<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    // real: index_norm_inf v = min (i: abs (v [i]) == max (abs (v [i])))
    template<class E>
    BOOST_UBLAS_INLINE
    typename vector_scalar_unary_traits<E, vector_index_norm_inf<typename E::value_type> >::result_type
    index_norm_inf (const vector_expression<E> &e) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_unary_traits<E, vector_index_norm_inf<BOOST_UBLAS_TYPENAME E::value_type> >::expression_type expression_type;
        return expression_type (e ());
    }

    template<class E1, class E2, class F>
    class vector_scalar_binary:
        public scalar_expression<typename F::result_type> {
    public:
        BOOST_STATIC_CONSTANT (unsigned, complexity = 1);
        typedef typename F::size_type size_type;
        typedef typename F::difference_type difference_type;
        typedef typename F::result_type value_type;
    private:
        typedef E1 expression1_type;
        typedef E2 expression2_type;
        typedef F functor_type;
        typedef typename E1::const_closure_type expression1_closure_type;
        typedef typename E2::const_closure_type expression2_closure_type;
        typedef typename iterator_restrict_traits<typename E1::const_iterator::iterator_category,
                                                  typename E2::const_iterator::iterator_category>::iterator_category iterator_category;
    public:
        typedef unknown_storage_tag storage_category;

        // Construction and destruction
        BOOST_UBLAS_INLINE
        vector_scalar_binary ():
            e1_ (), e2_ () {}
        BOOST_UBLAS_INLINE
        vector_scalar_binary (const expression1_type &e1, const expression2_type  &e2):
            e1_ (e1), e2_ (e2) {}

    private:
        // Accessors
        BOOST_UBLAS_INLINE
        const expression1_closure_type &expression1 () const {
            return e1_;
        }
        BOOST_UBLAS_INLINE
        const expression2_closure_type &expression2 () const {
            return e2_;
        }

    public:
        BOOST_UBLAS_INLINE
        operator value_type () const {
            return evaluate (iterator_category ());
        }

    private:
        // Dense random access specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (dense_random_access_iterator_tag) const {
#ifdef BOOST_UBLAS_USE_INDEXING
            return functor_type () (e1_, e2_);
#elif BOOST_UBLAS_USE_ITERATING
            difference_type size = BOOST_UBLAS_SAME (e1_.size (), e2_.size ());
            return functor_type () (size, e1_.begin (), e2_.begin ());
#else
            difference_type size = BOOST_UBLAS_SAME (e1_.size (), e2_.size ());
            if (size >= BOOST_UBLAS_ITERATOR_THRESHOLD)
                return functor_type () (size, e1_.begin (), e2_.begin ());
            else
                return functor_type () (e1_, e2_);
#endif
        }

        // Packed bidirectional specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (packed_random_access_iterator_tag) const {
            return functor_type () (e1_.begin (), e1_.end (), e2_.begin (), e2_.end ());
        }

        // Sparse bidirectional specialization
        BOOST_UBLAS_INLINE
        value_type evaluate (sparse_bidirectional_iterator_tag) const {
            return functor_type () (e1_.begin (), e1_.end (), e2_.begin (), e2_.end (), sparse_bidirectional_iterator_tag ());
        }

    private:
        expression1_closure_type e1_;
        expression2_closure_type e2_;
    };

    template<class E1, class E2, class F>
    struct vector_scalar_binary_traits {
        typedef vector_scalar_binary<E1, E2, F> expression_type;
#if !defined (BOOST_UBLAS_SIMPLE_ET_DEBUG) && defined (BOOST_UBLAS_USE_SCALAR_ET)
// FIXME don't define USE_SCALAR_ET other then for testing
// They do not work for complex types
        typedef expression_type result_type;
#else
        typedef typename F::result_type result_type;
#endif
    };

    // inner_prod (v1, v2) = sum (v1 [i] * v2 [i])
    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_scalar_binary_traits<E1, E2, vector_inner_prod<typename E1::value_type,
                                                                   typename E2::value_type,
                                                                   typename promote_traits<typename E1::value_type,
                                                                                           typename E2::value_type>::promote_type> >::result_type
    inner_prod (const vector_expression<E1> &e1,
                const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_binary_traits<E1, E2, vector_inner_prod<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                           BOOST_UBLAS_TYPENAME E2::value_type,
                                                                                           BOOST_UBLAS_TYPENAME promote_traits<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                                                               BOOST_UBLAS_TYPENAME E2::value_type>::promote_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    typename vector_scalar_binary_traits<E1, E2, vector_inner_prod<typename E1::value_type,
                                                                   typename E2::value_type,
                                                                   typename type_traits<typename promote_traits<typename E1::value_type,
                                                                                                                typename E2::value_type>::promote_type>::precision_type> >::result_type
    prec_inner_prod (const vector_expression<E1> &e1,
                     const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME vector_scalar_binary_traits<E1, E2, vector_inner_prod<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                           BOOST_UBLAS_TYPENAME E2::value_type,
                                                                                           BOOST_UBLAS_TYPENAME type_traits<BOOST_UBLAS_TYPENAME promote_traits<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                                                                                                BOOST_UBLAS_TYPENAME E2::value_type>::promote_type>::precision_type> >::expression_type expression_type;
        return expression_type (e1 (), e2 ());
    }

}}}

#endif
