//  Copyright (c) 2007 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This header just defines the function entry points, and adds dispatch
// to the right implementation method.  Most of the implementation details
// are in separate headers and copyright Xiaogang Zhang.
//
#ifndef BOOST_MATH_BESSEL_HPP
#define BOOST_MATH_BESSEL_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/math/special_functions/detail/bessel_jy.hpp>
#include <boost/math/special_functions/detail/bessel_jn.hpp>
#include <boost/math/special_functions/detail/bessel_yn.hpp>
#include <boost/math/special_functions/detail/bessel_jy_zero.hpp>
#include <boost/math/special_functions/detail/bessel_ik.hpp>
#include <boost/math/special_functions/detail/bessel_i0.hpp>
#include <boost/math/special_functions/detail/bessel_i1.hpp>
#include <boost/math/special_functions/detail/bessel_kn.hpp>
#include <boost/math/special_functions/detail/iconv.hpp>
#include <boost/math/special_functions/sin_pi.hpp>
#include <boost/math/special_functions/cos_pi.hpp>
#include <boost/math/special_functions/sinc.hpp>
#include <boost/math/special_functions/trunc.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/math/tools/promotion.hpp>
#include <boost/math/tools/series.hpp>
#include <boost/math/tools/roots.hpp>

namespace boost{ namespace math{

namespace detail{

template <class T, class Policy>
struct sph_bessel_j_small_z_series_term
{
   typedef T result_type;

