/*=============================================================================
    Copyright (c) 2005-2006 João Abecasis
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_PROCEDURE_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/blank.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/functional/invocation/limits.hpp>
#include <boost/fusion/functional/invocation/detail/that_ptr.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Function, class Sequence> struct invoke_procedure;
    }

    template <typename Function, class Sequence>
    inline void invoke_procedure(Function, Sequence &);

    template <typename Function, class Sequence>
    inline void invoke_procedure(Function, Sequence const &);

    //----- ---- --- -- - -  -   -

    namespace detail
    {
        namespace ft = function_types;

        template< 
            typename Function, class Sequence, 
            int N = result_of::size<Sequence>::value,
            bool CBI = ft::is_callable_builtin<Function>::value,
            bool MFP = ft::is_member_function_pointer<Function>::value,
            bool RandomAccess = boost::is_convertible< 
                    typename traits::category_of<Sequence>::type,
                    fusion::random_access_traversal_tag 
                >::value 
            >
        struct invoke_procedure_impl
        {
            typedef boost::blank result;
        };

        template <typename Func, class N, bool CBI = true>
        // Contains void type member, empty on arity mismatch
        struct invoke_procedure_result
            : mpl::if_< 
                  mpl::or_<
                      mpl::bool_<! CBI>, 
                      mpl::equal_to< ft::function_arity<Func>, N >,
                      mpl::and_< ft::is_callable_builtin<Func, ft::variadic>,
                          mpl::less< ft::function_arity<Func>, N > >
                  >, mpl::identity<void>, boost::blank
              >::type
        { }; 

        #define  BOOST_PP_FILENAME_1 \
            <boost/fusion/functional/invocation/invoke_procedure.hpp>
        #define  BOOST_PP_ITERATION_LIMITS \
            (0, BOOST_FUSION_INVOKE_PROCEDURE_MAX_ARITY)
        #include BOOST_PP_ITERATE()

    }

    namespace result_of
    {
        template <typename Function, class Sequence> struct invoke_procedure
            : detail::invoke_procedure_impl< 
                typename boost::remove_reference<Function>::type, Sequence
              >::result
        { }; 
    }

    template <typename Function, class Sequence>
    inline void invoke_procedure(Function f, Sequence & s)
    {
        detail::invoke_procedure_impl<
                typename boost::remove_reference<Function>::type,Sequence
            >::call(f,s);
    }

    template <typename Function, class Sequence>
    inline void invoke_procedure(Function f, Sequence const & s)
    {
        detail::invoke_procedure_impl<
                typename boost::remove_reference<Function>::type,Sequence const
            >::call(f,s);
    }

}}

#define BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_PROCEDURE_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#define N BOOST_PP_ITERATION()

#define M(z,j,data) fusion::at_c<j>(s)

        template <typename Function, class Sequence, bool CBI>
        struct invoke_procedure_impl<Function,Sequence,N,CBI,false,true>
        {
            struct result
                : invoke_procedure_result< Function, mpl::size_t<N>, CBI >
            { }; 

            static inline void call(Function & f, Sequence & s)
            {
                f(BOOST_PP_ENUM(N,M,~));
            }
        };

#if N > 0
        template <typename Function, class Sequence>
        struct invoke_procedure_impl<Function,Sequence,N,true,true,true>
        {
            struct result
                : invoke_procedure_result< Function, mpl::size_t<N> >
            { }; 

            static inline void call(Function & f, Sequence & s)
            {
                (that_ptr<typename mpl::front<
                                ft::parameter_types<Function> >::type
                    >::get(fusion::at_c<0>(s))->*f)(BOOST_PP_ENUM_SHIFTED(N,M,~));
            }
        };
#endif

#undef M

#define M(z,j,data)                                                             \
            typedef typename result_of::next< BOOST_PP_CAT(I,BOOST_PP_DEC(j))  \
                >::type I ## j ;                                               \
            I##j i##j = fusion::next(BOOST_PP_CAT(i,BOOST_PP_DEC(j)));

        template <typename Function, class Sequence, bool CBI>
        struct invoke_procedure_impl<Function,Sequence,N,CBI,false,false>
        {
            struct result
                : invoke_procedure_result< Function, mpl::size_t<N>, CBI >
            { }; 

            static inline void call(Function & f, Sequence & s)
            {
#if N > 0
                typedef typename result_of::begin<Sequence>::type I0;
                I0 i0 = fusion::begin(s);
                BOOST_PP_REPEAT_FROM_TO(1,N,M,~)
#endif
                f( BOOST_PP_ENUM_PARAMS(N,*i) );
            }
        };

#if N > 0
        template <typename Function, class Sequence>
        struct invoke_procedure_impl<Function,Sequence,N,true,true,false>
        {
            struct result
                : invoke_procedure_result< Function, mpl::size_t<N> >
            { }; 

            static inline void call(Function & f, Sequence & s)
            {
                typedef typename result_of::begin<Sequence>::type I0;
                I0 i0 = fusion::begin(s);
                BOOST_PP_REPEAT_FROM_TO(1,N,M,~)

                (that_ptr<typename mpl::front<
                                ft::parameter_types<Function> >::type
                    >::get(*i0)->*f)(BOOST_PP_ENUM_SHIFTED_PARAMS(N,*i));
            }
        };
#endif

#undef M

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
#endif 

