#ifndef BOOST_PP_IS_ITERATING
    ///////////////////////////////////////////////////////////////////////////////
    /// \file expr.hpp
    /// Contains definition of expr\<\> class template.
    //
    //  Copyright 2004 Eric Niebler. Distributed under the Boost
    //  Software License, Version 1.0. (See accompanying file
    //  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    #ifndef BOOST_PROTO_EXPR_HPP_EAN_04_01_2005
    #define BOOST_PROTO_EXPR_HPP_EAN_04_01_2005

    #include <boost/xpressive/proto/detail/prefix.hpp>
    #include <boost/preprocessor/inc.hpp>
    #include <boost/preprocessor/dec.hpp>
    #include <boost/preprocessor/cat.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/repetition/repeat_from_to.hpp>
    #include <boost/preprocessor/repetition/enum_trailing.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_params.hpp>
    #include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
    #include <boost/utility/result_of.hpp>
    #include <boost/utility/addressof.hpp>
    #include <boost/xpressive/proto/proto_fwd.hpp>
    #include <boost/xpressive/proto/ref.hpp>
    #include <boost/xpressive/proto/args.hpp>
    #include <boost/xpressive/proto/traits.hpp>
    #include <boost/xpressive/proto/detail/suffix.hpp>

    namespace boost { namespace proto
    {
    #define BOOST_PROTO_ARG(z, n, data)\
        typedef typename Args::BOOST_PP_CAT(arg, n) BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);\
        BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type) BOOST_PP_CAT(arg, n);\
        /**/

    #define BOOST_PROTO_VOID(z, n, data)\
        typedef void BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);\
        /**/

    #define BOOST_PROTO_AS_OP(z, n, data)\
        proto::as_arg(BOOST_PP_CAT(a,n))\
        /**/

    #define BOOST_PROTO_UNREF_ARG_TYPE(z, n, data)\
        typename result_of::unref<typename Args::BOOST_PP_CAT(arg, n)>::const_reference\
        /**/

    #define BOOST_PROTO_UNREF_ARG(z, n, data)\
        proto::unref(this->BOOST_PP_CAT(arg, n))\
        /**/

        namespace detail
        {
            template<typename Tag, typename Arg>
            struct address_of_hack
            {
                typedef address_of_hack type;
            };

            template<typename Expr>
            struct address_of_hack<tag::address_of, ref_<Expr> >
            {
                typedef Expr *type;
            };
        }

        namespace result_of
        {
        #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_PP_DEC(BOOST_PROTO_MAX_ARITY), <boost/xpressive/proto/detail/funop.hpp>))
        #include BOOST_PP_ITERATE()
        }

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, BOOST_PROTO_MAX_ARITY, <boost/xpressive/proto/expr.hpp>))
    #include BOOST_PP_ITERATE()

    #undef BOOST_PROTO_ARG
    #undef BOOST_PROTO_VOID
    #undef BOOST_PROTO_AS_OP
    #undef BOOST_PROTO_UNREF_ARG_TYPE
    #undef BOOST_PROTO_UNREF_ARG
    }}

    #endif // BOOST_PROTO_EXPR_HPP_EAN_04_01_2005