   sph_bessel_j_small_z_series_term(unsigned v_, T x)
      : N(0), v(v_)
   {
      BOOST_MATH_STD_USING
      mult = x / 2;
      term = pow(mult, T(v)) / boost::math::tgamma(v+1+T(0.5f), Policy());
      mult *= -mult;
   }
   T operator()()
   {
      T r = term;
      ++N;
      term *= mult / (N * T(N + v + 0.5f));
      return r;
   }
private:
   unsigned N;
   unsigned v;
   T mult;
   T term;
};

template <class T, class Policy>
inline T sph_bessel_j_small_z_series(unsigned v, T x, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names
   sph_bessel_j_small_z_series_term<T, Policy> s(v, x);
   boost::uintmax_t max_iter = policies::get_max_series_iterations<Policy>();
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
   T zero = 0;
   T result = boost::math::tools::sum_series(s, boost::math::policies::get_epsilon<T, Policy>(), max_iter, zero);
#else
   T result = boost::math::tools::sum_series(s, boost::math::policies::get_epsilon<T, Policy>(), max_iter);
#endif
   policies::check_series_iterations<T>("boost::math::sph_bessel_j_small_z_series<%1%>(%1%,%1%)", max_iter, pol);
   return result * sqrt(constants::pi<T>() / 4);
}

template <class T, class Policy>
T cyl_bessel_j_imp(T v, T x, const bessel_no_int_tag& t, const Policy& pol)
{
   BOOST_MATH_STD_USING
   static const char* function = "boost::math::bessel_j<%1%>(%1%,%1%)";
   if(x < 0)
   {
      // better have integer v:
      if(floor(v) == v)
      {
         T r = cyl_bessel_j_imp(v, T(-x), t, pol);
         if(iround(v, pol) & 1)
            r = -r;
         return r;
      }
      else
         return policies::raise_domain_error<T>(
            function,
            "Got x = %1%, but we need x >= 0", x, pol);
   }
   if(x == 0)
      return (v == 0) ? 1 : (v > 0) ? 0 : 
         policies::raise_domain_error<T>(
            function, 
            "Got v = %1%, but require v >= 0 or a negative integer: the result would be complex.", v, pol);
   
   
   if((v >= 0) && ((x < 1) || (v > x * x / 4) || (x < 5)))
   {
      //
      // This series will actually converge rapidly for all small
      // x - say up to x < 20 - but the first few terms are large
      // and divergent which leads to large errors :-(
      //
      return bessel_j_small_z_series(v, x, pol);
   }
   
   T j, y;
   bessel_jy(v, x, &j, &y, need_j, pol);
   return j;
}

template <class T, class Policy>
inline T cyl_bessel_j_imp(T v, T x, const bessel_maybe_int_tag&, const Policy& pol)
{
   BOOST_MATH_STD_USING  // ADL of std names.
   int ival = detail::iconv(v, pol);
   if((abs(ival) < 200) && (0 == v - ival))
   {
      return bessel_jn(ival/*iround(v, pol)*/, x, pol);
   }
   return cyl_bessel_j_imp(v, x, bessel_no_int_tag(), pol);
}

template <class T, class Policy>
inline T cyl_bessel_j_imp(int v, T x, const bessel_int_tag&, const Policy& pol)
{
   BOOST_MATH_STD_USING
   return bessel_jn(v, x, pol);
}

template <class T, class Policy>
inline T sph_bessel_j_imp(unsigned n, T x, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names
   if(x < 0)
      return policies::raise_domain_error<T>(
         "boost::math::sph_bessel_j<%1%>(%1%,%1%)",
         "Got x = %1%, but function requires x > 0.", x, pol);
   //
   // Special case, n == 0 resolves down to the sinus cardinal of x:
   //
   if(n == 0)
      return boost::math::sinc_pi(x, pol);
   //
   // When x is small we may end up with 0/0, use series evaluation
   // instead, especially as it converges rapidly:
   //
   if(x < 1)
      return sph_bessel_j_small_z_series(n, x, pol);
   //
   // Default case is just a naive evaluation of the definition:
   //
   return sqrt(constants::pi<T>() / (2 * x)) 
      * cyl_bessel_j_imp(T(T(n)+T(0.5f)), x, bessel_no_int_tag(), pol);
}

template <class T, class Policy>
T cyl_bessel_i_imp(T v, T x, const Policy& pol)
{
   //
   // This handles all the bessel I functions, note that we don't optimise
   // for integer v, other than the v = 0 or 1 special cases, as Millers
   // algorithm is at least as inefficient as the general case (the general
   // case has better error handling too).
   //
   BOOST_MATH_STD_USING
   if(x < 0)
   {
      // better have integer v:
      if(floor(v) == v)
      {
         T r = cyl_bessel_i_imp(v, T(-x), pol);
         if(iround(v, pol) & 1)
            r = -r;
         return r;
      }
      else
         return policies::raise_domain_error<T>(
         "boost::math::cyl_bessel_i<%1%>(%1%,%1%)",
            "Got x = %1%, but we need x >= 0", x, pol);
   }
   if(x == 0)
   {
      return (v == 0) ? 1 : 0;
   }
   if(v == 0.5f)
   {
      // common special case, note try and avoid overflow in exp(x):
      if(x >= tools::log_max_value<T>())
      {
         T e = exp(x / 2);
         return e * (e / sqrt(2 * x * constants::pi<T>()));
      }
      return sqrt(2 / (x * constants::pi<T>())) * sinh(x);
   }
   if(policies::digits<T, Policy>() <= 64)
   {
      if(v == 0)
      {
         return bessel_i0(x);
      }
      if(v == 1)
      {
         return bessel_i1(x);
      }
   }
   if((v > 0) && (x / v < 0.25))
      return bessel_i_small_z_series(v, x, pol);
   T I, K;
   bessel_ik(v, x, &I, &K, need_i, pol);
   return I;
}

template <class T, class Policy>
inline T cyl_bessel_k_imp(T v, T x, const bessel_no_int_tag& /* t */, const Policy& pol)
{
   static const char* function = "boost::math::cyl_bessel_k<%1%>(%1%,%1%)";
   BOOST_MATH_STD_USING
   if(x < 0)
   {
      return policies::raise_domain_error<T>(
         function,
         "Got x = %1%, but we need x > 0", x, pol);
   }
   if(x == 0)
   {
      return (v == 0) ? policies::raise_overflow_error<T>(function, 0, pol)
         : policies::raise_domain_error<T>(
         function,
         "Got x = %1%, but we need x > 0", x, pol);
   }
   T I, K;
   bessel_ik(v, x, &I, &K, need_k, pol);
   return K;
}

template <class T, class Policy>
inline T cyl_bessel_k_imp(T v, T x, const bessel_maybe_int_tag&, const Policy& pol)
{
   BOOST_MATH_STD_USING
   if((floor(v) == v))
   {
      return bessel_kn(itrunc(v), x, pol);
   }
   return cyl_bessel_k_imp(v, x, bessel_no_int_tag(), pol);
}

template <class T, class Policy>
inline T cyl_bessel_k_imp(int v, T x, const bessel_int_tag&, const Policy& pol)
{
   return bessel_kn(v, x, pol);
}

template <class T, class Policy>
inline T cyl_neumann_imp(T v, T x, const bessel_no_int_tag&, const Policy& pol)
{
   static const char* function = "boost::math::cyl_neumann<%1%>(%1%,%1%)";

   BOOST_MATH_INSTRUMENT_VARIABLE(v);
   BOOST_MATH_INSTRUMENT_VARIABLE(x);

   if(x <= 0)
   {
      return (v == 0) && (x == 0) ?
         policies::raise_overflow_error<T>(function, 0, pol)
         : policies::raise_domain_error<T>(
               function,
               "Got x = %1%, but result is complex for x <= 0", x, pol);
   }
   T j, y;
   bessel_jy(v, x, &j, &y, need_y, pol);
   // 
   // Post evaluation check for internal overflow during evaluation,
   // can occur when x is small and v is large, in which case the result
   // is -INF:
   //
   if(!(boost::math::isfinite)(y))
      return -policies::raise_overflow_error<T>(function, 0, pol);
   return y;
}

template <class T, class Policy>
inline T cyl_neumann_imp(T v, T x, const bessel_maybe_int_tag&, const Policy& pol)
{
   BOOST_MATH_STD_USING
   typedef typename bessel_asymptotic_tag<T, Policy>::type tag_type;

   BOOST_MATH_INSTRUMENT_VARIABLE(v);
   BOOST_MATH_INSTRUMENT_VARIABLE(x);

   if(floor(v) == v)
   {
      if((fabs(x) > asymptotic_bessel_y_limit<T>(tag_type())) && (4 * fabs(x) * sqrt(sqrt(sqrt(14 * tools::epsilon<T>() * (fabs(x) + fabs(v) / 2) / (5120 * fabs(x))))) > fabs(v)))
      {
         T r = asymptotic_bessel_y_large_x_2(static_cast<T>(abs(v)), x);
         if((v < 0) && (itrunc(v, pol) & 1))
            r = -r;
         BOOST_MATH_INSTRUMENT_VARIABLE(r);
         return r;
      }
      else
      {
         T r = bessel_yn(itrunc(v, pol), x, pol);
         BOOST_MATH_INSTRUMENT_VARIABLE(r);
         return r;
      }
   }
   T r = cyl_neumann_imp<T>(v, x, bessel_no_int_tag(), pol);
   BOOST_MATH_INSTRUMENT_VARIABLE(r);
   return r;
}

template <class T, class Policy>
inline T cyl_neumann_imp(int v, T x, const bessel_int_tag&, const Policy& pol)
{
   BOOST_MATH_STD_USING
   typedef typename bessel_asymptotic_tag<T, Policy>::type tag_type;

   BOOST_MATH_INSTRUMENT_VARIABLE(v);
   BOOST_MATH_INSTRUMENT_VARIABLE(x);

   if((fabs(x) > asymptotic_bessel_y_limit<T>(tag_type())) && (4 * fabs(x) * sqrt(sqrt(sqrt(14 * tools::epsilon<T>() * (fabs(x) + abs(v) / 2) / (5120 * x)))) > abs(v)))
   {
      T r = asymptotic_bessel_y_large_x_2(static_cast<T>(abs(v)), x);
      if((v < 0) && (v & 1))
         r = -r;
      return r;
   }
   else
      return bessel_yn(v, x, pol);
}

template <class T, class Policy>
inline T sph_neumann_imp(unsigned v, T x, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names
   static const char* function = "boost::math::sph_neumann<%1%>(%1%,%1%)";
   //
   // Nothing much to do here but check for errors, and
   // evaluate the function's definition directly:
   //
   if(x < 0)
      return policies::raise_domain_error<T>(
         function,
         "Got x = %1%, but function requires x > 0.", x, pol);

   if(x < 2 * tools::min_value<T>())
      return -policies::raise_overflow_error<T>(function, 0, pol);

   T result = cyl_neumann_imp(T(T(v)+0.5f), x, bessel_no_int_tag(), pol);
   T tx = sqrt(constants::pi<T>() / (2 * x));

   if((tx > 1) && (tools::max_value<T>() / tx < result))
      return -policies::raise_overflow_error<T>(function, 0, pol);

   return result * tx;
}

template <class T, class Policy>
inline T cyl_bessel_j_zero_imp(T v, unsigned m, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names, needed for log.

   // Handle negative order or if the zero'th zero is requested.
   // Return NaN if NaN is available or return 0 if NaN is not available.
   if((v < T(0)) || (m == 0U))
      return (std::numeric_limits<T>::has_quiet_NaN ? std::numeric_limits<T>::quiet_NaN() : T(0));

   // Set up the initial guess for the upcoming root-finding.
   const T guess_root = boost::math::detail::bessel_zero::cyl_bessel_j_zero_detail::initial_guess<T>(v, m);

   // Select the maximum allowed iterations, being at least 24.
   boost::uintmax_t number_of_iterations = (std::max)(24, int(std::numeric_limits<T>::digits10));

   // Select the desired number of binary digits of precision.
   // Account for the radix of number representations having non-two radix!
   const int my_digits2 = int(float(std::numeric_limits<T>::digits)
                              * (  log(float(std::numeric_limits<T>::radix))
                                 / log(2.0F)));

   // Perform the root-finding using Newton-Raphson iteration from Boost.Math.
   const T jvm =
      boost::math::tools::newton_raphson_iterate(
         boost::math::detail::bessel_zero::cyl_bessel_j_zero_detail::function_object<T, Policy>(v, pol),
         guess_root,
         T(guess_root - 0.3F),
         T(guess_root + 0.3F),
         my_digits2,
         number_of_iterations);

   static_cast<void>(number_of_iterations);

   return jvm;
}

template <class output_iterator, class T, class Policy>
inline void cyl_bessel_j_zero_imp(output_iterator out_it,
                                  T v,
                                  unsigned number_of_zeros,
                                  unsigned start_index,
                                  const Policy& pol)
{
   if(number_of_zeros > 0U)
   {
      const output_iterator end_it(out_it + number_of_zeros);

      while(out_it != end_it)
      {
         *out_it = boost::math::detail::cyl_bessel_j_zero_imp<T, Policy>(v, start_index, pol);
         ++start_index;
         ++out_it;
      }
   }
}

template <class T, class Policy>
inline T cyl_neumann_zero_imp(T v, unsigned m, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names, needed for log.

   // Handle negative order or if the zero'th zero is requested.
   // Return NaN if NaN is available or return 0 if NaN is not available.
   if((v < T(0)) || (m == 0U))
      return (std::numeric_limits<T>::has_quiet_NaN ? std::numeric_limits<T>::quiet_NaN() : T(0));

   // Set up the initial guess for the upcoming root-finding.
   const T guess_root = boost::math::detail::bessel_zero::cyl_neumann_zero_detail::initial_guess<T>(v, m);

   // Select the maximum allowed iterations, being at least 24.
   boost::uintmax_t number_of_iterations = (std::max)(24, int(std::numeric_limits<T>::digits10));

   // Select the desired number of binary digits of precision.
   // Account for the radix of number representations having non-two radix!
   const int my_digits2 = int(float(std::numeric_limits<T>::digits)
                              * (  log(float(std::numeric_limits<T>::radix))
                                 / log(2.0F)));

   // Perform the root-finding using Newton-Raphson iteration from Boost.Math.
   const T yvm =
      boost::math::tools::newton_raphson_iterate(
         boost::math::detail::bessel_zero::cyl_neumann_zero_detail::function_object<T, Policy>(v, pol),
         guess_root,
         T(guess_root - 0.3F),
         T(guess_root + 0.3F),
         my_digits2,
         number_of_iterations);

   static_cast<void>(number_of_iterations);

   return yvm;
}

template <class output_iterator, class T, class Policy>
inline void cyl_neumann_zero_imp(output_iterator out_it,
                                 T v,
                                 unsigned number_of_zeros,
                                 unsigned start_index,
                                 const Policy& pol)
{
   if(number_of_zeros > 0U)
   {
      const output_iterator end_it(out_it + number_of_zeros);

      while(out_it != end_it)
      {
         *out_it = boost::math::detail::cyl_neumann_zero_imp<T, Policy>(v, start_index, pol);
         ++start_index;
         ++out_it;
      }
   }
}

} // namespace detail

template <class T1, class T2, class Policy>
inline typename detail::bessel_traits<T1, T2, Policy>::result_type cyl_bessel_j(T1 v, T2 x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T1, T2, Policy>::result_type result_type;
   typedef typename detail::bessel_traits<T1, T2, Policy>::optimisation_tag tag_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_bessel_j_imp<value_type>(v, static_cast<value_type>(x), tag_type(), pol), "boost::math::cyl_bessel_j<%1%>(%1%,%1%)");
}

template <class T1, class T2>
inline typename detail::bessel_traits<T1, T2, policies::policy<> >::result_type cyl_bessel_j(T1 v, T2 x)
{
   return cyl_bessel_j(v, x, policies::policy<>());
}

template <class T, class Policy>
inline typename detail::bessel_traits<T, T, Policy>::result_type sph_bessel(unsigned v, T x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::sph_bessel_j_imp<value_type>(v, static_cast<value_type>(x), pol), "boost::math::sph_bessel<%1%>(%1%,%1%)");
}

