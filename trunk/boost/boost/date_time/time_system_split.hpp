#ifndef DATE_TIME_TIME_SYSTEM_SPLIT_HPP
#define DATE_TIME_TIME_SYSTEM_SPLIT_HPP

/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland, Bart Garst
 */

#include <string>
#include "boost/date_time/compiler_config.hpp"

namespace boost {
namespace date_time {

  //! An unadjusted time system implementation.
#if (defined(BOOST_DATE_TIME_NO_MEMBER_INIT))  
  template<typename config, boost::int32_t ticks_per_second>
#else
  template<typename config>
#endif
  class split_timedate_system
  {
   public:
    typedef typename config::time_rep_type time_rep_type;
    typedef typename config::date_type     date_type;
    typedef typename config::time_duration_type time_duration_type;
    typedef typename config::date_duration_type date_duration_type;
    typedef typename config::int_type int_type;
    typedef typename config::resolution_traits   resolution_traits;

    //86400 is number of seconds in a day...
#if (defined(BOOST_DATE_TIME_NO_MEMBER_INIT))  
    typedef date_time::wrapping_int<int_type, INT64_C(86400) * ticks_per_second > wrap_int_type;
#else
   private:
     BOOST_STATIC_CONSTANT(int_type, ticks_per_day = INT64_C(86400) * config::tick_per_second);
   public:
    typedef date_time::wrapping_int<int_type, ticks_per_day> wrap_int_type;
#endif

    static time_rep_type get_time_rep(const date_type& day,
                                      const time_duration_type& tod,
                                      date_time::dst_flags dst=not_dst)
    {
      if(day.is_special() || tod.is_special())
      {
	if(day.is_not_a_date() || tod.is_not_a_date_time())
	{
	  return time_rep_type(date_type(not_a_date_time),
	      time_duration_type(not_a_date_time));
	}
	else if(day.is_pos_infinity())
	{
	  if(tod.is_neg_infinity())
	  {
	    return time_rep_type(date_type(not_a_date_time),
		time_duration_type(not_a_date_time));
	  }
	  else{
	    return time_rep_type(day, time_duration_type(pos_infin));
	  }
	}
	else if(day.is_neg_infinity())
	{
	  if(tod.is_pos_infinity())
	  {
	    return time_rep_type(date_type(not_a_date_time),
		time_duration_type(not_a_date_time));
	  }
	  else{
	    return time_rep_type(day, time_duration_type(neg_infin));
	  }
	}
	else if(tod.is_pos_infinity())
	{
	  if(day.is_neg_infinity())
	  {
	    return time_rep_type(date_type(not_a_date_time),
		time_duration_type(not_a_date_time));
	  }
	  else{
	    return time_rep_type(date_type(pos_infin), tod);
	  }
	}
	else if(tod.is_neg_infinity())
	{
	  if(day.is_pos_infinity())
	  {
	    return time_rep_type(date_type(not_a_date_time),
		time_duration_type(not_a_date_time));
	  }
	  else{
	    return time_rep_type(date_type(neg_infin), tod);
	  }
	}

      }
      return time_rep_type(day, tod);
    }
    static date_type get_date(const time_rep_type& val)
    {
      return date_type(val.day);
    }
    static time_duration_type get_time_of_day(const time_rep_type& val)
    {
      return time_duration_type(val.time_of_day);
    }
    static std::string zone_name(const time_rep_type&)
    {
      return "";
    }
    static bool is_equal(const time_rep_type& lhs, const time_rep_type& rhs)
    {
      return ((lhs.day == rhs.day) && (lhs.time_of_day == rhs.time_of_day));
      //      return true;
    }
    static bool is_less(const time_rep_type& lhs, const time_rep_type& rhs)
    {
      if (lhs.day < rhs.day) return true;
      if (lhs.day > rhs.day) return false;
      return (lhs.time_of_day < rhs.time_of_day);
    }
    static time_rep_type add_days(const time_rep_type& base,
                                  const date_duration_type& dd)
    {
      return time_rep_type(base.day+dd, base.time_of_day);
    }
    static time_rep_type subtract_days(const time_rep_type& base,
                                       const date_duration_type& dd)
    {
      return split_timedate_system::get_time_rep(base.day-dd, base.time_of_day);
      //return time_rep_type(base.day-dd, base.time_of_day);
    }
    static time_rep_type subtract_time_duration(const time_rep_type& base,
                                                const time_duration_type& td)
    {
      if(base.day.is_special() || td.is_special())
      {
        return split_timedate_system::get_time_rep(base.day, -td);
      }

      wrap_int_type  day_offset(base.time_of_day.ticks());
      date_duration_type day_overflow(static_cast<int_type>(day_offset.subtract(td.ticks())));
//       std::cout << "sub: " << base.time_of_day.ticks() << "|"
//                 << day_offset.as_int() << "|"
//                 << day_overflow.days() << std::endl;
      return time_rep_type(base.day-day_overflow,
                           time_duration_type(0,0,0,day_offset.as_int()));
    }
    static time_rep_type add_time_duration(const time_rep_type& base,
                                           time_duration_type td)
    {
      if(base.day.is_special() || td.is_special())
      {
        return split_timedate_system::get_time_rep(base.day, td);
      }
      if (td.is_negative()) {
        time_duration_type td1 = td.invert_sign();
        return subtract_time_duration(base,td1);
      }
      wrap_int_type day_offset(base.time_of_day.ticks());      
      int_type doff = day_offset.add(td.ticks());
//       std::cout << "day overflow: " << doff << std::endl;
//       std::cout << "ticks: "         << td.ticks() << std::endl;
      date_duration_type day_overflow(static_cast<int_type>(doff));
//       std::cout << "base: " << to_simple_string(base.day) << std::endl;
//       std::cout << "overflow " << day_overflow.days() << std::endl;
      return time_rep_type(base.day+day_overflow,
                           time_duration_type(0,0,0,day_offset.as_int()));
    }
    static time_duration_type subtract_times(const time_rep_type& lhs,
                                             const time_rep_type& rhs)
    {
      date_duration_type dd = lhs.day - rhs.day;
      time_duration_type td(dd.days()*24,0,0); //days * 24 hours
      time_duration_type td2 = lhs.time_of_day - rhs.time_of_day;
      return td+td2;
      // return time_rep_type(base.day-dd, base.time_of_day);
    }
    
  };

} } //namespace date_time

/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#endif
