// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef MAKE_KEYWORD_RANGE_FN_DWA2002927_HPP
# define MAKE_KEYWORD_RANGE_FN_DWA2002927_HPP

# include <boost/python/make_function.hpp>
# include <boost/python/args_fwd.hpp>

# include <boost/python/object/make_holder.hpp>

# include <boost/mpl/size.hpp>


namespace boost { namespace python { namespace detail { 

// Think of this as a version of make_function without a compile-time
// check that the size of kw is no greater than the expected arity of
// F. This version is needed when defining functions with default
// arguments, because compile-time information about the number of
// keywords is missing for all but the initial function definition.
//
// {
template <class F, class Policies>
object make_keyword_range_function(
    F f
  , Policies const& policies
  , keyword_range const& kw)
{
    return detail::make_function_aux(
        f, policies, detail::get_signature(f), kw, mpl::int_<0>());
}

template <class F, class Policies, class Signature>
object make_keyword_range_function(
    F f
  , Policies const& policies
  , keyword_range const& kw
  , Signature const& sig)
{
    return detail::make_function_aux(
        f, policies, sig, kw, mpl::int_<0>());
}
// }

// Builds an '__init__' function which inserts the given Holder type
// in a wrapped C++ class instance. ArgList is an MPL type sequence
// describing the C++ argument types to be passed to Holder's
// constructor.
//
// Holder and ArgList are intended to be explicitly specified. 
template <class ArgList, class Arity, class Holder, class CallPolicies>
object make_keyword_range_constructor(
    CallPolicies const& policies        // The CallPolicies with which to invoke the Holder's constructor
    , detail::keyword_range const& kw   // The (possibly empty) set of associated argument keywords
    , Holder* = 0                       
    , ArgList* = 0, Arity* = 0)
{
    return detail::make_keyword_range_function(
        objects::make_holder<Arity::value>
            ::template apply<Holder,ArgList>::execute
        , policies
        , kw);
}

}}} // namespace boost::python::detail

#endif // MAKE_KEYWORD_RANGE_FN_DWA2002927_HPP