template <class T>
inline typename detail::bessel_traits<T, T, policies::policy<> >::result_type sph_bessel(unsigned v, T x)
{
   return sph_bessel(v, x, policies::policy<>());
}

template <class T1, class T2, class Policy>
inline typename detail::bessel_traits<T1, T2, Policy>::result_type cyl_bessel_i(T1 v, T2 x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T1, T2, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_bessel_i_imp<value_type>(v, static_cast<value_type>(x), pol), "boost::math::cyl_bessel_i<%1%>(%1%,%1%)");
}

template <class T1, class T2>
inline typename detail::bessel_traits<T1, T2, policies::policy<> >::result_type cyl_bessel_i(T1 v, T2 x)
{
   return cyl_bessel_i(v, x, policies::policy<>());
}

template <class T1, class T2, class Policy>
inline typename detail::bessel_traits<T1, T2, Policy>::result_type cyl_bessel_k(T1 v, T2 x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T1, T2, Policy>::result_type result_type;
   typedef typename detail::bessel_traits<T1, T2, Policy>::optimisation_tag tag_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_bessel_k_imp<value_type>(v, static_cast<value_type>(x), tag_type(), pol), "boost::math::cyl_bessel_k<%1%>(%1%,%1%)");
}

template <class T1, class T2>
inline typename detail::bessel_traits<T1, T2, policies::policy<> >::result_type cyl_bessel_k(T1 v, T2 x)
{
   return cyl_bessel_k(v, x, policies::policy<>());
}

