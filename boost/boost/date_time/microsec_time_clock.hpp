#ifndef DATE_TIME_HIGHRES_TIME_CLOCK_HPP___
#define DATE_TIME_HIGHRES_TIME_CLOCK_HPP___

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst 
 * $Date$
 */


/*! @file microsec_time_clock.hpp
  This file contains a high resolution time clock implementation.
*/

#include "boost/date_time/c_time.hpp"
#include "boost/cstdint.hpp"

#ifdef BOOST_HAS_FTIME
#include <Windows.h>
#endif

#ifdef BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK

namespace boost {
namespace date_time {


  //! A clock providing microsecond level resolution 
  /*! A high precision clock that measures the local time
   *  at a resolution up to microseconds and adjusts to the
   *  resolution of the time system.  For example, for the
   *  a library configuration with nano second resolution,
   *  the last 3 places of the fractional seconds will always
   *  be 000 since there are 1000 nano-seconds in a micro second.
   */
  template<class time_type> 
  class microsec_clock
  {
  public:
    typedef typename time_type::date_type date_type;
    typedef typename time_type::time_duration_type time_duration_type;
    typedef typename time_duration_type::rep_type resolution_traits_type;
    
#ifdef BOOST_HAS_GETTIMEOFDAY
    //! Return the local time based on computer clock settings
    static time_type local_time() {
      timeval tv;
      gettimeofday(&tv, 0); //gettimeofday does not support TZ adjust on Linux.
      return create_time(&tv);
    }
    
  private:
    static time_type create_time(timeval* tv) {
      time_t t = tv->tv_sec;
      boost::uint32_t fs = tv->tv_usec;
      ::std::time(&t); 
      tm* curr = localtime(&t);
      date_type d(curr->tm_year + 1900, 
                  curr->tm_mon + 1, 
                  curr->tm_mday);
      //The following line will adjusts the fractional second tick in terms
      //of the current time system.  For example, if the time system
      //doesn't support fractional seconds then res_adjust returns 0
      //and all the fractional seconds return 0.
      int adjust = resolution_traits_type::res_adjust()/1000000;

      time_duration_type td(curr->tm_hour,
                            curr->tm_min,
                            curr->tm_sec,
                            fs*adjust);
      return time_type(d,td);
      
    }
#endif // BOOST_HAS_GETTIMEOFDAY

#ifdef BOOST_HAS_FTIME
    //! Return the local time based on computer clock settings
    static time_type local_time() {
      //SYSTEMTIME st;
      //GetSystemTime(&st);
      FILETIME ft;
      GetSystemTimeAsFileTime(&ft);
      return create_time(ft);
    }
    
  private:
    static time_type create_time(FILETIME& ft) {
      // this offset calculation was borrowed from the boost::threads library
      const boost::uint64_t TIMESPEC_TO_FILETIME_OFFSET =
        ((boost::uint64_t)27111902UL << 32) +
        (boost::uint64_t)3577643008UL;
      boost::int_fast32_t sec = 
        (int)((*(__int64*)&ft - TIMESPEC_TO_FILETIME_OFFSET) / 10000000);
      boost::int_fast32_t sub_sec = 
        (int)((*(__int64*)&ft - TIMESPEC_TO_FILETIME_OFFSET -
               ((__int64)sec * (__int64)10000000)) / 10); // in microseconds

     
      time_t t;
      ::std::time(&t); 
      tm* curr = localtime(&t);
      date_type d(curr->tm_year + 1900, 
                  curr->tm_mon + 1, 
                  curr->tm_mday);

      //The following line will adjusts the fractional second tick in terms
      //of the current time system.  For example, if the time system
      //doesn't support fractional seconds then res_adjust returns 0
      //and all the fractional seconds return 0.
      int adjust = resolution_traits_type::res_adjust()/1000000;

	  time_duration_type td(curr->tm_hour,
                            curr->tm_min,
                            curr->tm_sec,
                            sub_sec * adjust);
                            //st.wMilliseconds * adjust);
      return time_type(d,td);
      
    }
#endif // BOOST_HAS_FTIME
  };

  
} } //namespace date_time

#endif //BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK


#endif
