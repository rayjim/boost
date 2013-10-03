// event.hpp, mach events
//
// Copyright (C) 2013 Tim Blechmann
// Copyright (C) 2013 Andrey Semashev
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SYNC_EVENT_DETAIL_DARWIN_EVENT_MACH_HPP
#define BOOST_SYNC_EVENT_DETAIL_DARWIN_EVENT_MACH_HPP

// auto_reset_event is implemented via sync::semaphores:
#include <boost/sync/detail/event/event_autoreset_semaphore.hpp>


#include <cstddef>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/sync/detail/config.hpp>
#include <boost/sync/detail/atomic.hpp>
#include <boost/sync/detail/pause.hpp>
#include <boost/sync/exceptions/resource_error.hpp>

#include <mach/task.h>
#include <mach/semaphore.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>

#include <boost/sync/detail/header.hpp>

namespace boost {

namespace sync {

BOOST_SYNC_DETAIL_OPEN_ABI_NAMESPACE {


class manual_reset_event
{
    BOOST_DELETED_FUNCTION(manual_reset_event(manual_reset_event const&))
    BOOST_DELETED_FUNCTION(manual_reset_event& operator=(manual_reset_event const&));

public:
    manual_reset_event() BOOST_NOEXCEPT :
        m_state(0)
    {
        kern_return_t result = semaphore_create(mach_task_self(), &m_sem, SYNC_POLICY_FIFO, 0);
        BOOST_VERIFY(result == KERN_SUCCESS);
    }

    ~manual_reset_event() BOOST_NOEXCEPT
    {
        kern_return_t result = semaphore_destroy(mach_task_self(), m_sem);
        BOOST_VERIFY(result == KERN_SUCCESS);
    }

    void post() BOOST_NOEXCEPT
    {
        m_state.store( 1, detail::atomic_ns::memory_order_release );
        semaphore_signal_all( m_sem ); // wake all threads!& reset semaphore count
    }

    void reset() BOOST_NOEXCEPT
    {
        m_state = 0;
    }

    void wait() BOOST_NOEXCEPT
    {
        if (m_state.load(detail::atomic_ns::memory_order_acquire) == 1)
            return;

        kern_return_t result = semaphore_wait( m_sem );
        BOOST_VERIFY (result == KERN_SUCCESS);
    }

    bool try_wait() BOOST_NOEXCEPT
    {
        const mach_timespec_t immediate = {0, 0};
        return do_try_wait_until(immediate);
    }

    template <class Rep, class Period>
    bool try_wait_for(const chrono::duration<Rep, Period> & duration) BOOST_NOEXCEPT
    {
        chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(duration);
        chrono::nanoseconds nanoseconds = chrono::duration_cast<chrono::nanoseconds>(duration) - seconds;

        const mach_timespec_t mach_duration = { static_cast<unsigned int>(seconds.count()), static_cast<clock_res_t>(nanoseconds.count()) };
        return do_try_wait_until( mach_duration );
    }

    template <class Clock, class Duration>
    bool try_wait_until(const chrono::time_point<Clock, Duration> & timeout ) BOOST_NOEXCEPT
    {
        return try_wait_for( timeout - Clock::now() );
    }

private:
    bool do_try_wait_until (const mach_timespec_t & timeout)
    {
        if (m_state.load( detail::atomic_ns::memory_order_acquire ) == 1)
            return true;

        kern_return_t result = semaphore_timedwait( m_sem, timeout );
        if (result == KERN_SUCCESS)
            return true;
        else
            return false;
    }

    semaphore_t m_sem;
    detail::atomic_ns::atomic<int32_t> m_state;
};

}
}
}

#include <boost/sync/detail/footer.hpp>

#endif // BOOST_SYNC_EVENT_DETAIL_DARWIN_EVENT_MACH_HPP