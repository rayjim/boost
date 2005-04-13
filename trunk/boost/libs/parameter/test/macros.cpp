// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>
#include <boost/parameter/macros.hpp>
#include <boost/bind.hpp>
#include <boost/static_assert.hpp>
#include <boost/ref.hpp>
#include <cassert>
#include <string.h>

namespace test
{
  using namespace boost::parameter;
  
  keyword<struct name_> name;
  keyword<struct value_> value;
  keyword<struct index_> index;
  keyword<struct tester_> tester;

  struct f_parameters // vc6 is happier with inheritance than with a typedef
    : parameters<
          tester_
        , name_
        , value_
        , index_
      >
  {};

  double value_default()
  {
      return 666.222;
  }
  
  BOOST_PARAMETER_FUN(int, f, 2, 4, f_parameters)
  {
      p[tester](
          p[name]
        , p[value || boost::bind(&value_default) ]
        , p[index | 999 ]
      );

      return 1;
  }

  template <class T>
  bool equal(T const& x, T const& y)
  {
      return x == y;
  }
  
  bool equal(char const* s1, char const* s2)
  {
      return !strcmp(s1,s2);
  }
  
  template <class Name, class Value, class Index>
  struct values_t
  {
      values_t(Name const& n, Value const& v, Index const& i)
        : n(n), v(v), i(i)
      {}

      template <class Name_, class Value_, class Index_>
      void operator()(Name_ const& n_, Value_ const& v_, Index_ const& i_) const
      {

          // Only VC and its emulators fail this; they seem to have
          // problems with deducing the constness of string literal
          // arrays.
 #if defined(_MSC_VER)                                      \
     && (BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, <= 700)  \
         || BOOST_WORKAROUND(BOOST_MSVC, < 1310))
# else 
          BOOST_STATIC_ASSERT((boost::is_same<Index,Index_>::value));
          BOOST_STATIC_ASSERT((boost::is_same<Value,Value_>::value));
          BOOST_STATIC_ASSERT((boost::is_same<Name,Name_>::value));
#endif
          assert(equal(n, n_));
          assert(equal(v, v_));
          assert(equal(i, i_));
      }
      
      Name const& n;
      Value const& v;
      Index const& i;
  };

  template <class Name, class Value, class Index>
  values_t<Name,Value,Index>
  values(Name const& n, Value const& v, Index const& i)
  {
      return values_t<Name,Value,Index>(n,v,i);
  }
  
} // namespace test

// GCC2 has a problem with char (&)[] deduction, so we'll cast string
// literals there.
#undef S
#if BOOST_WORKAROUND(__GNUC__, == 2)
# define S(s) (char const*)s
#else
# define S(s) s
#endif

int main()
{
    using test::f;
    using test::name;
    using test::value;
    using test::index;
    using test::tester;

    f(
       test::values(S("foo"), S("bar"), S("baz"))
     , S("foo"), S("bar"), S("baz")
   );

   int x = 56;
   f(
       test::values("foo", 666.222, 56)
     , index = boost::ref(x), name = "foo"
   );

   return 0;
}

