// (C) Copyright David Abrahams 2000. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_ITERATOR_ADAPTOR_DWA053000_HPP_
#define BOOST_ITERATOR_ADAPTOR_DWA053000_HPP_

#include <boost/iterator.hpp>
#include <boost/utility.hpp>
#include <boost/compressed_pair.hpp>
#include <boost/concept_check.hpp>

// I was having some problems with VC6. I couldn't tell whether our hack for
// stock GCC was causing problems so I needed an easy way to turn it on and
// off. Now we can test the hack with various compilers and still have an 
// "out" if it doesn't work. -dwa 7/31/00
#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96 && !defined(__STL_USE_NAMESPACES)
# define BOOST_RELOPS_AMBIGUITY_BUG 1
#endif

namespace boost {

// Just a "type envelope"; works around some MSVC deficiencies.
template <class T>
struct type {};


//============================================================================
// Concept checking classes that express the requirements for iterator
// policies and adapted types. These classes are mostly for
// documentation purposes, and are not used in this header file. They
// merely provide a more succinct statement of what is expected of the
// iterator policies.

template <class Policies, class Adapted, class Traits>
struct TrivialIteratorPoliciesConcept
{
  typedef typename Traits::reference Reference;
  void constraints() {
    function_requires< AssignableConcept<Policies> >();
    function_requires< DefaultConstructibleConcept<Policies> >();
    function_requires< AssignableConcept<Adapted> >();
    function_requires< DefaultConstructibleConcept<Adapted> >();

    const_constraints();
  }
  void const_constraints() const {
    Reference r = p.dereference(type<Reference>(), x);
    b = p.equal(x, x);
    ignore_unused_variable_warning(r);
  }
  Policies p;
  Adapted x;
  mutable bool b;
};

// Add InputIteratorPoliciesConcept?

template <class Policies, class Adapted, class Traits>
struct ForwardIteratorPoliciesConcept
{
  typedef typename Traits::iterator_category iterator_category;
  void constraints() {
    function_requires< 
      TrivialIteratorPoliciesConcept<Policies, Adapted, Traits>
      >();

    p.increment(x);
    std::forward_iterator_tag t = iterator_category();
    ignore_unused_variable_warning(t);
  }
  Policies p;
  Adapted x;
  iterator_category category;
};

template <class Policies, class Adapted, class Traits>
struct BidirectionalIteratorPoliciesConcept
{
  typedef typename Traits::iterator_category iterator_category;
  void constraints() {
    function_requires< 
      ForwardIteratorPoliciesConcept<Policies, Adapted, Traits>
      >();

    p.decrement(x);
    std::bidirectional_iterator_tag t = iterator_category();
    ignore_unused_variable_warning(t);
  }
  Policies p;
  Adapted x;
};

template <class Policies, class Adapted, class Traits>
struct RandomAccessIteratorPoliciesConcept
{
  typedef typename Traits::difference_type DifferenceType;
  typedef typename Traits::iterator_category iterator_category;
  void constraints() {
    function_requires< 
      BidirectionalIteratorPoliciesConcept<Policies, Adapted, Traits>
      >();

    p.advance(x, n);
    std::random_access_iterator_tag t = iterator_category();
    const_constraints();
    ignore_unused_variable_warning(t);
  }
  void const_constraints() const {
    n = p.distance(type<DifferenceType>(), x, x);
    b = p.less(x, x);
  }
  Policies p;
  Adapted x;
  mutable DifferenceType n;
  mutable bool b;
};


//============================================================================
// Default policies for iterator adaptors. You can use this as a base
// class if you want to customize particular policies.
struct default_iterator_policies
{
    // Some of these members were defined static, but Borland got confused
    // and thought they were non-const. Also, Sun C++ does not like static
    // function templates.

    template <class Reference, class Iterator>
    Reference dereference(type<Reference>, const Iterator& x) const
        { return *x; }

    template <class Iterator>
    void increment(Iterator& x)
        { ++x; }

