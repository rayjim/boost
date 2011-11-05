//  boost/chrono/io/ios_base_state.hpp
//
//  (C) Copyright 2010-2011 Vicente J. Botet Escriba
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
// This code was adapted by Vicente from Howard Hinnant's experimental work
// on chrono i/o to Boost

#ifndef BOOST_CHRONO_IO_IOS_BASE_STATE_HPP
#define BOOST_CHRONO_IO_IOS_BASE_STATE_HPP

#include <boost/chrono/config.hpp>
#include <locale>
#include <boost/chrono/io/duration_style.hpp>
#include <boost/chrono/io/timezone.hpp>
#include <boost/chrono/io/utility/ios_base_state_ptr.hpp>

namespace boost
{
  namespace chrono
  {

    class fmt_masks : ios_base_flags<fmt_masks>
    {
      typedef ios_base_flags<fmt_masks> base_type;

    public:
      fmt_masks(std::ios_base& ios): base_type(ios) {}
      enum type
      {
        uses_symbol = 1 << 0,
        uses_local  = 1 << 1
      };

      inline duration_style::type get_duration_style()
      {
        return (flags() & uses_symbol) ? duration_style::symbol : duration_style::prefix;
      }
      inline void set_duration_style(duration_style::type style)
      {
        if (style == duration_style::symbol)
          setf(uses_symbol);
        else
          unsetf(uses_symbol);
      }

      inline timezone_type get_timezone()
      {
        return (flags() & uses_local) ? timezone::local : timezone::utc;
      }
      inline void set_timezone(timezone_type tz)
      {
        if (tz == timezone::local)
          setf(uses_local);
        else
          unsetf(uses_local);
      }
    };

    inline duration_style::type get_duration_style(std::ios_base & ios)
    {
      return fmt_masks(ios).get_duration_style();
    }
    inline void set_duration_style(std::ios_base& ios, duration_style::type style)
    {
      fmt_masks(ios).set_duration_style(style);
    }

    inline timezone_type get_timezone(std::ios_base & ios)
    {
      return fmt_masks(ios).get_timezone();
    }
    inline void set_timezone(std::ios_base& ios, timezone_type tz)
    {
      fmt_masks(ios).set_timezone(tz);
    }

    namespace detail
    {

      template<typename CharT>
      struct ios_base_data_aux
      {
        std::basic_string<CharT> time_fmt;
        std::basic_string<CharT> duration_fmt;
      public:

        ios_base_data_aux() :
          time_fmt(""),
          duration_fmt("")
        {
        }
      };

    } // detail

    template<typename CharT>
    static inline std::basic_string<CharT> get_time_fmt(std::ios_base & ios)
    {
      ios_base_state<detail::ios_base_data_aux<CharT> > ptr(ios);
      return ptr->time_fmt;
    }
    template<typename CharT>
    static inline void set_time_fmt(std::ios_base& ios, std::basic_string<
        CharT> const& fmt)
    {
      ios_base_state<detail::ios_base_data_aux<CharT> > ptr(ios);
      ptr->time_fmt = fmt;
    }

  } // chrono
} // boost

#endif  // header
