#ifndef GREG_DATE_HPP___
#define GREG_DATE_HPP___
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

#include "boost/date_time/date.hpp"
#include "boost/date_time/special_defs.hpp"
#include "boost/date_time/gregorian/greg_calendar.hpp"
#include "boost/date_time/gregorian/greg_duration.hpp"

namespace boost {
namespace gregorian {

  //bring special enum values into the namespace
  using date_time::special_values;
  using date_time::not_special;
  using date_time::neg_infin;
  using date_time::pos_infin;
  using date_time::not_a_date_time;
  using date_time::max_date_time;
  using date_time::min_date_time;

  //! A date type based on gregorian_calendar
  /*! This class is the primary interface for programming with 
      greogorian dates.  The is a lightweight type that can be
      freely passed by value.  All comparison operators are 
      supported.  
      \ingroup date_basics
  */
  class date : public date_time::date<date, gregorian_calendar, date_duration>
  {
   public:
    typedef gregorian_calendar::year_type year_type;
    typedef gregorian_calendar::month_type month_type;
    typedef gregorian_calendar::day_type day_type;
    typedef gregorian_calendar::day_of_year_type day_of_year_type;
    typedef gregorian_calendar::ymd_type ymd_type;
    typedef gregorian_calendar::date_rep_type date_rep_type;
    typedef gregorian_calendar::date_int_type date_int_type;
    typedef date_duration  duration_type;
    //! Main constructor with year, month, day
    date(year_type year, month_type month, day_type day) 
      : date_time::date<date, gregorian_calendar, date_duration>(year, month, day)
    {
      if (gregorian_calendar::end_of_month_day(year, month) < day) {
        throw bad_day_of_month(std::string("Day of month is not valid for year"));
      }
    }
    //! Constructor from a ymd_type structure
    explicit date(const ymd_type& ymd) 
      : date_time::date<date, gregorian_calendar, date_duration>(ymd)
    {}
    //! Needed copy constructor
    explicit date(const date_int_type& rhs):
      date_time::date<date,gregorian_calendar, date_duration>(rhs)
    {}
    //! Needed copy constructor
    explicit date(date_rep_type rhs):
      date_time::date<date,gregorian_calendar, date_duration>(rhs)
    {}
    //! Constructor for infinities, not a date, max and min date
    explicit date(special_values sv):
      date_time::date<date, gregorian_calendar, date_duration>(date_rep_type::from_special(sv))
    {}
    //!Return the Julian Day number for the date.
    date_int_type julian_day() const
    {
      ymd_type ymd = year_month_day();
      return gregorian_calendar::julian_day_number(ymd);
    }
    //!Return the day of year 1..365 or 1..366 (for leap year)
    day_of_year_type day_of_year() const
    {
      date start_of_year(year(), 1, 1);
      unsigned short doy = static_cast<unsigned short>((*this-start_of_year).days() + 1);
      return day_of_year_type(doy);
    }
    //!Return the Modified Julian Day number for the date.
    long modjulian_day() const
    {
      ymd_type ymd = year_month_day();
      return gregorian_calendar::modjulian_day_number(ymd);      
    }
    //!Return the iso 8601 week number 1..53
    int week_number() const
    {
      ymd_type ymd = year_month_day();
      return gregorian_calendar::week_number(ymd);      
    }
    //! Return the day number from the calendar
    date_int_type day_number() const
    {
      return days_;
    }

   private:

  };
  


} } //namespace gregorian

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
 */


#endif
