#ifndef BOOST_PP_IS_ITERATING
    ///////////////////////////////////////////////////////////////////////////////
    /// \file traits.hpp
    /// Contains definitions for arg\<\>, arg_c\<\>, left\<\>,
    /// right\<\>, tag\<\>, and the helper functions arg(), arg_c(),
    /// left() and right().
    //
    //  Copyright 2004 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #ifndef BOOST_PROTO_ARG_TRAITS_HPP_EAN_04_01_2005
    #define BOOST_PROTO_ARG_TRAITS_HPP_EAN_04_01_2005

    #include <boost/xpressive/proto/detail/prefix.hpp>
    #include <boost/config.hpp>
    #include <boost/detail/workaround.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_params.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/repeat_from_to.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/arithmetic/sub.hpp>
    #include <boost/ref.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/eval_if.hpp>
    #include <boost/static_assert.hpp>
    #include <boost/utility/result_of.hpp>
    #include <boost/type_traits/is_array.hpp>
    #include <boost/type_traits/is_function.hpp>
    #include <boost/type_traits/remove_cv.hpp>
    #include <boost/type_traits/remove_const.hpp>
    #include <boost/type_traits/add_reference.hpp>
    #include <boost/xpressive/proto/proto_fwd.hpp>
    #include <boost/xpressive/proto/ref.hpp>
    #include <boost/xpressive/proto/args.hpp>
    #include <boost/xpressive/proto/tags.hpp>
    #include <boost/xpressive/proto/transform/pass_through.hpp>
    #include <boost/xpressive/proto/detail/suffix.hpp>

    #if BOOST_WORKAROUND( BOOST_MSVC, == 1310 )
        #define BOOST_PROTO_IS_ARRAY_(T) boost::is_array<typename boost::remove_const<T>::type>
        #define BOOST_PROTO_WITH_ALIAS_(X,Y) template<typename Y>
    #else
        #define BOOST_PROTO_IS_ARRAY_(T) boost::is_array<T>
        #define BOOST_PROTO_WITH_ALIAS_(X,Y) typedef X Y;
    #endif

    #if BOOST_WORKAROUND( BOOST_MSVC, == 1400 )
        #pragma warning(push)
        #pragma warning(disable: 4180) // warning C4180: qualifier applied to function type has no meaning; ignored
    #endif

    namespace boost { namespace proto
    {

        // is_ref
        template<typename T, typename EnableIf>
        struct is_ref
          : mpl::false_
        {};

        template<typename T>
        struct is_ref<T, typename T::is_boost_proto_ref_>
          : mpl::true_
        {};

        // is_expr
        template<typename T, typename EnableIf>
        struct is_expr
          : mpl::false_
        {};

        template<typename T>
        struct is_expr<T, typename T::is_boost_proto_expr_>
          : mpl::true_
        {};

        namespace result_of
        {
            // as_expr
            template<typename T, typename Domain, typename EnableIf>
            struct as_expr
            {
                typedef typename mpl::eval_if<
                    mpl::or_<BOOST_PROTO_IS_ARRAY_(T), is_function<T> >
                  , add_reference<T>
                  , remove_cv<T>
                >::type arg0_type;

                typedef expr<proto::tag::terminal, args1<arg0_type> > expr_type;
                typedef typename Domain::template apply<expr_type>::type type;
                typedef type result_type;

                BOOST_PROTO_WITH_ALIAS_(T, T2)
                static result_type call(T2 &t)
                {
                    return Domain::make(expr_type::make(t));
                }
            };

            template<typename T, typename Domain>
            struct as_expr<T, Domain, typename T::is_boost_proto_expr_>
            {
                typedef typename T::boost_proto_expr_type_ type;
                typedef T &result_type;

                BOOST_PROTO_WITH_ALIAS_(T, T2)
                static result_type call(T2 &t)
                {
                    return t;
                }
            };

            // as_arg
            template<typename T, typename Domain, typename EnableIf>
            struct as_arg
            {
                typedef expr<proto::tag::terminal, args1<T &> > expr_type;
                typedef typename Domain::template apply<expr_type>::type type;

                BOOST_PROTO_WITH_ALIAS_(T, T2)
                static type call(T2 &t)
                {
                    return Domain::make(expr_type::make(t));
                }
            };

            template<typename T, typename Domain>
            struct as_arg<T, Domain, typename T::is_boost_proto_expr_>
            {
                typedef ref_<T> type;

                BOOST_PROTO_WITH_ALIAS_(T, T2)
                static type call(T2 &t)
                {
                    return type::make(t);
                }
            };

            template<typename Expr, typename N>
            struct arg
              : arg_c<Expr, N::value>
            {};

            // left
                // BUGBUG this forces the instantiation of Expr. Couldn't we
                // partially specialize left<> on expr< T, A > and
                // ref_< expr< T, A > > and return A::arg0 ?
            template<typename Expr>
            struct left
              : unref<typename Expr::arg0_type>
            {};

            // right
            template<typename Expr>
            struct right
              : unref<typename Expr::arg1_type>
            {};

        }

        namespace detail
        {
            template<typename T, typename EnableIf = void>
            struct if_vararg {};

            template<typename T>
            struct if_vararg<T, typename T::boost_proto_is_vararg_>
              : T
            {};
        }

        // terminal
        template<typename T>
        struct terminal : has_identity_transform
        {
            terminal();
            typedef expr<proto::tag::terminal, args1<T> > type;
            typedef proto::tag::terminal tag_type;
            typedef T arg0_type;
        };

        // unary_expr
        template<typename Tag, typename T>
        struct unary_expr : has_pass_through_transform<unary_expr<Tag, T> >
        {
            unary_expr();
            typedef expr<Tag, args1<T> > type;
            typedef Tag tag_type;
            typedef T arg0_type;
        };

        // binary_expr
        template<typename Tag, typename T, typename U>
        struct binary_expr : has_pass_through_transform<binary_expr<Tag, T, U> >
        {
            binary_expr();
            typedef expr<Tag, args2<T, U> > type;
            typedef Tag tag_type;
            typedef T arg0_type;
            typedef U arg1_type;
        };

    #define BOOST_PROTO_UNARY_GENERATOR(Name)\
        template<typename T>\
        struct Name : has_pass_through_transform<Name<T> >\
        {\
            Name();\
            typedef expr<proto::tag::Name, args1<T> > type;\
            typedef proto::tag::Name tag_type;\
            typedef T arg0_type;\
        };\
        /**/

    #define BOOST_PROTO_BINARY_GENERATOR(Name)\
        template<typename T, typename U>\
        struct Name : has_pass_through_transform<Name<T, U> >\
        {\
            Name();\
            typedef expr<proto::tag::Name, args2<T, U> > type;\
            typedef proto::tag::Name tag_type;\
            typedef T arg0_type;\
            typedef U arg1_type;\
        };\
        /**/

        BOOST_PROTO_UNARY_GENERATOR(posit)
        BOOST_PROTO_UNARY_GENERATOR(negate)
        BOOST_PROTO_UNARY_GENERATOR(dereference)
        BOOST_PROTO_UNARY_GENERATOR(complement)
        BOOST_PROTO_UNARY_GENERATOR(address_of)
        BOOST_PROTO_UNARY_GENERATOR(logical_not)
        BOOST_PROTO_UNARY_GENERATOR(pre_inc)
        BOOST_PROTO_UNARY_GENERATOR(pre_dec)
        BOOST_PROTO_UNARY_GENERATOR(post_inc)
        BOOST_PROTO_UNARY_GENERATOR(post_dec)

        BOOST_PROTO_BINARY_GENERATOR(shift_left)
        BOOST_PROTO_BINARY_GENERATOR(shift_right)
        BOOST_PROTO_BINARY_GENERATOR(multiplies)
        BOOST_PROTO_BINARY_GENERATOR(divides)
        BOOST_PROTO_BINARY_GENERATOR(modulus)
        BOOST_PROTO_BINARY_GENERATOR(plus)
        BOOST_PROTO_BINARY_GENERATOR(minus)
        BOOST_PROTO_BINARY_GENERATOR(less)
        BOOST_PROTO_BINARY_GENERATOR(greater)
        BOOST_PROTO_BINARY_GENERATOR(less_equal)
        BOOST_PROTO_BINARY_GENERATOR(greater_equal)
        BOOST_PROTO_BINARY_GENERATOR(equal_to)
        BOOST_PROTO_BINARY_GENERATOR(not_equal_to)
        BOOST_PROTO_BINARY_GENERATOR(logical_or)
        BOOST_PROTO_BINARY_GENERATOR(logical_and)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_and)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_or)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_xor)
        BOOST_PROTO_BINARY_GENERATOR(comma)
        BOOST_PROTO_BINARY_GENERATOR(mem_ptr)

        BOOST_PROTO_BINARY_GENERATOR(assign)
        BOOST_PROTO_BINARY_GENERATOR(shift_left_assign)
        BOOST_PROTO_BINARY_GENERATOR(shift_right_assign)
        BOOST_PROTO_BINARY_GENERATOR(multilpies_assign)
        BOOST_PROTO_BINARY_GENERATOR(divides_assign)
        BOOST_PROTO_BINARY_GENERATOR(modulus_assign)
        BOOST_PROTO_BINARY_GENERATOR(plus_assign)
        BOOST_PROTO_BINARY_GENERATOR(minus_assign)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_and_assign)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_or_assign)
        BOOST_PROTO_BINARY_GENERATOR(bitwise_xor_assign)
        BOOST_PROTO_BINARY_GENERATOR(subscript)

    #undef BOOST_PROTO_UNARY_GENERATOR
    #undef BOOST_PROTO_BINARY_GENERATOR

        // tag_of
        template<typename Expr>
        struct tag_of
        {
            typedef typename Expr::tag_type type;
        };

        // id
        template<typename Expr>
        struct id
          : result_of::deep_copy<Expr>
        {};

    #define BOOST_PROTO_ARG(z, n, data)\
        typedef BOOST_PP_CAT(data, n) BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);\
        /**/

    #define BOOST_PROTO_ARG_N_TYPE(z, n, data)\
        typename proto::result_of::unref<\
            typename Expr::BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type)\
        >::const_reference\
        /**/

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/traits.hpp>))
    #include BOOST_PP_ITERATE()
    #undef BOOST_PP_ITERATION_PARAMS_1

    #undef BOOST_PROTO_ARG
    #undef BOOST_PROTO_ARG_N_TYPE

        namespace functional
        {
            template<typename Domain>
            struct as_expr
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename T>
                struct result<This(T)>
                  : result_of::as_expr<typename remove_reference<T>::type, Domain>
                {};

                template<typename T>
                typename result_of::as_expr<T, Domain>::result_type
                operator ()(T &t) const
                {
                    return result_of::as_expr<T, Domain>::call(t);
                }

                template<typename T>
                typename result_of::as_expr<T const, Domain>::result_type
                operator ()(T const &t) const
                {
                    return result_of::as_expr<T const, Domain>::call(t);
                }

                #if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
                template<typename T, std::size_t N_>
                typename result_of::as_expr<T(&)[N_], Domain>::result_type
                operator ()(T (&t)[N_]) const
                {
                    return result_of::as_expr<T(&)[N_], Domain>::call(t);
                }

                template<typename T, std::size_t N_>
                typename result_of::as_expr<T const(&)[N_], Domain>::result_type
                operator ()(T const (&t)[N_]) const
                {
                    return result_of::as_expr<T const(&)[N_], Domain>::call(t);
                }
                #endif
            };

            template<typename Domain>
            struct as_arg
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename T>
                struct result<This(T)>
                  : result_of::as_arg<typename remove_reference<T>::type, Domain>
                {};

                template<typename T>
                typename result_of::as_arg<T, Domain>::type
                operator ()(T &t) const
                {
                    return result_of::as_arg<T, Domain>::call(t);
                }

                template<typename T>
                typename result_of::as_arg<T const, Domain>::type
                operator ()(T const &t) const
                {
                    return result_of::as_arg<T const, Domain>::call(t);
                }
            };

            template<long N>
            struct arg_c
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result_of::arg_c<typename detail::remove_cv_ref<Expr>::type, N>
                {};

                template<typename Expr>
                typename result_of::arg_c<Expr, N>::reference operator ()(Expr &expr) const
                {
                    return result_of::arg_c<Expr, N>::call(expr);
                }

                template<typename Expr>
                typename result_of::arg_c<Expr, N>::const_reference operator ()(Expr const &expr) const
                {
                    return result_of::arg_c<Expr, N>::call(expr);
                }
            };

            template<typename N>
            struct arg
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result_of::arg<typename detail::remove_cv_ref<Expr>::type, N>
                {};

                template<typename Expr>
                typename result_of::arg<Expr, N>::reference operator ()(Expr &expr) const
                {
                    return result_of::arg<Expr, N>::call(expr);
                }

                template<typename Expr>
                typename result_of::arg<Expr, N>::const_reference operator ()(Expr const &expr) const
                {
                    return result_of::arg<Expr, N>::call(expr);
                }
            };

            struct left
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result_of::left<typename detail::remove_cv_ref<Expr>::type>
                {};

                template<typename Expr>
                typename result_of::left<Expr>::reference operator ()(Expr &expr) const
                {
                    return proto::unref(expr.cast().arg0);
                }

                template<typename Expr>
                typename result_of::left<Expr>::const_reference operator ()(Expr const &expr) const
                {
                    return proto::unref(expr.cast().arg0);
                }
            };

            struct right
            {
                template<typename Sig>
                struct result {};

                template<typename This, typename Expr>
                struct result<This(Expr)>
                  : result_of::right<typename detail::remove_cv_ref<Expr>::type>
                {};

                template<typename Expr>
                typename result_of::right<Expr>::reference operator ()(Expr &expr) const
                {
                    return proto::unref(expr.cast().arg1);
                }

                template<typename Expr>
                typename result_of::right<Expr>::const_reference operator ()(Expr const &expr) const
                {
                    return proto::unref(expr.cast().arg1);
                }
            };

        }

        functional::left const left = {};
        functional::right const right = {};

        /// as_expr
        ///
        template<typename T>
        typename result_of::as_expr<T>::result_type
        as_expr(T &t BOOST_PROTO_DISABLE_IF_IS_CONST(T))
        {
            return result_of::as_expr<T>::call(t);
        }

        /// \overload
        ///
        template<typename T>
        typename result_of::as_expr<T const>::result_type
        as_expr(T const &t)
        {
            return result_of::as_expr<T const>::call(t);
        }

        /// \overload
        ///
        template<typename Domain, typename T>
        typename result_of::as_expr<T, Domain>::result_type
        as_expr(T &t BOOST_PROTO_DISABLE_IF_IS_CONST(T))
        {
            return result_of::as_expr<T, Domain>::call(t);
        }

        /// \overload
        ///
        template<typename Domain, typename T>
        typename result_of::as_expr<T const, Domain>::result_type
        as_expr(T const &t)
        {
            return result_of::as_expr<T const, Domain>::call(t);
        }

        /// as_arg
        ///
        template<typename T>
        typename result_of::as_arg<T>::type
        as_arg(T &t BOOST_PROTO_DISABLE_IF_IS_CONST(T))
        {
            return result_of::as_arg<T>::call(t);
        }

        /// \overload
        ///
        template<typename T>
        typename result_of::as_arg<T const>::type
        as_arg(T const &t)
        {
            return result_of::as_arg<T const>::call(t);
        }

        /// \overload
        ///
        template<typename Domain, typename T>
        typename result_of::as_arg<T, Domain>::type
        as_arg(T &t BOOST_PROTO_DISABLE_IF_IS_CONST(T))
        {
            return result_of::as_arg<T, Domain>::call(t);
        }

        /// \overload
        ///
        template<typename Domain, typename T>
        typename result_of::as_arg<T const, Domain>::type
        as_arg(T const &t)
        {
            return result_of::as_arg<T const, Domain>::call(t);
        }

        /// arg
        ///
        template<typename Expr>
        typename result_of::unref<typename Expr::type::arg0_type>::reference
        arg(Expr &expr BOOST_PROTO_DISABLE_IF_IS_CONST(Expr))
        {
            return proto::unref(expr.cast().arg0);
        };

        /// \overload
        ///
        template<typename Expr>
        typename result_of::unref<typename Expr::type::arg0_type>::const_reference
        arg(Expr const &expr)
        {
            return proto::unref(expr.cast().arg0);
        };

        /// \overload
        ///
        template<typename N, typename Expr>
        typename result_of::arg<Expr, N>::reference
        arg(Expr &expr BOOST_PROTO_DISABLE_IF_IS_CONST(Expr))
        {
            return result_of::arg<Expr, N>::call(expr);
        };

        /// \overload
        ///
        template<typename N, typename Expr>
        typename result_of::arg<Expr, N>::const_reference
        arg(Expr const &expr)
        {
            return result_of::arg<Expr, N>::call(expr);
        };

        /// arg_c
        ///
        template<long N, typename Expr>
        typename result_of::arg_c<Expr, N>::reference
        arg_c(Expr &expr BOOST_PROTO_DISABLE_IF_IS_CONST(Expr))
        {
            return result_of::arg_c<Expr, N>::call(expr);
        };

        /// \overload
        ///
        template<long N, typename Expr>
        typename result_of::arg_c<Expr, N>::const_reference arg_c(Expr const &expr)
        {
            return result_of::arg_c<Expr, N>::call(expr);
        };

    }}

    #if BOOST_WORKAROUND( BOOST_MSVC, == 1400 )
        #pragma warning(pop)
    #endif

    #endif

