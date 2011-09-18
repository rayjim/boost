//  boost/chrono/stopwatches/stopwatch_formatter.hpp  ------------------------------------------------------------//
//  Copyright 2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/chrono/stopwatches for documentation.

#ifndef BOOST_CHRONO_STOPWATCHES_FORMATTERS_ELAPSED_HPP
#define BOOST_CHRONO_STOPWATCHES_FORMATTERS_ELAPSED_HPP

#include <boost/chrono/stopwatches/formatters/base_formatter.hpp>
#include <boost/system/error_code.hpp>
#include <boost/current_function.hpp>
//#include <boost/chrono/stopwatches/detail/adaptive_string.hpp>
#include <boost/format.hpp>
#include <boost/format/group.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <string>
#include <iostream>
#include <cassert>
#include <iomanip>

#define BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT_DEFAULT "%1%"

namespace boost
{
  namespace chrono
  {

    template<typename Ratio = micro, typename CharT = char,
        typename Traits = std::char_traits<CharT>,
        class Alloc = std::allocator<CharT> >
    class basic_elapsed_formatter: public base_formatter<CharT, Traits>
    {

    public:
      typedef base_formatter<CharT, Traits> base_type;
      typedef basic_format<CharT, Traits> format_type;
      typedef std::basic_string<CharT, Traits, Alloc> string_type;
      typedef CharT char_type;
      typedef std::basic_ostream<CharT, Traits> ostream_type;

      basic_elapsed_formatter() :
        base_type(),
            internal_fmt_(BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT_DEFAULT),
            fmt_(internal_fmt_)
      {
      }
      basic_elapsed_formatter(ostream_type& os) :
        base_type(os),
            internal_fmt_(BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT_DEFAULT),
            fmt_(internal_fmt_)
      {
      }
      basic_elapsed_formatter(const char* fmt, ostream_type& os = std::cout) :
        base_type(os), internal_fmt_(fmt), fmt_(internal_fmt_)
      {
      }
      basic_elapsed_formatter(string_type const& fmt, ostream_type& os =
          std::cout) :
        base_type(os), internal_fmt_(fmt), fmt_(internal_fmt_)
      {
      }
      basic_elapsed_formatter(format_type & fmt, ostream_type& os = std::cout) :
        base_type(os), fmt_(fmt)
      {
      }

//      static string_type format(const char* s)
//      {
//        string_type res(s);
//        res += boost::chrono::detail::adaptive_string(" : ");
//        res += BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT_DEFAULT;
//        return res;
//      }

      template<class Stopwatch>
      void operator()(Stopwatch & stopwatch_, system::error_code & ec)
      {
        typedef typename Stopwatch::duration duration_t;
        duration_t d = stopwatch_.elapsed(ec);

        if (d < duration_t::zero())
          return;

        this->os_
            << fmt_
                % io::group(std::fixed, std::setprecision(this->precision_), duration_fmt(this->style_), boost::chrono::duration<
                    double, Ratio>(d)) << std::endl;

      }
    private:
      boost::format internal_fmt_;
      boost::format& fmt_;
    };

    typedef basic_elapsed_formatter<micro, char> elapsed_formatter;
    typedef basic_elapsed_formatter<micro, wchar_t> welapsed_formatter;

  } // namespace chrono
} // namespace boost

#if 0
#define BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT(F) \
  boost::chrono::detail::adaptive_string(F " : " BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_CHRONO_STOPWATCHES_ELAPSED_FUNCTION_FORMAT \
  boost::chrono::elapsed_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_STOPWATCHES_ELAPSED_FUNCTION_FORMAT \
    BOOST_CHRONO_STOPWATCHES_ELAPSED_FORMAT(BOOST_CURRENT_FUNCTION)
#endif
#endif

#endif
