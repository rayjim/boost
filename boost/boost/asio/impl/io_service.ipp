//
// io_service.ipp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2006 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_IO_SERVICE_IPP
#define BOOST_ASIO_IO_SERVICE_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/detail/epoll_reactor.hpp>
#include <boost/asio/detail/kqueue_reactor.hpp>
#include <boost/asio/detail/select_reactor.hpp>
#include <boost/asio/detail/task_io_service.hpp>
#include <boost/asio/detail/win_iocp_io_service.hpp>

namespace boost {
namespace asio {

inline io_service::io_service()
  : service_registry_(*this),
    impl_(service_registry_.use_service<impl_type>())
{
}

inline size_t io_service::run()
{
  return impl_.run();
}

inline size_t io_service::run_one()
{
  return impl_.run_one();
}

inline size_t io_service::poll()
{
  return impl_.poll();
}

inline size_t io_service::poll_one()
{
  return impl_.poll_one();
}

inline void io_service::interrupt()
{
  impl_.interrupt();
}

inline void io_service::reset()
{
  impl_.reset();
}

template <typename Handler>
inline void io_service::dispatch(Handler handler)
{
  impl_.dispatch(handler);
}

template <typename Handler>
inline void io_service::post(Handler handler)
{
  impl_.post(handler);
}

template <typename Handler>
#if defined(GENERATING_DOCUMENTATION)
unspecified
#else
inline detail::wrapped_handler<io_service, Handler>
#endif
io_service::wrap(Handler handler)
{
  return detail::wrapped_handler<io_service, Handler>(*this, handler);
}

inline io_service::work::work(boost::asio::io_service& io_service)
  : io_service_(io_service)
{
  io_service_.impl_.work_started();
}

inline io_service::work::work(const work& other)
  : io_service_(other.io_service_)
{
  io_service_.impl_.work_started();
}

inline io_service::work::~work()
{
  io_service_.impl_.work_finished();
}

inline boost::asio::io_service& io_service::work::io_service()
{
  return io_service_;
}

inline io_service::service::service(boost::asio::io_service& owner)
  : owner_(owner),
    type_info_(0),
    next_(0)
{
}

inline io_service::service::~service()
{
}

inline boost::asio::io_service& io_service::service::io_service()
{
  return owner_;
}

template <typename Service>
inline Service& use_service(io_service& ios)
{
  return ios.service_registry_.template use_service<Service>();
}

template <typename Service>
void add_service(io_service& ios, Service* svc)
{
  if (&ios != &svc->io_service())
    boost::throw_exception(invalid_service_owner());
  if (!ios.service_registry_.template add_service<Service>(svc))
    boost::throw_exception(service_already_exists());
}

template <typename Service>
bool has_service(io_service& ios)
{
  return ios.service_registry_.template has_service<Service>();
}

} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_IO_SERVICE_IPP
