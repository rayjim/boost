//
// detail/impl/posix_event.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_IMPL_POSIX_EVENT_IPP
#define BOOST_ASIO_DETAIL_IMPL_POSIX_EVENT_IPP

#if defined(_MSC_VER)
# pragma once
#endif // defined(_MSC_VER)

#include <boost/asio/detail/config.hpp>

#if defined(BOOST_ASIO_HAS_PTHREADS)

#include <boost/asio/detail/posix_event.hpp>
#include <boost/asio/detail/throw_error.hpp>
#include <boost/asio/error.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace detail {

posix_event::posix_event()
  : signalled_(false)
{
  int error = ::pthread_cond_init(&cond_, 0);
  boost::system::error_code ec(error,
      boost::asio::error::get_system_category());
  boost::asio::detail::throw_error(ec, "event");
}

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_ASIO_HAS_PTHREADS)

#endif // BOOST_ASIO_DETAIL_IMPL_POSIX_EVENT_IPP
