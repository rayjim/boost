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

#include <cstddef>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>

#include <boost/sync/detail/config.hpp>
#include <boost/sync/exceptions/resource_error.hpp>

#include <mach/task.h>
#include <mach/semaphore.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>

#include <boost/atomic.hpp>

#include <boost/sync/detail/header.hpp>

namespace boost {

namespace sync {

BOOST_SYNC_DETAIL_OPEN_ABI_NAMESPACE {

class event
{
    BOOST_DELETED_FUNCTION(event(event const&))
    BOOST_DELETED_FUNCTION(event& operator=(event const&));

public:
    explicit event(bool auto_reset = false) BOOST_NOEXCEPT:
        m_auto_reset(auto_reset), m_state(0)
    {
        kern_return_t result = semaphore_create(mach_task_self(), &m_sem, SYNC_POLICY_FIFO, 0);
        BOOST_VERIFY(result == KERN_SUCCESS);
    }

    ~event() BOOST_NOEXCEPT
    {
        kern_return_t result = semaphore_destroy(mach_task_self(), m_sem);
        BOOST_VERIFY(result == KERN_SUCCESS);
    }

    void post() BOOST_NOEXCEPT
    {
        if (m_auto_reset) {
            bool was_signalled;

            uint32_t old_state = m_state.load( memory_order_acquire );
            for(;;) {
                was_signalled = ((old_state & 1u) != 0);
                uint32_t new_state = ((old_state + 2u) | 1u) & std::numeric_limits<uint32_t>::max();
                if ( m_state.compare_exchange_weak( old_state, new_state, memory_order_release, memory_order_acquire ) )
                    break;
                // backoff
            }

            if (!was_signalled)
                semaphore_signal(m_sem); // wake one thread!
        } else {
            m_state.store( 1, memory_order_release );
            semaphore_signal_all( m_sem ); // wake all threads!& reset semaphore count
        }
    }

    void reset() BOOST_NOEXCEPT
    {
        m_state = 0; // CHECK: do we need to increment the ABA tag?
    }

    void wait() BOOST_NOEXCEPT
    {
        if (m_auto_reset) {
            kern_return_t result = semaphore_wait( m_sem );
            BOOST_VERIFY (result == KERN_SUCCESS);

            uint32_t old_state = m_state.load(memory_order_acquire);
            if (!m_state.compare_exchange_strong( old_state, old_state & ~1u, memory_order_release, memory_order_acquire ) )
                semaphore_signal(m_sem); // another post succeeded, wake up another thread

        } else {
            if (m_state.load(memory_order_acquire) == 1)
                return;

            kern_return_t result = semaphore_wait( m_sem );
            BOOST_VERIFY (result == KERN_SUCCESS);
        }
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
        if (m_auto_reset) {
            kern_return_t result = semaphore_timedwait( m_sem, timeout );

            if (result == KERN_SUCCESS) {
                uint32_t old_state = m_state.load(memory_order_acquire);
                if (!m_state.compare_exchange_strong( old_state, old_state & ~1u, memory_order_release, memory_order_acquire ) )
                    semaphore_signal(m_sem); // another post succeeded, wake up another thread
                return true;
            } else
                return false;
        } else {
            if (m_state.load( memory_order_acquire ) == 1)
                return true;

            kern_return_t result = semaphore_timedwait( m_sem, timeout );
            if (result == KERN_SUCCESS)
                return true;
            else
                return false;
        }
    }

    const bool m_auto_reset;
    semaphore_t m_sem;
    atomic<uint32_t> m_state;
};

}
}
}

#include <boost/sync/detail/footer.hpp>

#endif // BOOST_SYNC_EVENT_DETAIL_DARWIN_EVENT_MACH_HPP
