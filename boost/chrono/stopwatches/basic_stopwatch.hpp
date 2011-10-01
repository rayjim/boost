//  boost/chrono/stopwatches/basic_stopwatch.hpp  ------------------------------------------------------------//
//  Copyright 2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org/libs/libs/chrono/stopwatches for documentation.

#ifndef BOOST_CHRONO_STOPWATCHES_BASIC_STOPWATCH__HPP
#define BOOST_CHRONO_STOPWATCHES_BASIC_STOPWATCH__HPP

#include <utility>

#include <boost/chrono/config.hpp>
#include <boost/chrono/stopwatches/stopwatch_scoped.hpp>
#include <boost/chrono/stopwatches/memories/no_memory.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/system/error_code.hpp>

namespace boost
{
  namespace chrono
  {

    /**
     * Type used to don't start a basic_stopwatch at construction time.
     */
    struct dont_start_t
    {
    };

    /**
     * Instance used to don't start a basic_stopwatch at construction time.
     */
    static const dont_start_t dont_start =
    { };

    /**
     * A basic_stopwatch is a model of @c Stopwatch taking as parameters the @c Clock and the @c LapsCollector.
     *
     * The main difference respect to a @c simple_stopwatch is that the user can stop it.
     * Each sequence of start-stop results in a new elapsed duration sample that is provided to the LapsCollector.
     *
     * It is up to the LapsCollector to make whatever wants with each sample.
     * A LapCollector must define a store(duration const&) and a clear() functions.
     *
     * The library provides LapsCollectors that forget the sample, store the
     * last one, cummulates the samples in an accumulator set or store them in a container.
     * For simplicity the default LapCollector is the one that forget the samples.
     *
     * Even if it is preferable to use process or thread wide clocks,
     * the default of the Clock parameter is high_resolution_clock,
     * as it is the single one ensured on all platforms.
     */
    template<typename Clock=high_resolution_clock, typename LapsCollector=no_memory<typename Clock::duration> >
    class basic_stopwatch
    {
    public:
      typedef LapsCollector laps_collector;
      typedef Clock clock;
      typedef typename Clock::duration duration;
      typedef typename Clock::time_point time_point;
      typedef typename Clock::rep rep;
      typedef typename Clock::period period;
      BOOST_CHRONO_STATIC_CONSTEXPR bool is_steady = Clock::is_steady;

      /**
       * Default constructor.
       *
       * Effects: Starts the stopwatch.
       * Post-conditions: is_running().
       */
      explicit basic_stopwatch()
      :
        start_(duration::zero()),
        running_(false),
        laps_collector_()
      {
        start();
      }

      /**
       * Default constructor.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: Starts the stopwatch.
       * Post-conditions: is_running() if no error occur.
       */
      explicit basic_stopwatch(
          system::error_code & ec
          ) :
        start_(duration::zero()),
        running_(false),
        laps_collector_()
      {
        start(ec);
      }

      /**
       * Not starting constructor.
       *
       * Effects: Don't starts the stopwatch.
       * Post-conditions: ! is_running() if no error occur.
       */
      explicit basic_stopwatch(
          const dont_start_t&
          ) :
        start_(duration::zero()),
        running_(false),
        laps_collector_()
      {
      }

      /**
       * Starting constructor from a LapsCollector instance.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: Copies the LapsCollector. Starts the stopwatch.
       * Post-conditions: is_running() if no error occur.
       *
       * Remark: The LapsCollector is copied and owned by the stopwatch.
       */
      explicit basic_stopwatch(
          laps_collector const& acc,
          system::error_code & ec = BOOST_CHRONO_THROWS
          ) :
        start_(duration::zero()),
        running_(false),
        laps_collector_(acc)
      {
        start(ec);
      }

      /**
       * Not starting constructor from a LapsCollector instance.
       *
       * Effects: Copies the LapsCollector. Don't starts the stopwatch.
       * Post-conditions: ! is_running() if no error occur.
       *
       * Remark: The LapsCollector is copied and owned by the stopwatch.
       */
      basic_stopwatch(
          laps_collector const& acc,
          const dont_start_t&
          ) :
        start_(duration::zero()),
        running_(false),
        laps_collector_(acc)
      {
      }

      /**
       * Destructor.
       *
       * Effects: Do nothing.
       */
      ~basic_stopwatch()
      {
      }

