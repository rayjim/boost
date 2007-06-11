/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_FUNCTION_OBJECT_HPP_INCLUDED)
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_FUNCTION_OBJECT_HPP_INCLUDED

#include <boost/type_traits/add_reference.hpp>

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/functional/invocation/invoke_function_object.hpp>

namespace boost { namespace fusion
{
    template <class Function> class fused_function_object;

    //----- ---- --- -- - -  -   -

    template <class Function>
    class fused_function_object
    {
        Function fnc_transformed;

        typedef typename boost::add_reference<Function>::type func_fwd_t;
        typedef typename detail::call_param<Function>::type func_const_fwd_t;

    public:

        inline explicit fused_function_object(func_const_fwd_t f = Function())
            : fnc_transformed(f)
        { }

        template <class Seq> 
        inline typename result_of::invoke_function_object<Function,Seq const
            >::type operator()(Seq const & s) const
        {
          return fusion::invoke_function_object<
              func_const_fwd_t >(this->fnc_transformed,s);
        }

        template <class Seq> 
        inline typename result_of::invoke_function_object<Function,Seq const
            >::type 
        operator()(Seq const & s) 
        {
          return fusion::invoke_function_object<
              func_fwd_t >(this->fnc_transformed,s);
        }

        template <class Seq> 
        inline typename result_of::invoke_function_object<Function,Seq>::type
        operator()(Seq & s) const
        {
          return fusion::invoke_function_object<
              func_const_fwd_t >(this->fnc_transformed,s);
        }

        template <class Seq> 
        inline typename result_of::invoke_function_object<Function,Seq>::type
        operator()(Seq & s) 
        {
          return fusion::invoke_function_object<
              func_fwd_t >(this->fnc_transformed,s);
        }

        template<typename T>
        struct result;

        template <typename Seq>
        struct result<fused_function_object(Seq)>
            : result_of::invoke_function_object<Function, Seq>
        { };
    };

}}

#endif

