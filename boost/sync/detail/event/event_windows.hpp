// event.hpp, win32 events
//
// Copyright (C) 2013 Tim Blechmann
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SYNC_DETAIL_EVENT_EVENT_WINDOWS_HPP_INCLUDED_
#define BOOST_SYNC_DETAIL_EVENT_EVENT_WINDOWS_HPP_INCLUDED_

#include <cstddef>
#include <boost/assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/detail/winapi/GetLastError.hpp>
#include <boost/detail/winapi/synchronization.hpp>
#include <boost/detail/winapi/handles.hpp>

#include <boost/sync/detail/config.hpp>
#include <boost/sync/exceptions/resource_error.hpp>
#include <boost/sync/detail/header.hpp>

namespace boost {

namespace sync {

BOOST_SYNC_DETAIL_OPEN_ABI_NAMESPACE {

class event
{
    BOOST_DELETED_FUNCTION(event(event const&))
    BOOST_DELETED_FUNCTION(event& operator=(event const&))

    typedef boost::detail::winapi::HANDLE_ HANDLE_;
    typedef boost::detail::winapi::BOOL_   BOOL_;
    typedef boost::detail::winapi::DWORD_  DWORD_;

public:
    explicit event(bool auto_reset = false)
    {
        handle_ = boost::detail::winapi::CreateEventA(NULL, !auto_reset, 0, NULL);
        if (!handle_)
        {
            const DWORD_ err = boost::detail::winapi::GetLastError();
            BOOST_THROW_EXCEPTION(resource_error(err, "boost::sync::event constructor failed in CreateEvent"));
        }
    }

    ~event() BOOST_NOEXCEPT
    {
        BOOST_VERIFY( boost::detail::winapi::CloseHandle(handle_) );
    }

    void post()
    {
        const BOOL_ status = boost::detail::winapi::SetEvent(handle_);
        if (status == 0)
        {
            const DWORD_ err = boost::detail::winapi::GetLastError();
            BOOST_THROW_EXCEPTION(resource_error(err, "boost::sync::event::post failed in ReleaseEvent"));
        }
    }

    void reset()
    {
        const BOOL_ status = boost::detail::winapi::ResetEvent(handle_);
        if (status == 0)
        {
            const DWORD_ err = boost::detail::winapi::GetLastError();
            BOOST_THROW_EXCEPTION(resource_error(err, "boost::sync::event::reset failed in ResetEvent"));
        }
    }

    bool wait()
    {
        using namespace boost::detail::winapi;

        switch ( WaitForSingleObject(handle_, infinite) )
        {
        case wait_object_0:
            return true;

        case wait_failed:
            {
                const DWORD_ err = boost::detail::winapi::GetLastError();
                BOOST_THROW_EXCEPTION(resource_error(err, "boost::sync::event::wait failed in WaitForSingleObject"));
            }

        default:
            BOOST_ASSERT(false);
            return false;
        }
    }

    bool try_wait()
    {
        return do_try_wait_for( 0 );
    }

    template <class Rep, class Period>
    bool try_wait_for(const chrono::duration<Rep, Period> & rel_time)
    {
        return do_try_wait_for(static_cast< DWORD_ >(chrono::duration_cast<chrono::milliseconds>( rel_time ).count()));
    }

    template <class Clock, class Duration>
    bool try_wait_until(const chrono::time_point<Clock, Duration> & timeout )
    {
        typename Clock::time_point  c_now = Clock::now();
        return try_wait_for( timeout - c_now );
    }

private:
    bool do_try_wait_for( DWORD_ milliseconds )
    {
        using namespace boost::detail::winapi;

        switch ( WaitForSingleObject(handle_, milliseconds) )
        {
        case wait_object_0:
            return true;

        case wait_timeout:
            return false;

        case wait_failed:
            {
                const DWORD_ err = boost::detail::winapi::GetLastError();
                BOOST_THROW_EXCEPTION(resource_error(err, "boost::sync::event::do_try_wait_for failed in WaitForSingleObject"));
            }

        default:
            BOOST_ASSERT(false);
            return false;
        }
    }

    HANDLE_ handle_;
};

}
}
}

#include <boost/sync/detail/footer.hpp>

#endif // BOOST_SYNC_DETAIL_EVENT_EVENT_WINDOWS_HPP_INCLUDED_