template <class T1, class T2, class Policy>
inline typename detail::bessel_traits<T1, T2, Policy>::result_type cyl_neumann(T1 v, T2 x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T1, T2, Policy>::result_type result_type;
   typedef typename detail::bessel_traits<T1, T2, Policy>::optimisation_tag tag_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_neumann_imp<value_type>(v, static_cast<value_type>(x), tag_type(), pol), "boost::math::cyl_neumann<%1%>(%1%,%1%)");
}

template <class T1, class T2>
inline typename detail::bessel_traits<T1, T2, policies::policy<> >::result_type cyl_neumann(T1 v, T2 x)
{
   return cyl_neumann(v, x, policies::policy<>());
}

template <class T, class Policy>
inline typename detail::bessel_traits<T, T, Policy>::result_type sph_neumann(unsigned v, T x, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   return policies::checked_narrowing_cast<result_type, Policy>(detail::sph_neumann_imp<value_type>(v, static_cast<value_type>(x), pol), "boost::math::sph_neumann<%1%>(%1%,%1%)");
}

template <class T>
inline typename detail::bessel_traits<T, T, policies::policy<> >::result_type sph_neumann(unsigned v, T x)
{
   return sph_neumann(v, x, policies::policy<>());
}

template <class T, class Policy>
inline typename detail::bessel_traits<T, T, Policy>::result_type cyl_bessel_j_zero(T v, unsigned m, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_bessel_j_zero_imp<value_type>(v, m, pol), "boost::math::cyl_bessel_j_zero<%1%>(%1%,%1%)");
}

