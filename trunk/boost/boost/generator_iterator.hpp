// (C) Copyright Jens Maurer 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// Revision History:

// 15 Nov 2001   Jens Maurer
//      created.

//  See http://www.boost.org/libs/utility/iterator_adaptors.htm for documentation.

#ifndef BOOST_ITERATOR_ADAPTOR_GENERATOR_ITERATOR_HPP
#define BOOST_ITERATOR_ADAPTOR_GENERATOR_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/ref.hpp>

namespace boost {

template<class Generator>
class generator_iterator
  : public iterator_facade<
        generator_iterator<Generator>
      , typename Generator::result_type
      , readable_lvalue_iterator_tag
      , single_pass_traversal_tag
    >
{
    typedef iterator_facade<
        generator_iterator<Generator>
      , typename Generator::result_type
      , readable_iterator_tag
      , single_pass_traversal_tag
    > super_t;
    
 public:
    generator_iterator() {}
    generator_iterator(Generator* g) : m_g(g), m_value((*m_g)()) {}

    void increment()
    {
        m_value = (*m_g)();
    }

    const typename Generator::result_type&
    dereference() const
    {
        return m_value;
    }

    bool equal(generator_iterator const& y) const
    {
        return this->m_g == y.m_g && this->m_value == y.m_value;
    }

 private:
    Generator* m_g;
    typename Generator::result_type m_value;
};

template<class Generator>
struct generator_iterator_generator
{
  typedef generator_iterator<Generator> type;
};

template <class Generator>
inline generator_iterator<Generator>
make_generator_iterator(Generator & gen)
{
  typedef generator_iterator<Generator> result_t;
  return result_t(&gen);
}

} // namespace boost


#endif // BOOST_ITERATOR_ADAPTOR_GENERATOR_ITERATOR_HPP