    template <class Iterator>
    void decrement(Iterator& x)
        { --x; }

    template <class Iterator, class DifferenceType>
    void advance(Iterator& x, DifferenceType n)
        { x += n; }

    template <class Difference, class Iterator1, class Iterator2>
    Difference distance(type<Difference>, const Iterator1& x,
                        const Iterator2& y) const
        { return y - x; }

    template <class Iterator1, class Iterator2>
    bool equal(const Iterator1& x, const Iterator2& y) const
        { return x == y; }

    template <class Iterator1, class Iterator2>
    bool less(const Iterator1& x, const Iterator2& y) const
        { return x < y; }
};

// putting the comparisons in a base class avoids the g++ 
// ambiguous overload bug due to the relops operators

#ifdef BOOST_RELOPS_AMBIGUITY_BUG
template <class Derived, class Base>
struct iterator_comparisons : Base { };

template <class D1, class D2, class Base1, class Base2>
inline bool operator==(const iterator_comparisons<D1,Base1>& xb, 
                       const iterator_comparisons<D2,Base2>& yb)
{
        const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return x.policies().equal(x.iter(), y.iter());
}

template <class D1, class D2, class Base1, class Base2>
inline bool operator!=(const iterator_comparisons<D1,Base1>& xb, 
                       const iterator_comparisons<D2,Base2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return !x.policies().equal(x.iter(), y.iter());
}

template <class D1, class D2, class Base1, class Base2>
inline bool operator<(const iterator_comparisons<D1,Base1>& xb, 
                      const iterator_comparisons<D2,Base2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return x.policies().less(x.iter(), y.iter());
}

template <class D1, class D2, class Base1, class Base2>
inline bool operator>(const iterator_comparisons<D1,Base1>& xb, 
                      const iterator_comparisons<D2,Base2>& yb)
{ 
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return x.policies().less(y.iter(), x.iter());
}

template <class D1, class D2, class Base1, class Base2>
inline bool operator>=(const iterator_comparisons<D1,Base1>& xb, 
                       const iterator_comparisons<D2,Base2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return !x.policies().less(x.iter(), y.iter());
}

template <class D1, class D2, class Base1, class Base2>
inline bool operator<=(const iterator_comparisons<D1,Base1>& xb, 
                       const iterator_comparisons<D2,Base2>& yb)
{
    const D1& x = static_cast<const D1&>(xb);
    const D2& y = static_cast<const D2&>(yb);
    return !x.policies().less(y.iter(), x.iter());
}
#endif

//============================================================================
// Some compilers (SGI MIPSpro 7.1.3.3) instantiate/compile member functions
// whether or not they are used. The following functions make sure that
// when the base iterators do not support particular operators, those
// operators do not get used.

namespace detail {

  // Dummy versions for iterators that don't support member access
  template <class Iter>
  inline typename Iter::pointer
  operator_arrow(const Iter&, std::input_iterator_tag) {
    return typename Iter::pointer();
  }
  template <class Iter>
  inline typename Iter::pointer
  operator_arrow(const Iter&, std::output_iterator_tag) {
    return typename Iter::pointer();
  }

  // Real version
  template <class Iter>
  inline typename Iter::pointer
  operator_arrow(const Iter& i, std::forward_iterator_tag) {
    return &(*i);
  }

  // Dummy version for non-random access iterators 
  template <class Iter, class Diff>
  inline void advance_impl(Iter&, Diff, std::input_iterator_tag) { }
  template <class Iter, class Diff>
  inline void advance_impl(Iter&, Diff, std::output_iterator_tag) { }

  // Real version
  template <class Iter, class Diff>
  inline void
  advance_impl(Iter& i, Diff n, std::random_access_iterator_tag) {
#ifdef __MWERKS__
        i.policies().advance<Iter>(i.iter(), n);
#else
        i.policies().advance(i.iter(), n);
#endif
  }

