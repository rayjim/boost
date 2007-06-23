//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2007.
// (C) Copyright Gennaro Prota 2003 - 2004.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_DETAIL_ITERATORS_HPP
#define BOOST_INTERPROCESS_DETAIL_ITERATORS_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>

#include <iterator>

namespace boost {
namespace interprocess { 

template <class T, class Difference = std::ptrdiff_t>
class constant_iterator
  : public std::iterator
      <std::random_access_iterator_tag, T, Difference, const T*, const T &>
{
   typedef  constant_iterator<T, Difference> this_type;

   public:
   explicit constant_iterator(const T &ref, Difference range_size)
      :  m_ptr(&ref), m_num(range_size){}

   //Constructors
   constant_iterator()
      :  m_ptr(0), m_num(0){}

   constant_iterator& operator++() 
   { increment();   return *this;   }
   
   constant_iterator operator++(int)
   {
      constant_iterator result (*this);
      increment();
      return result;
   }

   friend bool operator== (const constant_iterator& i, const constant_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i == i2); }

   friend bool operator< (const constant_iterator& i, const constant_iterator& i2)
   { return i.less(i2); }

   friend bool operator> (const constant_iterator& i, const constant_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const constant_iterator& i, const constant_iterator& i2)
   { return !(i < i2); }

   friend Difference operator- (const constant_iterator& i, const constant_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   constant_iterator& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   constant_iterator operator+(Difference off) const
   {
      constant_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend constant_iterator operator+(Difference off, const constant_iterator& right)
   {  return right + off; }

   constant_iterator& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   constant_iterator operator-(Difference off) const
   {  return *this + (-off);  }

   const T& operator*() const
   { return dereference(); }

   const T* operator->() const
   { return &(dereference()); }

   private:
   const T *   m_ptr;
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   const T & dereference() const
   { return *m_ptr; }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};

template <class T, class Difference = std::ptrdiff_t>
class repeat_iterator
  : public std::iterator
      <std::random_access_iterator_tag, T, Difference>
{
   typedef repeat_iterator this_type;
   public:
   explicit repeat_iterator(T &ref, Difference range_size)
      :  m_ptr(&ref), m_num(range_size){}

   //Constructors
   repeat_iterator()
      :  m_ptr(0), m_num(0){}

   repeat_iterator& operator++() 
   { increment();   return *this;   }
   
   repeat_iterator operator++(int)
   {
      repeat_iterator result (*this);
      increment();
      return result;
   }

   friend bool operator== (const repeat_iterator& i, const repeat_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const repeat_iterator& i, const repeat_iterator& i2)
   { return !(i == i2); }

   friend bool operator< (const repeat_iterator& i, const repeat_iterator& i2)
   { return i.less(i2); }

   friend bool operator> (const repeat_iterator& i, const repeat_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const repeat_iterator& i, const repeat_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const repeat_iterator& i, const repeat_iterator& i2)
   { return !(i < i2); }

   friend Difference operator- (const repeat_iterator& i, const repeat_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   repeat_iterator& operator+=(Difference off)
   {  this->advance(off); return *this;   }

   repeat_iterator operator+(Difference off) const
   {
      repeat_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend repeat_iterator operator+(Difference off, const repeat_iterator& right)
   {  return right + off; }

   repeat_iterator& operator-=(Difference off)
   {  this->advance(-off); return *this;   }

   repeat_iterator operator-(Difference off) const
   {  return *this + (-off);  }

   T& operator*() const
   { return dereference(); }

   T *operator->() const
   { return &(dereference()); }

   private:
   T *         m_ptr;
   Difference  m_num;

   void increment()
   { --m_num; }

   void decrement()
   { ++m_num; }

   bool equal(const this_type &other) const
   {  return m_num == other.m_num;   }

   bool less(const this_type &other) const
   {  return other.m_num < m_num;   }

   T & dereference() const
   { return *m_ptr; }

   void advance(Difference n)
   {  m_num -= n; }

   Difference distance_to(const this_type &other)const
   {  return m_num - other.m_num;   }
};

template <class PseudoReference>
struct operator_arrow_proxy
{
   operator_arrow_proxy(const PseudoReference &px)
      :  m_value(px)
   {}

   PseudoReference* operator->() const { return &m_value; }
   // This function is needed for MWCW and BCC, which won't call operator->
   // again automatically per 13.3.1.2 para 8
//   operator T*() const { return &m_value; }
   mutable PseudoReference m_value;
};


template <class Iterator, class UnaryFunction>
class transform_iterator
   : public UnaryFunction
   , public std::iterator
      < typename Iterator::iterator_category
      , typename Iterator::value_type
      , typename Iterator::difference_type
      , typename Iterator::pointer
      , typename UnaryFunction::result_type>
{
   public:
   explicit transform_iterator(const Iterator &it, const UnaryFunction &f)
      :  UnaryFunction(f), m_it(it)
   {}

   //Constructors
   transform_iterator& operator++() 
   { increment();   return *this;   }

   transform_iterator operator++(int)
   {
      transform_iterator result (*this);
      increment();
      return result;
   }

   friend bool operator== (const transform_iterator& i, const transform_iterator& i2)
   { return i.equal(i2); }

   friend bool operator!= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i == i2); }

/*
   friend bool operator> (const transform_iterator& i, const transform_iterator& i2)
   { return i2 < i; }

   friend bool operator<= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i > i2); }

   friend bool operator>= (const transform_iterator& i, const transform_iterator& i2)
   { return !(i < i2); }
*/
   friend typename Iterator::difference_type operator- (const transform_iterator& i, const transform_iterator& i2)
   { return i2.distance_to(i); }

   //Arithmetic
   transform_iterator& operator+=(typename Iterator::difference_type off)
   {  this->advance(off); return *this;   }

   transform_iterator operator+(typename Iterator::difference_type off) const
   {
      transform_iterator other(*this);
      other.advance(off);
      return other;
   }

   friend transform_iterator operator+(typename Iterator::difference_type off, const transform_iterator& right)
   {  return right + off; }

   transform_iterator& operator-=(typename Iterator::difference_type off)
   {  this->advance(-off); return *this;   }

   transform_iterator operator-(typename Iterator::difference_type off) const
   {  return *this + (-off);  }

   typename UnaryFunction::result_type operator*() const
   { return dereference(); }

   operator_arrow_proxy<typename UnaryFunction::result_type>
      operator->() const
   { return operator_arrow_proxy<typename UnaryFunction::result_type>(dereference());  }

   private:
   Iterator m_it;

   void increment()
   { ++m_it; }

   void decrement()
   { --m_it; }

   bool equal(const transform_iterator &other) const
   {  return m_it == other.m_it;   }

   bool less(const transform_iterator &other) const
   {  return other.m_it < m_it;   }

   typename UnaryFunction::result_type dereference() const
   { return UnaryFunction::operator()(*m_it); }

   void advance(typename Iterator::difference_type n)
   {  std::advance(m_it, n); }

   typename Iterator::difference_type distance_to(const transform_iterator &other)const
   {  return std::distance(other.m_it, m_it); }
};

template <class Iterator, class UnaryFunc>
transform_iterator<Iterator, UnaryFunc>
make_transform_iterator(Iterator it, UnaryFunc fun)
{
   return transform_iterator<Iterator, UnaryFunc>(it, fun);
}

}  //namespace interprocess { 
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_DETAIL_ITERATORS_HPP

