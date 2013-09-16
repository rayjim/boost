// semaphore.hpp
//
// Copyright (C) 2013 Tim Blechmann
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SYNC_SEMAPHORE_HPP_INCLUDED_
#define BOOST_SYNC_SEMAPHORE_HPP_INCLUDED_

#ifdef BOOST_SYNC_DETAIL_DOXYGEN

namespace boost {
namespace sync  {

class semaphore
{
public:
    /**
     * \b Effects: Constructs a semaphore object. The semaphore is initialized to `initial_count`, which is expected to be non-negative.
     *
     * \b Throws: if an error occurs.
     *
     * */
    explicit semaphore(unsigned int initial_count = 0);

    /**
     * \b Precondition: No thread is waiting on the semaphore
     *
     * \b Effects: Destroys the object
     *
     * */
    ~semaphore();

    semaphore(semaphore const&) = delete;
    semaphore& operator= (semaphore const&) = delete;

    /**
     * \b Effects: Increments the semaphore count. If a thread is waiting for this semaphore, it will be unblocked.
     *
     * \b Throws: if an error occurs.
     * */
    void post();

    /**
     * \b Effects: If the semaphore count is positive, it atomically decrements it and returns. Otherwise blocks the current thread, until it can successfully decrement a positive semaphore count.
     *
     * \b Throws: if an error occurs.
     * */
    void wait();

    /**
     * \b Effects: If the semaphore count is positive, it atomically decrements it and returns `true`. Otherwise `false`.
     *
     * \b Throws: if an error occurs.
     * */
    bool try_wait();

    /**
     * \b Effects: If the semaphore count is positive, it atomically decrements it and returns `true`. Otherwise it waits for the semaphore for `duration`.
     *
     * \b Throws: if an error occurs.
     * */
    template <class Duration>
    bool try_wait_for(const Duration & duration);

    /**
     * \b Effects: If the semaphore count is positive, it atomically decrements it and returns `true`. Otherwise it waits for the semaphore until `timeout`.
     *
     * \b Throws: if an error occurs.
     * */
    template <class TimePoint>
    bool try_wait_until(const TimePoint & timeout);
};

}
}

#else // BOOST_SYNC_DETAIL_DOXYGEN

#include <boost/sync/detail/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#ifdef BOOST_HAS_UNISTD_H
#include <unistd.h>

#if (_POSIX_SEMAPHORES - 0) >= 200112L
#define BOOST_SYNC_DETAIL_USE_POSIX_SEMAPHORES
#endif

#endif // BOOST_HAS_UNISTD_H

#if !defined(BOOST_SYNC_DETAIL_USE_POSIX_SEMAPHORES) && (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__))
#include <Availability.h>

// OSX
#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED

#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_6
#define BOOST_SYNC_DETAIL_USE_DISPATCH_SEMAPHORES
#endif

#endif // __MAC_OS_X_VERSION_MIN_REQUIRED

// iOS
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED

// untested!
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
#define BOOST_SYNC_DETAIL_USE_DISPATCH_SEMAPHORES
#endif

#endif // __IPHONE_OS_VERSION_MIN_REQUIRED

#endif // !defined(BOOST_SYNC_DETAIL_USE_POSIX_SEMAPHORES) && defined(__APPLE__)


#if defined(BOOST_SYNC_DETAIL_PLATFORM_WINAPI)
#include <boost/sync/detail/windows/semaphore.hpp>

#elif defined(BOOST_SYNC_DETAIL_USE_POSIX_SEMAPHORES)
#include <boost/sync/detail/posix/semaphore.hpp>

#elif defined(BOOST_SYNC_DETAIL_USE_DISPATCH_SEMAPHORES)
#include <boost/sync/detail/darwin/semaphore.hpp>

#else
#include <boost/sync/detail/generic/semaphore.hpp>

#endif

#endif // BOOST_SYNC_DETAIL_DOXYGEN

#endif // BOOST_SYNC_SEMAPHORE_HPP_INCLUDED_