  // Dummy versions for non-bidirectional iterators 
  template <class Iter>
  inline void decrement_impl(Iter&, std::input_iterator_tag) { }
  template <class Iter>
  inline void decrement_impl(Iter&, std::output_iterator_tag) { }

  // Real version
  template <class Iter>
  inline void
  decrement_impl(Iter& i, std::bidirectional_iterator_tag) {
#ifdef __MWERKS__
        i.policies().decrement<Iter>(i.iter());
#else
        i.policies().decrement(i.iter());
#endif
  }

} // namespace detail

//============================================================================
// iterator_adaptor - A generalized adaptor around an existing
//   iterator, which is itself an iterator
//
//   Iterator - the iterator type being wrapped.
//
//   Policies - a set of policies determining how the resulting iterator
//      works.
//
//   Traits - a class satisfying the same requirements as a specialization of
//      std::iterator_traits for the resulting iterator.
//
template <class Iterator, class Policies, 
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
          class Traits
#else
          class Traits = std::iterator_traits<Iterator>
#endif
         >
struct iterator_adaptor :
#ifdef BOOST_RELOPS_AMBIGUITY_BUG
    iterator_comparisons<
          iterator_adaptor<Iterator,Policies,Traits>,
#endif
          boost::iterator<typename Traits::iterator_category, 
              typename Traits::value_type, typename Traits::difference_type,
              typename Traits::pointer, typename Traits::reference>
#ifdef BOOST_RELOPS_AMBIGUITY_BUG
>
#endif
{
    typedef iterator_adaptor<Iterator, Policies, Traits> Self;
public:
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::value_type value_type;
    typedef typename Traits::pointer pointer;
    typedef typename Traits::reference reference;
    typedef typename Traits::iterator_category iterator_category;
    typedef Iterator iterator_type;

    iterator_adaptor() { }

    iterator_adaptor(const Iterator& iter, const Policies& p = Policies())
        : m_iter_p(iter, p) {}

    template <class OtherIter, class OtherTraits>
    iterator_adaptor (const iterator_adaptor<OtherIter, Policies,
            OtherTraits>& src)
            : m_iter_p(src.iter(), src.policies()) {
    }

    reference operator*() const {
        return policies().dereference(type<reference>(), iter());
    }

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning( disable : 4284 )
#endif

    pointer operator->() const
        { return detail::operator_arrow(*this, iterator_category()); }

#ifdef _MSC_VER
# pragma warning(pop)
#endif

    reference operator[](difference_type n) const
        { return *(*this + n); }
    
    Self& operator++() {
#ifdef __MWERKS__
        // Odd bug, MWERKS couldn't  deduce the type for the member template
        // Workaround by explicitly specifying the type.
        policies().increment<Iterator>(iter());
#else
        policies().increment(iter());
#endif
        return *this;
    }

    Self operator++(int) { Self tmp(*this); ++*this; return tmp; }
    
    Self& operator--() {
        detail::decrement_impl(*this, iterator_category());
        return *this;
    }
    
    Self operator--(int) { Self tmp(*this); --*this; return tmp; }

    Self& operator+=(difference_type n) {
        detail::advance_impl(*this, n, iterator_category());
        return *this;
    }
  
    Self& operator-=(difference_type n) {
        detail::advance_impl(*this, -n, iterator_category());
        return *this;
    }

    iterator_type base() const { return m_iter_p.first(); }

private:
    typedef Policies policies_type;
    compressed_pair<Iterator,Policies> m_iter_p;
public: // too many compilers have trouble when these are private.
    Policies& policies() { return m_iter_p.second(); }
    const Policies& policies() const { return m_iter_p.second(); }
    Iterator& iter() { return m_iter_p.first(); }
    const Iterator& iter() const { return m_iter_p.first(); }
};


template <class Iterator, class Policies, class Traits>
iterator_adaptor<Iterator,Policies,Traits>
operator-(iterator_adaptor<Iterator,Policies,Traits> p, const typename Traits::difference_type x)
{
    return p -= x;
}

template <class Iterator, class Policies, class Traits>
iterator_adaptor<Iterator,Policies,Traits>
operator+(iterator_adaptor<Iterator,Policies,Traits> p, typename Traits::difference_type x)
{
    return p += x;
}

template <class Iterator, class Policies, class Traits>
iterator_adaptor<Iterator,Policies,Traits>
operator+(typename Traits::difference_type x, iterator_adaptor<Iterator,Policies,Traits> p)
{
    return p += x;
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
typename Traits1::difference_type operator-(
    const iterator_adaptor<Iterator1,Policies,Traits1>& x,
    const iterator_adaptor<Iterator2,Policies,Traits2>& y )
{
    typedef typename Traits1::difference_type difference_type;
    return x.policies().distance(type<difference_type>(), y.iter(), x.iter());
}

#ifndef BOOST_RELOPS_AMBIGUITY_BUG
template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator==(const iterator_adaptor<Iterator1,Policies,Traits1>& x, const iterator_adaptor<Iterator2,Policies,Traits2>& y) {
    return x.policies().equal(x.iter(), y.iter());
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator<(const iterator_adaptor<Iterator1,Policies,Traits1>& x, const iterator_adaptor<Iterator2,Policies,Traits2>& y) {
    return x.policies().less(x.iter(), y.iter());
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator>(const iterator_adaptor<Iterator1,Policies,Traits1>& x,
          const iterator_adaptor<Iterator2,Policies,Traits2>& y) { 
    return x.policies().less(y.iter(), x.iter());
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator>=(const iterator_adaptor<Iterator1,Policies,Traits1>& x, const iterator_adaptor<Iterator2,Policies,Traits2>& y) {
    return !x.policies().less(x.iter(), y.iter());
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator<=(const iterator_adaptor<Iterator1,Policies,Traits1>& x,
           const iterator_adaptor<Iterator2,Policies,Traits2>& y) {
    return !x.policies().less(y.iter(), x.iter());
}

template <class Iterator1, class Iterator2, class Policies, class Traits1, class Traits2>
inline bool 
operator!=(const iterator_adaptor<Iterator1,Policies,Traits1>& x, 
           const iterator_adaptor<Iterator2,Policies,Traits2>& y) {
    return !x.policies().equal(x.iter(), y.iter());
}
#endif

//=============================================================================
// iterator_adaptors - A type generator that simplifies creating
//   mutable/const pairs of iterator adaptors.

template <class Iterator, class ConstIterator, 
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
          class Traits, 
          class ConstTraits,
#else
          class Traits = std::iterator_traits<Iterator>, 
          class ConstTraits = std::iterator_traits<ConstIterator>,
#endif
          class Policies = default_iterator_policies>
class iterator_adaptors
{
public:
    typedef iterator_adaptor<Iterator, Policies, Traits> iterator;
    typedef iterator_adaptor<ConstIterator, Policies, ConstTraits> 
      const_iterator;
};


//=============================================================================
// Transform Iterator Adaptor
//
// Upon deference, apply some unary function object and return the
// result by value.

template <class AdaptableUnaryFunction>
struct transform_iterator_policies : public default_iterator_policies
{
    transform_iterator_policies() { }
    transform_iterator_policies(const AdaptableUnaryFunction& f) : m_f(f) { }
    
    template <class Reference, class Iterator>
    Reference dereference(type<Reference>, const Iterator& iter) const
        { return m_f(*iter); }

    AdaptableUnaryFunction m_f;
};

template <class AdaptableUnaryFunction, class IteratorTraits>
struct transform_iterator_traits {
    typedef typename AdaptableUnaryFunction::result_type value_type;
    typedef value_type reference;
    typedef value_type* pointer;
    typedef typename IteratorTraits::difference_type difference_type;
    typedef typename IteratorTraits::iterator_category iterator_category;
};
  
template <class AdaptableUnaryFunction,
          class Iterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
          class Traits = std::iterator_traits<Iterator>
#else
          class Traits
#endif
         >
struct transform_iterator
{
    typedef transform_iterator_traits<AdaptableUnaryFunction,Traits>
      TransTraits;
    typedef iterator_adaptor<Iterator, 
      transform_iterator_policies<AdaptableUnaryFunction>, TransTraits>
      type;
};


//=============================================================================
// Indirect Iterators Adaptor

// Given a pointer to pointers (or iterator to iterators),
// apply a double dereference inside operator*().
//
// We use the term "outer" to refer to the first level iterator type
// and "inner" to refer to the second level iterator type.  For
// example, given T**, T* is the inner iterator type and T** is the
// outer iterator type. Also, const T* would be the const inner
// iterator.

// We tried to implement this with transform_iterator, but that required
// using boost::remove_ref, which is not compiler portable.

struct indirect_iterator_policies : public default_iterator_policies
{
    template <class Reference, class Iterator>
    Reference dereference(type<Reference>, const Iterator& x) const
        { return **x; }
};

template <class OuterIterator, class InnerIterator,
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
          class OuterTraits,
          class InnerTraits
#else
          class OuterTraits = std::iterator_traits<OuterIterator>,
          class InnerTraits = std::iterator_traits<InnerIterator>
#endif
       >
struct indirect_traits
{
    typedef typename OuterTraits::difference_type difference_type;
    typedef typename InnerTraits::value_type value_type;
    typedef typename InnerTraits::pointer pointer;
    typedef typename InnerTraits::reference reference;
    typedef typename OuterTraits::iterator_category iterator_category;
};

template <class OuterIterator,      // Mutable or Immutable, does not matter
          class InnerIterator,      // Mutable -> mutable indirect iterator
                                    // Immutable -> immutable indirect iterator
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
          class OuterTraits,
          class InnerTraits
#else
          class OuterTraits = std::iterator_traits<OuterIterator>,
          class InnerTraits = std::iterator_traits<InnerIterator>
#endif
           >
struct indirect_iterator
{
    typedef iterator_adaptor<OuterIterator,
        indirect_iterator_policies,
        indirect_traits<OuterIterator, InnerIterator,
                        OuterTraits, InnerTraits>
    > type;
};

template <class OuterIterator,      // Mutable or Immutable, does not matter
          class InnerIterator,      // Mutable
          class ConstInnerIterator, // Immutable
#ifdef BOOST_NO_STD_ITERATOR_TRAITS
          class OuterTraits,
          class InnerTraits,
          class ConstInnerTraits
#else
          class OuterTraits = std::iterator_traits<OuterIterator>,
          class InnerTraits = std::iterator_traits<InnerIterator>,
          class ConstInnerTraits = std::iterator_traits<ConstInnerIterator>
#endif
           >
struct indirect_iterators
{
    typedef iterator_adaptors<OuterIterator, OuterIterator,
        indirect_traits<OuterIterator, InnerIterator,
                        OuterTraits, InnerTraits>,
        indirect_traits<OuterIterator, ConstInnerIterator,
                        OuterTraits, ConstInnerTraits>,
        indirect_iterator_policies
        > Adaptors;
    typedef typename Adaptors::iterator iterator;
    typedef typename Adaptors::const_iterator const_iterator;
};


//=============================================================================
// Reverse Iterators Adaptor

struct reverse_iterator_policies
{
    template <class Reference, class Iterator>
    Reference dereference(type<Reference>, const Iterator& x) const
        { return *boost::prior(x); }
    
    template <class Iterator>
    void increment(Iterator& x) const
        { --x; }
    
    template <class Iterator>
    void decrement(Iterator& x) const
        { ++x; }
    
    template <class Iterator, class DifferenceType>
    void advance(Iterator& x, DifferenceType n) const
        { x -= n; }
    
    template <class Difference, class Iterator1, class Iterator2>
    Difference distance(type<Difference>, const Iterator1& x, 
                        const Iterator2& y) const
        { return x - y; }
    
    template <class Iterator1, class Iterator2>
    bool equal(const Iterator1& x, const Iterator2& y) const
        { return x == y; }
    
    template <class Iterator1, class Iterator2>
    bool less(const Iterator1& x, const Iterator2& y) const
        { return y < x; }
};
  
template <class Iterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
          class Traits = std::iterator_traits<Iterator>
#else
          class Traits
#endif
         >
struct reverse_iterator
{
    typedef iterator_adaptor<Iterator, reverse_iterator_policies,
        Traits> type;
};

template <class ConstIterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
          class ConstTraits = std::iterator_traits<ConstIterator>
#else
          class ConstTraits
#endif
         >
struct const_reverse_iterator
{
    typedef iterator_adaptor<ConstIterator, reverse_iterator_policies,
        ConstTraits> type;
};

template <class Iterator, class ConstIterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
          class Traits = std::iterator_traits<Iterator>, 
          class ConstTraits = std::iterator_traits<ConstIterator>
#else
          class Traits,
          class ConstTraits
#endif
         >
struct reverse_iterators
{
    typedef iterator_adaptors<Iterator,ConstIterator,Traits,ConstTraits,
        reverse_iterator_policies> Adaptor;
    typedef typename Adaptor::iterator iterator;
    typedef typename Adaptor::const_iterator const_iterator;
};

//=============================================================================
// Projection Iterators Adaptor

template <class AdaptableUnaryFunction>
struct projection_iterator_policies : public default_iterator_policies
{
    projection_iterator_policies() { }
    projection_iterator_policies(const AdaptableUnaryFunction& f) : m_f(f) { }

    template <class Reference, class Iterator>
    Reference dereference (type<Reference>, Iterator const& iter) const {
        return m_f(*iter);
    }

    AdaptableUnaryFunction m_f;    
};

template <class AdaptableUnaryFunction, class Traits>
struct projection_iterator_traits {
    typedef typename AdaptableUnaryFunction::result_type value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::iterator_category iterator_category;
};

template <class AdaptableUnaryFunction, class Traits>
struct const_projection_iterator_traits {
    typedef typename AdaptableUnaryFunction::result_type value_type;
    typedef value_type const& reference;
    typedef value_type const* pointer;
    typedef typename Traits::difference_type difference_type;
    typedef typename Traits::iterator_category iterator_category;
};

template <class AdaptableUnaryFunction, class Iterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
        class Traits = std::iterator_traits<Iterator>
#else
        class Traits
#endif
        >
struct projection_iterator {
    typedef projection_iterator_traits<AdaptableUnaryFunction, Traits>
            Projection_Traits;
    typedef iterator_adaptor<Iterator,
            projection_iterator_policies<AdaptableUnaryFunction>,
            Projection_Traits> type;
};

template <class AdaptableUnaryFunction, class Iterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
        class Traits = std::iterator_traits<Iterator>
#else
        class Traits
#endif
        >
struct const_projection_iterator {
    typedef const_projection_iterator_traits<AdaptableUnaryFunction,
            Traits> Projection_Traits;
    typedef iterator_adaptor<Iterator,
            projection_iterator_policies<AdaptableUnaryFunction>,
            Projection_Traits> type;
};

template <class AdaptableUnaryFunction, class Iterator, class ConstIterator,
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
        class Traits = std::iterator_traits<Iterator>,
        class ConstTraits = std::iterator_traits<ConstIterator>
#else
        class Traits,
        class ConstTraits
#endif
        >
struct projection_iterators {
    typedef projection_iterator_traits<AdaptableUnaryFunction, Traits>
            Projection_Traits;
    typedef const_projection_iterator_traits<AdaptableUnaryFunction,
            ConstTraits> Const_Projection_Traits;
    typedef iterator_adaptors<Iterator, ConstIterator,
            Projection_Traits, Const_Projection_Traits,
            projection_iterator_policies<AdaptableUnaryFunction> > Adaptors;
    typedef typename Adaptors::iterator iterator;
    typedef typename Adaptors::const_iterator const_iterator;
};

} // namespace boost

#endif



