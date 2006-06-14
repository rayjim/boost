//
// basic_resolver_entry.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2006 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_IP_BASIC_RESOLVER_ENTRY_HPP
#define BOOST_ASIO_IP_BASIC_RESOLVER_ENTRY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/detail/push_options.hpp>
#include <string>
#include <boost/asio/detail/pop_options.hpp>

namespace boost {
namespace asio {
namespace ip {

/// An entry produced by a resolver.
/**
 * The boost::asio::ip::basic_resolver_entry class template describes an entry
 * as returned by a resolver.
 *
 * @par Thread Safety:
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 *
 * @par Concepts:
 * Endpoint.
 */
template <typename Protocol>
class basic_resolver_entry
{
public:
  /// The protocol type associated with the endpoint entry.
  typedef Protocol protocol_type;

  /// The endpoint type associated with the endpoint entry.
  typedef typename Protocol::endpoint endpoint_type;

  /// Default constructor.
  basic_resolver_entry()
  {
  }

  /// Construct with specified endpoint, host name and service name.
  basic_resolver_entry(const endpoint_type& endpoint,
      const std::string& host_name, const std::string& service_name)
    : endpoint_(endpoint),
      host_name_(host_name),
      service_name_(service_name)
  {
  }

  /// Get the endpoint associated with the entry.
  endpoint_type endpoint() const
  {
    return endpoint_;
  }

  /// Convert to the endpoint associated with the entry.
  operator endpoint_type() const
  {
    return endpoint_;
  }

  /// Get the host name associated with the entry.
  std::string host_name() const
  {
    return host_name_;
  }

  /// Get the service name associated with the entry.
  std::string service_name() const
  {
    return service_name_;
  }

private:
  endpoint_type endpoint_;
  std::string host_name_;
  std::string service_name_;
};

} // namespace ip
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_IP_BASIC_RESOLVER_ENTRY_HPP