      /**
       * Restart the stopwatch.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: As if stop(); start() were called, but ensuring that the start time is the same as the stop time.
       *
       * Post-conditions: is_running() if no error occur.
       */
      void restart()
      {
        time_point tmp = clock::now();

        if (is_running())
        {
          laps_collector_.store(tmp - start_);
        }
        else
        {
          running_ = true;
        }
        start_ = tmp;
      }

      /**
       * Restart the stopwatch.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: As if stop(); start() were called, but ensuring that the start time is the same as the stop time.
       *
       * Post-conditions: is_running() if no error occur.
       */
      void restart(
          system::error_code & ec
          )
      {
        time_point tmp = clock::now(ec);
        if (!BOOST_CHRONO_IS_THROWS(ec) && ec) return;

        if (is_running())
        {
          laps_collector_.store(tmp - start_);
        }
        else
        {
          running_ = true;
        }
        start_ = tmp;
      }

      /**
       * Start the stopwatch.
       *
       * Effects: Memorize the current time.
       *
       * Post-conditions: is_running().
       */
      void start()
      {
          start_ = clock::now();
          running_ = true;
      }

      /**
       * Start the stopwatch.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: Memorize the current time.
       *
       * Post-conditions: is_running() if no error occur.
       */
      void start(
          system::error_code & ec
          )
      {
          time_point tmp = clock::now(ec);
          if (!BOOST_CHRONO_IS_THROWS(ec) && ec) return;

          start_ = tmp;
          running_ = true;
      }


      /**
       * Start the stopwatch.
       *
       * Effects: Gives the elapsed time since start time to the LapCollector.
       *
       * Throws: Any exception that the LapCollector can throw when .
       *
       * Post-conditions: !is_running() if no error occur.
       */
      void stop()
      {
        if (is_running())
        {
          laps_collector_.store(clock::now() - start_);
          start_ = time_point(duration::zero());
          running_ = false;
        }
      }

      /**
       * Start the stopwatch.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       * Effects: Gives the elapsed time since start time to the LapCollector if no internal error occurs.
       *
       * Throws: Any exception that the LapCollector can Throw.
       *
       * Post-conditions: !is_running() if no error occur.
       */
      void stop(
          system::error_code & ec
          )
      {
        if (is_running())
        {
          time_point tmp = clock::now(ec);
          if (!BOOST_CHRONO_IS_THROWS(ec) && ec) return;

          laps_collector_.store(tmp - start_);
          start_ = time_point(duration::zero());
          running_ = false;
        }
      }

      /**
       * States if the Stopwatch is running.
       */
      bool is_running() const {
        return running_;
      }

      /**
       * Elapsed time getter.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       *
       * Returns: the elapsed time since the start if no internal error occur.
       *
       */
      duration elapsed() const
      {
        if (is_running())
        {
          return clock::now() - start_;
        }
        else
        {
          return duration::zero();
        }
      }

      /**
       * Elapsed time getter.
       *
       * Effects: Assign the error code if any internal error occur while retrieving the current time.
       *
       * Returns: the elapsed time since the start if no internal error occur.
       *
       */
      duration elapsed(
          system::error_code & ec
          ) const
      {
        if (is_running())
        {
            time_point tmp = clock::now(ec);
            if (!BOOST_CHRONO_IS_THROWS(ec) && ec) return duration::zero();

            return tmp - start_;
        } else
        {
          return duration::zero();
        }
      }
      /**
       * Resets the stopwatch.
       *
       * Effects: Resets the LapCollector.
       *
       * Post-conditions: !is_running() if no error occur.
       *
       */
      void reset()
      {

        laps_collector_.reset();
        running_ = false;
        start_ = time_point(duration::zero());
      }

      /**
       * LapsCollector getter.
       *
       * Returns: the LapCollector instance.
       *
       */
      laps_collector const& get_laps_collector() BOOST_CHRONO_NOEXCEPT
      {
        return laps_collector_;
      }

      /**
       * Useful typedef for scoped run
       */
      typedef stopwatch_runner<basic_stopwatch<Clock, LapsCollector> >
          scoped_run;
      /**
       * Useful typedef for scoped stop
       */
      typedef stopwatch_stopper<basic_stopwatch<Clock, LapsCollector> >
          scoped_stop;

    private:
      time_point start_;
      bool running_;
      laps_collector laps_collector_;
    };

  } // namespace chrono
} // namespace boost

#endif