#elif BOOST_PP_ITERATION_DEPTH() == 1

        /// \brief Representation of a node in an expression tree.
        /// 
        /// \c proto::expr\<\> is a node in an expression template tree. It
        /// is a container for its children sub-trees. It also serves as
        /// the terminal nodes of the tree.
        /// 
        /// \c Tag is type that represents the operation encoded by
        ///             this expression. It is typically one of the structs
        ///             in the \c boost::proto::tag namespace, but it doesn't
        ///             have to be. If the \c Tag type is \c boost::proto::tag::terminal
        ///             then this \c expr\<\> type represents a leaf in the
        ///             expression tree.
        ///
        /// \c Args is a type list representing the type of the children
        ///             of this expression. It is an instantiation of one
        ///             of \c proto::args1\<\>, \c proto::args2\<\>, etc. The 
        ///             children types must all themselves be either \c expr\<\>
        ///             or \c proto::ref_\<proto::expr\<\>\>, unless the \c Tag
        ///             type is \c boost::proto::tag::terminal, in which case
        ///             \c Args must be \c proto::args1\<T\>, where \c T can be any
        ///             type.
        template<typename Tag, typename Args>
        struct expr<Tag, Args, BOOST_PP_ITERATION() >
        {
            typedef Tag tag_type;
            typedef expr type;
            typedef Args args_type;
            typedef default_domain domain;
            typedef mpl::long_<BOOST_PP_ITERATION()> arity;
            typedef proto_expr_tag fusion_tag;
            typedef void is_boost_proto_expr_;
            typedef expr boost_proto_expr_type_;

            BOOST_PROTO_IDENTITY_TRANSFORM();
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_PROTO_ARG, ~)
            BOOST_PP_REPEAT_FROM_TO(BOOST_PP_ITERATION(), BOOST_PROTO_MAX_ARITY, BOOST_PROTO_VOID, ~)

            /// \return *this
            ///
            expr &cast()
            {
                return *this;
            }

            /// \return *this
            ///
            expr const &cast() const
            {
                return *this;
            }

            /// \return A new \c expr\<\> object initialized with the specified
            /// arguments.
            ///
            template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename A)>
            static expr make(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), A, const &a))
            {
                expr that = {BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), a)};
                return that;
            }

        #if 1 == BOOST_PP_ITERATION()
            /// \overload
            ///
            template<typename A0>
            static expr make(A0 &a0)
            {
                expr that = {a0};
                return that;
            }

            /// If \c Tag is \c boost::proto::tag::address_of and \c arg0_type is
            /// \c proto::ref_\<T\>, then \c address_of_hack_type_ is <tt>T*</tt>.
            /// Otherwise, it is some undefined type.
            typedef typename detail::address_of_hack<Tag, arg0_type>::type address_of_hack_type_;

            /// \return The address of <tt>this->arg0</tt> if \c Tag is
            /// \c boost::proto::tag::address_of. Otherwise, this function will
            /// fail to compile.
            ///
            /// \attention Proto overloads <tt>operator&</tt>, which means that
            /// proto-ified objects cannot have their addresses taken, unless we use
            /// the following hack to make \c &x implicitly convertible to \c X*.
            operator address_of_hack_type_() const
            {
                return boost::addressof(this->arg0.expr);
            }
        #endif

            /// Assignment
            ///
            /// \param a The rhs.
            /// \return A new \c expr\<\> node representing an assignment of \c a to \c *this.
            template<typename A>
            expr<tag::assign, args2<ref_<expr>, typename result_of::as_arg<A>::type> > const
            operator =(A &a)
            {
                expr<tag::assign, args2<ref_<expr>, typename result_of::as_arg<A>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::assign, args2<ref_<expr>, typename result_of::as_arg<A const>::type> > const
            operator =(A const &a)
            {
                expr<tag::assign, args2<ref_<expr>, typename result_of::as_arg<A const>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::assign, args2<ref_<expr const>, typename result_of::as_arg<A>::type> > const
            operator =(A &a) const
            {
                expr<tag::assign, args2<ref_<expr const>, typename result_of::as_arg<A>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::assign, args2<ref_<expr const>, typename result_of::as_arg<A const>::type> > const
            operator =(A const &a) const
            {
                expr<tag::assign, args2<ref_<expr const>, typename result_of::as_arg<A const>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// Subscript
            ///
            /// \param a The rhs.
            /// \return A new \c expr\<\> node representing \c *this subscripted with \c a.
            template<typename A>
            expr<tag::subscript, args2<ref_<expr>, typename result_of::as_arg<A>::type> > const
            operator [](A &a)
            {
                expr<tag::subscript, args2<ref_<expr>, typename result_of::as_arg<A>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::subscript, args2<ref_<expr>, typename result_of::as_arg<A const>::type> > const
            operator [](A const &a)
            {
                expr<tag::subscript, args2<ref_<expr>, typename result_of::as_arg<A const>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::subscript, args2<ref_<expr const>, typename result_of::as_arg<A>::type> > const
            operator [](A &a) const
            {
                expr<tag::subscript, args2<ref_<expr const>, typename result_of::as_arg<A>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            /// \overload
            ///
            template<typename A>
            expr<tag::subscript, args2<ref_<expr const>, typename result_of::as_arg<A const>::type> > const
            operator [](A const &a) const
            {
                expr<tag::subscript, args2<ref_<expr const>, typename result_of::as_arg<A const>::type> > that = {{*this}, proto::as_arg(a)};
                return that;
            }

            template<typename Sig>
            struct result {};

            /// Encodes the return type of \c expr\<\>::operator(), for use with \c boost::result_of\<\>
            ///
            template<typename This>
            struct result<This()>
            {
                typedef expr<tag::function, args1<ref_<expr const> > > type;
            };

            /// Function call
            ///
            /// \return A new \c expr\<\> node representing the function invocation of \c (*this)().
            expr<tag::function, args1<ref_<expr const> > > const
            operator ()() const
            {
                expr<tag::function, args1<ref_<expr const> > > that = {{*this}};
                return that;
            }

    #define BOOST_PP_ITERATION_PARAMS_2 (3, (1, BOOST_PP_DEC(BOOST_PROTO_MAX_ARITY), <boost/xpressive/proto/expr.hpp>))
    #include BOOST_PP_ITERATE()
        };

#elif BOOST_PP_ITERATION_DEPTH() == 2

    #define N BOOST_PP_ITERATION()

        template<typename This BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
        struct result<This(BOOST_PP_ENUM_PARAMS(N, A))>
          : result_of::BOOST_PP_CAT(funop, N)<expr const BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(N, typename remove_reference<A, >::type BOOST_PP_INTERCEPT)>
        {};

        /// \overload
        ///
        template<BOOST_PP_ENUM_PARAMS(N, typename A)>
        typename result_of::BOOST_PP_CAT(funop, N)<expr const BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>::type const
        operator ()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const &a)) const
        {
            return result_of::BOOST_PP_CAT(funop, N)<expr const BOOST_PP_ENUM_TRAILING_PARAMS(N, const A)>
                ::call(*this BOOST_PP_ENUM_TRAILING_PARAMS(N, a));
        }

    #undef N

#endif