template <class T>
inline typename detail::bessel_traits<T, T, policies::policy<> >::result_type cyl_bessel_j_zero(T v, unsigned m)
{
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<T>::is_integer, "Order must be a floating-point type.");
   return cyl_bessel_j_zero_imp<T, policies::policy<> >(v, m, policies::policy<>());
}

template <class output_iterator, class T, class Policy>
inline void cyl_bessel_j_zero(output_iterator out_it,
                              T v,
                              unsigned number_of_zeros,
                              unsigned start_index,
                              const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   detail::cyl_bessel_j_zero_imp<output_iterator, value_type, Policy>(out_it, v, number_of_zeros, start_index, pol);
}

template <class output_iterator, class T>
inline void cyl_bessel_j_zero(output_iterator out_it,
                              T v,
                              unsigned number_of_zeros,
                              unsigned start_index)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, policies::policy<> >::result_type result_type;
   typedef typename policies::evaluation<result_type, policies::policy<> >::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   detail::cyl_bessel_j_zero_imp<output_iterator, value_type, policies::policy<> >(out_it, v, number_of_zeros, start_index, policies::policy<>());
}

template <class T, class Policy>
inline typename detail::bessel_traits<T, T, Policy>::result_type cyl_neumann_zero(T v, unsigned m, const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   return policies::checked_narrowing_cast<result_type, Policy>(detail::cyl_neumann_zero_imp<value_type>(v, m, pol), "boost::math::cyl_neumann_zero<%1%>(%1%,%1%)");
}

