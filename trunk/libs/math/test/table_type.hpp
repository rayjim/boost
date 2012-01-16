
#ifndef BOOST_MATH_TEST_TABLE_TYPE_HPP
#define BOOST_MATH_TEST_TABLE_TYPE_HPP

template <class T>
struct table_type
{
   typedef T type;
};

namespace boost{ namespace math{ namespace concepts{

   class real_concept;

}}}

template <>
struct table_type<boost::math::concepts::real_concept>
{
   typedef long double type;
};

namespace boost{ namespace multiprecision{

   template <class Backend>
   class mp_number;

}}

template <class Backend>
struct table_type<boost::multiprecision::mp_number<Backend> >
{
   typedef const char* type;
};

#endif
