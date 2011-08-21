
//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(TTI_MF_TYPE_HPP)
#define TTI_MF_TYPE_HPP

#include <boost/config.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/tti/gen/namespace_gen.hpp>
#include <boost/tti/detail/dnotype.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

namespace boost
  {
  namespace tti
    {
  
    /// A metafunction which checks whether a type exists within an enclosing type and optionally is a particular type.
    /**

        This metafunction takes its specific types as nullary metafunctions whose typedef 'type' member is the actual type used.
    
        The metafunction types and return:

          TTI_HasType = a Boost MPL lambda expression using the metafunction generated from the TTI_HAS_TYPE ( or TTI_TRAIT_HAS_TYPE ) macro.<br />
                        The easiest way to generate the lambda expression is to use a Boost MPL placeholder expression of the form 'metafunction\<_\>' ( or optionally 'metafunction\<_,_\>' ).
                        You can also use a metafunction class generated by boost::mpl::quote2.
                    
          TTI_T       = the enclosing type as a nullary metafunction.
          
          TTI_U       = the type of the inner type as a nullary metafunction, as an optional parameter.
      
          returns = 'value' is true if the type exists within the enclosing type
                    and, if type U is specified, the type is the same as the type U,
                    otherwise 'value' is false.
                          
    */
    template
      <
      class TTI_HasType,
      class TTI_T,
      class TTI_U = boost::mpl::identity<BOOST_TTI_NAMESPACE::detail::notype>
      >
    struct mf_has_type :
      boost::mpl::apply
        <
        TTI_HasType,
        typename TTI_T::type,
        typename TTI_U::type
        >::type
      {
      };
    }
  }
  
#endif // TTI_MF_TYPE_HPP