template <class T>
inline typename detail::bessel_traits<T, T, policies::policy<> >::result_type cyl_neumann_zero(T v, unsigned m)
{
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<T>::is_integer, "Order must be a floating-point type.");
   return cyl_neumann_zero_imp<T, policies::policy<> >(v, m, policies::policy<>());
}

template <class output_iterator, class T, class Policy>
inline void cyl_neumann_zero(output_iterator out_it,
                             T v,
                             unsigned number_of_zeros,
                             unsigned start_index,
                             const Policy& pol)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, Policy>::result_type result_type;
   typedef typename policies::evaluation<result_type, Policy>::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   detail::cyl_neumann_zero_imp<output_iterator, value_type, Policy>(out_it, v, number_of_zeros, start_index, pol);
}

template <class output_iterator, class T>
inline void cyl_neumann_zero(output_iterator out_it,
                             T v,
                             unsigned number_of_zeros,
                             unsigned start_index)
{
   BOOST_FPU_EXCEPTION_GUARD
   typedef typename detail::bessel_traits<T, T, policies::policy<> >::result_type result_type;
   typedef typename policies::evaluation<result_type, policies::policy<> >::type value_type;
   BOOST_STATIC_ASSERT_MSG(false == std::numeric_limits<value_type>::is_integer, "Order must be a floating-point type.");
   detail::cyl_neumann_zero_imp<output_iterator, value_type, policies::policy<> >(out_it, v, number_of_zeros, start_index, policies::policy<>());
}

} // namespace math
} // namespace boost

#endif // BOOST_MATH_BESSEL_HPP