#else // PP_IS_ITERATING

    #define N BOOST_PP_ITERATION()
    #if N > 0
        template<BOOST_PP_ENUM_PARAMS(N, typename A)>
        struct function<
            BOOST_PP_ENUM_PARAMS(N, A)
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(BOOST_PROTO_MAX_ARITY, N), void BOOST_PP_INTERCEPT), void
        >
          : has_pass_through_transform<
                function<
                    BOOST_PP_ENUM_PARAMS(N, A)
                    BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(BOOST_PROTO_MAX_ARITY, N), void BOOST_PP_INTERCEPT), void
                >
            >
        {
            typedef expr<proto::tag::function, BOOST_PP_CAT(args, N)<BOOST_PP_ENUM_PARAMS(N, A)> > type;
            typedef proto::tag::function tag_type;
            BOOST_PP_REPEAT(N, BOOST_PROTO_ARG, A)
            BOOST_PP_REPEAT_FROM_TO(N, BOOST_PROTO_MAX_ARITY, BOOST_PROTO_ARG, detail::if_vararg<BOOST_PP_CAT(A, BOOST_PP_DEC(N))> BOOST_PP_INTERCEPT)
        };

        template<typename Tag BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct nary_expr<
            Tag
            BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(BOOST_PROTO_MAX_ARITY, N), void BOOST_PP_INTERCEPT), void
        >
          : has_pass_through_transform<
                nary_expr<
                    Tag
                    BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
                    BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SUB(BOOST_PROTO_MAX_ARITY, N), void BOOST_PP_INTERCEPT), void
                >
            >
        {
            typedef expr<Tag, BOOST_PP_CAT(args, N)<BOOST_PP_ENUM_PARAMS(N, A)> > type;
            typedef Tag tag_type;
            BOOST_PP_REPEAT(N, BOOST_PROTO_ARG, A)
            BOOST_PP_REPEAT_FROM_TO(N, BOOST_PROTO_MAX_ARITY, BOOST_PROTO_ARG, detail::if_vararg<BOOST_PP_CAT(A, BOOST_PP_DEC(N))> BOOST_PP_INTERCEPT)
        };
    #endif

        namespace result_of
        {
            template<typename Expr>
            struct arg_c<Expr, N>
              : unref<typename Expr::BOOST_PP_CAT(BOOST_PP_CAT(arg, N), _type)>
            {
                static typename arg_c::reference call(Expr &expr)
                {
                    return proto::unref(expr.cast().BOOST_PP_CAT(arg, N));
                }

                static typename arg_c::const_reference call(Expr const &expr)
                {
                    return proto::unref(expr.cast().BOOST_PP_CAT(arg, N));
                }
            };

            template<typename Expr>
            struct arg_c<Expr const, N>
              : arg_c<Expr, N>
            {};
        }

    #undef N

#endif
