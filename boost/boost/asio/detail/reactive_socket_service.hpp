//
// reactive_socket_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2006 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_REACTIVE_SOCKET_SERVICE_HPP
#define BOOST_ASIO_DETAIL_REACTIVE_SOCKET_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/detail/push_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/detail/pop_options.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <boost/asio/detail/noncopyable.hpp>
#include <boost/asio/detail/socket_holder.hpp>
#include <boost/asio/detail/socket_ops.hpp>
#include <boost/asio/detail/socket_types.hpp>

namespace boost {
namespace asio {
namespace detail {

template <typename Protocol, typename Reactor>
class reactive_socket_service
  : public boost::asio::io_service::service
{
public:
  // The protocol type.
  typedef Protocol protocol_type;

  // The endpoint type.
  typedef typename Protocol::endpoint endpoint_type;

  // The native type of a socket.
  typedef socket_type native_type;

  // The implementation type of the socket.
  class implementation_type
    : private boost::asio::detail::noncopyable
  {
  public:
    // Default constructor.
    implementation_type()
      : socket_(invalid_socket),
        flags_(0),
        protocol_(endpoint_type().protocol())
    {
    }

  private:
    // Only this service will have access to the internal values.
    friend class reactive_socket_service<Protocol, Reactor>;

    // The native socket representation.
    socket_type socket_;

    enum
    {
      user_set_non_blocking = 1, // The user wants a non-blocking socket.
      internal_non_blocking = 2, // The socket has been set non-blocking.
      enable_connection_aborted = 4, // User wants connection_aborted errors.
      user_set_linger = 8 // The user set the linger option.
    };

    // Flags indicating the current state of the socket.
    unsigned char flags_;

    // The protocol associated with the socket.
    protocol_type protocol_;
  };

  // The maximum number of buffers to support in a single operation.
  enum { max_buffers = 16 };

  // Constructor.
  reactive_socket_service(boost::asio::io_service& io_service)
    : boost::asio::io_service::service(io_service),
      reactor_(boost::asio::use_service<Reactor>(io_service))
  {
  }

  // Destroy all user-defined handler objects owned by the service.
  void shutdown_service()
  {
  }

  // Construct a new socket implementation.
  void construct(implementation_type& impl)
  {
    impl.socket_ = invalid_socket;
    impl.flags_ = 0;
  }

  // Destroy a socket implementation.
  void destroy(implementation_type& impl)
  {
    if (impl.socket_ != invalid_socket)
    {
      reactor_.close_descriptor(impl.socket_);

      if (impl.flags_ & implementation_type::internal_non_blocking)
      {
        ioctl_arg_type non_blocking = 0;
        boost::system::error_code ignored_ec;
        socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ignored_ec);
        impl.flags_ &= ~implementation_type::internal_non_blocking;
      }

      if (impl.flags_ & implementation_type::user_set_linger)
      {
        ::linger opt;
        opt.l_onoff = 0;
        opt.l_linger = 0;
        boost::system::error_code ignored_ec;
        socket_ops::setsockopt(impl.socket_,
            SOL_SOCKET, SO_LINGER, &opt, sizeof(opt), ignored_ec);
      }

      boost::system::error_code ignored_ec;
      socket_ops::close(impl.socket_, ignored_ec);

      impl.socket_ = invalid_socket;
    }
  }

  // Open a new socket implementation.
  boost::system::error_code open(implementation_type& impl,
      const protocol_type& protocol, boost::system::error_code& ec)
  {
    boost::system::error_code ignored_ec;
    close(impl, ignored_ec);

    socket_holder sock(socket_ops::socket(protocol.family(),
          protocol.type(), protocol.protocol(), ec));
    if (sock.get() == invalid_socket)
      return ec;

    if (int err = reactor_.register_descriptor(sock.get()))
    {
      ec = boost::system::error_code(err, boost::system::native_ecat);
      return ec;
    }

    impl.socket_ = sock.release();
    impl.flags_ = 0;
    impl.protocol_ = protocol;
    ec = boost::system::error_code();
    return ec;
  }

  // Assign a native socket to a socket implementation.
  boost::system::error_code assign(implementation_type& impl,
      const protocol_type& protocol, const native_type& native_socket,
      boost::system::error_code& ec)
  {
    boost::system::error_code ignored_ec;
    close(impl, ignored_ec);

    if (int err = reactor_.register_descriptor(native_socket))
    {
      ec = boost::system::error_code(err, boost::system::native_ecat);
      return ec;
    }

    impl.socket_ = native_socket;
    impl.flags_ = 0;
    impl.protocol_ = protocol;
    ec = boost::system::error_code();
    return ec;
  }

  // Destroy a socket implementation.
  boost::system::error_code close(implementation_type& impl,
      boost::system::error_code& ec)
  {
    if (impl.socket_ != invalid_socket)
    {
      reactor_.close_descriptor(impl.socket_);

      if (impl.flags_ & implementation_type::internal_non_blocking)
      {
        ioctl_arg_type non_blocking = 0;
        boost::system::error_code ignored_ec;
        socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ignored_ec);
        impl.flags_ &= ~implementation_type::internal_non_blocking;
      }

      if (socket_ops::close(impl.socket_, ec) == socket_error_retval)
        return ec;

      impl.socket_ = invalid_socket;
    }

    ec = boost::system::error_code();
    return ec;
  }

  // Get the native socket representation.
  native_type native(implementation_type& impl)
  {
    return impl.socket_;
  }

  // Cancel all operations associated with the socket.
  boost::system::error_code cancel(implementation_type& impl,
      boost::system::error_code& ec)
  {
    if (impl.socket_ == invalid_socket)
    {
      ec = boost::asio::error::bad_descriptor;
      return ec;
    }
    else
    {
      reactor_.cancel_ops(impl.socket_);
      ec = boost::system::error_code();
      return ec;
    }
  }

  // Bind the socket to the specified local endpoint.
  boost::system::error_code bind(implementation_type& impl,
      const endpoint_type& endpoint, boost::system::error_code& ec)
  {
    socket_ops::bind(impl.socket_, endpoint.data(), endpoint.size(), ec);
    return ec;
  }

  // Place the socket into the state where it will listen for new connections.
  boost::system::error_code listen(implementation_type& impl, int backlog,
      boost::system::error_code& ec)
  {
    socket_ops::listen(impl.socket_, backlog, ec);
    return ec;
  }

  // Set a socket option.
  template <typename Option>
  boost::system::error_code set_option(implementation_type& impl,
      const Option& option, boost::system::error_code& ec)
  {
    if (option.level(impl.protocol_) == custom_socket_option_level
        && option.name(impl.protocol_) == enable_connection_aborted_option)
    {
      if (option.size(impl.protocol_) != sizeof(int))
      {
        ec = boost::asio::error::invalid_argument;
      }
      else
      {
        if (*reinterpret_cast<const int*>(option.data(impl.protocol_)))
          impl.flags_ |= implementation_type::enable_connection_aborted;
        else
          impl.flags_ &= ~implementation_type::enable_connection_aborted;
        ec = boost::system::error_code();
      }
      return ec;
    }
    else
    {
      if (option.level(impl.protocol_) == SOL_SOCKET
          && option.name(impl.protocol_) == SO_LINGER)
      {
        impl.flags_ |= implementation_type::user_set_linger;
      }

      socket_ops::setsockopt(impl.socket_,
          option.level(impl.protocol_), option.name(impl.protocol_),
          option.data(impl.protocol_), option.size(impl.protocol_), ec);
      return ec;
    }
  }

  // Set a socket option.
  template <typename Option>
  boost::system::error_code get_option(const implementation_type& impl,
      Option& option, boost::system::error_code& ec) const
  {
    if (option.level(impl.protocol_) == custom_socket_option_level
        && option.name(impl.protocol_) == enable_connection_aborted_option)
    {
      if (option.size(impl.protocol_) != sizeof(int))
      {
        ec = boost::asio::error::invalid_argument;
      }
      else
      {
        int* target = reinterpret_cast<int*>(option.data(impl.protocol_));
        if (impl.flags_ & implementation_type::enable_connection_aborted)
          *target = 1;
        else
          *target = 0;
        ec = boost::system::error_code();
      }
      return ec;
    }
    else
    {
      size_t size = option.size(impl.protocol_);
      socket_ops::getsockopt(impl.socket_,
          option.level(impl.protocol_), option.name(impl.protocol_),
          option.data(impl.protocol_), &size, ec);
      return ec;
    }
  }

  // Perform an IO control command on the socket.
  template <typename IO_Control_Command>
  boost::system::error_code io_control(implementation_type& impl,
      IO_Control_Command& command, boost::system::error_code& ec)
  {
    if (command.name() == static_cast<int>(FIONBIO))
    {
      if (command.get())
        impl.flags_ |= implementation_type::user_set_non_blocking;
      else
        impl.flags_ &= ~implementation_type::user_set_non_blocking;
      ec = boost::system::error_code();
    }
    else
    {
      socket_ops::ioctl(impl.socket_, command.name(),
          static_cast<ioctl_arg_type*>(command.data()), ec);
    }
    return ec;
  }

  // Get the local endpoint.
  endpoint_type local_endpoint(const implementation_type& impl,
      boost::system::error_code& ec) const
  {
    endpoint_type endpoint;
    socket_addr_len_type addr_len = endpoint.capacity();
    if (socket_ops::getsockname(impl.socket_, endpoint.data(), &addr_len, ec))
      return endpoint_type();
    endpoint.resize(addr_len);
    return endpoint;
  }

  // Get the remote endpoint.
  endpoint_type remote_endpoint(const implementation_type& impl,
      boost::system::error_code& ec) const
  {
    endpoint_type endpoint;
    socket_addr_len_type addr_len = endpoint.capacity();
    if (socket_ops::getpeername(impl.socket_, endpoint.data(), &addr_len, ec))
      return endpoint_type();
    endpoint.resize(addr_len);
    return endpoint;
  }

  /// Disable sends or receives on the socket.
  boost::system::error_code shutdown(implementation_type& impl,
      socket_base::shutdown_type what, boost::system::error_code& ec)
  {
    socket_ops::shutdown(impl.socket_, what, ec);
    return ec;
  }

  // Send the given data to the peer.
  template <typename Const_Buffers>
  size_t send(implementation_type& impl, const Const_Buffers& buffers,
      socket_base::message_flags flags, boost::system::error_code& ec)
  {
    // Copy buffers into array.
    socket_ops::buf bufs[max_buffers];
    typename Const_Buffers::const_iterator iter = buffers.begin();
    typename Const_Buffers::const_iterator end = buffers.end();
    size_t i = 0;
    size_t total_buffer_size = 0;
    for (; iter != end && i < max_buffers; ++iter, ++i)
    {
      boost::asio::const_buffer buffer(*iter);
      socket_ops::init_buf(bufs[i],
          boost::asio::buffer_cast<const void*>(buffer),
          boost::asio::buffer_size(buffer));
      total_buffer_size += boost::asio::buffer_size(buffer);
    }

    // A request to receive 0 bytes on a stream socket is a no-op.
    if (impl.protocol_.type() == SOCK_STREAM && total_buffer_size == 0)
    {
      ec = boost::system::error_code();
      return 0;
    }

    // Send the data.
    for (;;)
    {
      // Try to complete the operation without blocking.
      int bytes_sent = socket_ops::send(impl.socket_, bufs, i, flags, ec);

      // Check if operation succeeded.
      if (bytes_sent >= 0)
        return bytes_sent;

      // Operation failed.
      if ((impl.flags_ & implementation_type::user_set_non_blocking)
          || (ec != boost::asio::error::would_block
            && ec != boost::asio::error::try_again))
        return 0;

      // Wait for socket to become ready.
      if (socket_ops::poll_write(impl.socket_, ec) < 0)
        return 0;
    }
  }

  template <typename Const_Buffers, typename Handler>
  class send_handler
  {
  public:
    send_handler(socket_type socket, boost::asio::io_service& io_service,
        const Const_Buffers& buffers, socket_base::message_flags flags,
        Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        buffers_(buffers),
        flags_(flags),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result, 0));
        return true;
      }

      // Copy buffers into array.
      socket_ops::buf bufs[max_buffers];
      typename Const_Buffers::const_iterator iter = buffers_.begin();
      typename Const_Buffers::const_iterator end = buffers_.end();
      size_t i = 0;
      for (; iter != end && i < max_buffers; ++iter, ++i)
      {
        boost::asio::const_buffer buffer(*iter);
        socket_ops::init_buf(bufs[i],
            boost::asio::buffer_cast<const void*>(buffer),
            boost::asio::buffer_size(buffer));
      }

      // Send the data.
      boost::system::error_code ec;
      int bytes = socket_ops::send(socket_, bufs, i, flags_, ec);

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;

      io_service_.post(bind_handler(handler_, ec, bytes < 0 ? 0 : bytes));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Const_Buffers buffers_;
    socket_base::message_flags flags_;
    Handler handler_;
  };

  // Start an asynchronous send. The data being sent must be valid for the
  // lifetime of the asynchronous operation.
  template <typename Const_Buffers, typename Handler>
  void async_send(implementation_type& impl, const Const_Buffers& buffers,
      socket_base::message_flags flags, Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor, 0));
    }
    else
    {
      if (impl.protocol_.type() == SOCK_STREAM)
      {
        // Determine total size of buffers.
        typename Const_Buffers::const_iterator iter = buffers.begin();
        typename Const_Buffers::const_iterator end = buffers.end();
        size_t i = 0;
        size_t total_buffer_size = 0;
        for (; iter != end && i < max_buffers; ++iter, ++i)
        {
          boost::asio::const_buffer buffer(*iter);
          total_buffer_size += boost::asio::buffer_size(buffer);
        }

        // A request to receive 0 bytes on a stream socket is a no-op.
        if (total_buffer_size == 0)
        {
          io_service().post(bind_handler(handler,
                boost::system::error_code(), 0));
          return;
        }
      }

      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec, 0));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      reactor_.start_write_op(impl.socket_,
          send_handler<Const_Buffers, Handler>(
            impl.socket_, io_service(), buffers, flags, handler));
    }
  }

  // Send a datagram to the specified endpoint. Returns the number of bytes
  // sent.
  template <typename Const_Buffers>
  size_t send_to(implementation_type& impl, const Const_Buffers& buffers,
      const endpoint_type& destination, socket_base::message_flags flags,
      boost::system::error_code& ec)
  {
    // Copy buffers into array.
    socket_ops::buf bufs[max_buffers];
    typename Const_Buffers::const_iterator iter = buffers.begin();
    typename Const_Buffers::const_iterator end = buffers.end();
    size_t i = 0;
    for (; iter != end && i < max_buffers; ++iter, ++i)
    {
      boost::asio::const_buffer buffer(*iter);
      socket_ops::init_buf(bufs[i],
          boost::asio::buffer_cast<const void*>(buffer),
          boost::asio::buffer_size(buffer));
    }

    // Send the data.
    for (;;)
    {
      // Try to complete the operation without blocking.
      int bytes_sent = socket_ops::sendto(impl.socket_, bufs, i, flags,
          destination.data(), destination.size(), ec);

      // Check if operation succeeded.
      if (bytes_sent >= 0)
        return bytes_sent;

      // Operation failed.
      if ((impl.flags_ & implementation_type::user_set_non_blocking)
          || (ec != boost::asio::error::would_block
            && ec != boost::asio::error::try_again))
        return 0;

      // Wait for socket to become ready.
      if (socket_ops::poll_write(impl.socket_, ec) < 0)
        return 0;
    }
  }

  template <typename Const_Buffers, typename Handler>
  class send_to_handler
  {
  public:
    send_to_handler(socket_type socket, boost::asio::io_service& io_service,
        const Const_Buffers& buffers, const endpoint_type& endpoint,
        socket_base::message_flags flags, Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        buffers_(buffers),
        destination_(endpoint),
        flags_(flags),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result, 0));
        return true;
      }

      // Copy buffers into array.
      socket_ops::buf bufs[max_buffers];
      typename Const_Buffers::const_iterator iter = buffers_.begin();
      typename Const_Buffers::const_iterator end = buffers_.end();
      size_t i = 0;
      for (; iter != end && i < max_buffers; ++iter, ++i)
      {
        boost::asio::const_buffer buffer(*iter);
        socket_ops::init_buf(bufs[i],
            boost::asio::buffer_cast<const void*>(buffer),
            boost::asio::buffer_size(buffer));
      }

      // Send the data.
      boost::system::error_code ec;
      int bytes = socket_ops::sendto(socket_, bufs, i, flags_,
          destination_.data(), destination_.size(), ec);

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;

      io_service_.post(bind_handler(handler_, ec, bytes < 0 ? 0 : bytes));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Const_Buffers buffers_;
    endpoint_type destination_;
    socket_base::message_flags flags_;
    Handler handler_;
  };

  // Start an asynchronous send. The data being sent must be valid for the
  // lifetime of the asynchronous operation.
  template <typename Const_Buffers, typename Handler>
  void async_send_to(implementation_type& impl, const Const_Buffers& buffers,
      const endpoint_type& destination, socket_base::message_flags flags,
      Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor, 0));
    }
    else
    {
      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec, 0));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      reactor_.start_write_op(impl.socket_,
          send_to_handler<Const_Buffers, Handler>(
            impl.socket_, io_service(), buffers, destination, flags, handler));
    }
  }

  // Receive some data from the peer. Returns the number of bytes received.
  template <typename Mutable_Buffers>
  size_t receive(implementation_type& impl, const Mutable_Buffers& buffers,
      socket_base::message_flags flags, boost::system::error_code& ec)
  {
    // Copy buffers into array.
    socket_ops::buf bufs[max_buffers];
    typename Mutable_Buffers::const_iterator iter = buffers.begin();
    typename Mutable_Buffers::const_iterator end = buffers.end();
    size_t i = 0;
    size_t total_buffer_size = 0;
    for (; iter != end && i < max_buffers; ++iter, ++i)
    {
      boost::asio::mutable_buffer buffer(*iter);
      socket_ops::init_buf(bufs[i],
          boost::asio::buffer_cast<void*>(buffer),
          boost::asio::buffer_size(buffer));
      total_buffer_size += boost::asio::buffer_size(buffer);
    }

    // A request to receive 0 bytes on a stream socket is a no-op.
    if (impl.protocol_.type() == SOCK_STREAM && total_buffer_size == 0)
    {
      ec = boost::system::error_code();
      return 0;
    }

    // Receive some data.
    for (;;)
    {
      // Try to complete the operation without blocking.
      int bytes_recvd = socket_ops::recv(impl.socket_, bufs, i, flags, ec);

      // Check if operation succeeded.
      if (bytes_recvd > 0)
        return bytes_recvd;

      // Check for EOF.
      if (bytes_recvd == 0)
      {
        ec = boost::asio::error::eof;
        return 0;
      }

      // Operation failed.
      if ((impl.flags_ & implementation_type::user_set_non_blocking)
          || (ec != boost::asio::error::would_block
            && ec != boost::asio::error::try_again))
        return 0;

      // Wait for socket to become ready.
      if (socket_ops::poll_read(impl.socket_, ec) < 0)
        return 0;
    }
  }

  template <typename Mutable_Buffers, typename Handler>
  class receive_handler
  {
  public:
    receive_handler(socket_type socket, boost::asio::io_service& io_service,
        const Mutable_Buffers& buffers, socket_base::message_flags flags,
        Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        buffers_(buffers),
        flags_(flags),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result, 0));
        return true;
      }

      // Copy buffers into array.
      socket_ops::buf bufs[max_buffers];
      typename Mutable_Buffers::const_iterator iter = buffers_.begin();
      typename Mutable_Buffers::const_iterator end = buffers_.end();
      size_t i = 0;
      for (; iter != end && i < max_buffers; ++iter, ++i)
      {
        boost::asio::mutable_buffer buffer(*iter);
        socket_ops::init_buf(bufs[i],
            boost::asio::buffer_cast<void*>(buffer),
            boost::asio::buffer_size(buffer));
      }

      // Receive some data.
      boost::system::error_code ec;
      int bytes = socket_ops::recv(socket_, bufs, i, flags_, ec);
      if (bytes == 0)
        ec = boost::asio::error::eof;

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;

      io_service_.post(bind_handler(handler_, ec, bytes < 0 ? 0 : bytes));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Mutable_Buffers buffers_;
    socket_base::message_flags flags_;
    Handler handler_;
  };

  // Start an asynchronous receive. The buffer for the data being received
  // must be valid for the lifetime of the asynchronous operation.
  template <typename Mutable_Buffers, typename Handler>
  void async_receive(implementation_type& impl, const Mutable_Buffers& buffers,
      socket_base::message_flags flags, Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor, 0));
    }
    else
    {
      if (impl.protocol_.type() == SOCK_STREAM)
      {
        // Determine total size of buffers.
        typename Mutable_Buffers::const_iterator iter = buffers.begin();
        typename Mutable_Buffers::const_iterator end = buffers.end();
        size_t i = 0;
        size_t total_buffer_size = 0;
        for (; iter != end && i < max_buffers; ++iter, ++i)
        {
          boost::asio::mutable_buffer buffer(*iter);
          total_buffer_size += boost::asio::buffer_size(buffer);
        }

        // A request to receive 0 bytes on a stream socket is a no-op.
        if (total_buffer_size == 0)
        {
          io_service().post(bind_handler(handler,
                boost::system::error_code(), 0));
          return;
        }
      }

      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec, 0));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      if (flags & socket_base::message_out_of_band)
      {
        reactor_.start_except_op(impl.socket_,
            receive_handler<Mutable_Buffers, Handler>(
              impl.socket_, io_service(), buffers, flags, handler));
      }
      else
      {
        reactor_.start_read_op(impl.socket_,
            receive_handler<Mutable_Buffers, Handler>(
              impl.socket_, io_service(), buffers, flags, handler));
      }
    }
  }

  // Receive a datagram with the endpoint of the sender. Returns the number of
  // bytes received.
  template <typename Mutable_Buffers>
  size_t receive_from(implementation_type& impl, const Mutable_Buffers& buffers,
      endpoint_type& sender_endpoint, socket_base::message_flags flags,
      boost::system::error_code& ec)
  {
    // Copy buffers into array.
    socket_ops::buf bufs[max_buffers];
    typename Mutable_Buffers::const_iterator iter = buffers.begin();
    typename Mutable_Buffers::const_iterator end = buffers.end();
    size_t i = 0;
    for (; iter != end && i < max_buffers; ++iter, ++i)
    {
      boost::asio::mutable_buffer buffer(*iter);
      socket_ops::init_buf(bufs[i],
          boost::asio::buffer_cast<void*>(buffer),
          boost::asio::buffer_size(buffer));
    }

    // Receive some data.
    for (;;)
    {
      // Try to complete the operation without blocking.
      socket_addr_len_type addr_len = sender_endpoint.capacity();
      int bytes_recvd = socket_ops::recvfrom(impl.socket_, bufs, i, flags,
          sender_endpoint.data(), &addr_len, ec);

      // Check if operation succeeded.
      if (bytes_recvd > 0)
      {
        sender_endpoint.resize(addr_len);
        return bytes_recvd;
      }

      // Check for EOF.
      if (bytes_recvd == 0)
      {
        ec = boost::asio::error::eof;
        return 0;
      }

      // Operation failed.
      if ((impl.flags_ & implementation_type::user_set_non_blocking)
          || (ec != boost::asio::error::would_block
            && ec != boost::asio::error::try_again))
        return 0;

      // Wait for socket to become ready.
      if (socket_ops::poll_read(impl.socket_, ec) < 0)
        return 0;
    }
  }

  template <typename Mutable_Buffers, typename Handler>
  class receive_from_handler
  {
  public:
    receive_from_handler(socket_type socket,
        boost::asio::io_service& io_service, const Mutable_Buffers& buffers,
        endpoint_type& endpoint, socket_base::message_flags flags,
        Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        buffers_(buffers),
        sender_endpoint_(endpoint),
        flags_(flags),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result != 0)
      {
        io_service_.post(bind_handler(handler_, result, 0));
        return true;
      }

      // Copy buffers into array.
      socket_ops::buf bufs[max_buffers];
      typename Mutable_Buffers::const_iterator iter = buffers_.begin();
      typename Mutable_Buffers::const_iterator end = buffers_.end();
      size_t i = 0;
      for (; iter != end && i < max_buffers; ++iter, ++i)
      {
        boost::asio::mutable_buffer buffer(*iter);
        socket_ops::init_buf(bufs[i],
            boost::asio::buffer_cast<void*>(buffer),
            boost::asio::buffer_size(buffer));
      }

      // Receive some data.
      socket_addr_len_type addr_len = sender_endpoint_.capacity();
      boost::system::error_code ec;
      int bytes = socket_ops::recvfrom(socket_, bufs, i, flags_,
          sender_endpoint_.data(), &addr_len, ec);
      if (bytes == 0)
        ec = boost::asio::error::eof;

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;

      sender_endpoint_.resize(addr_len);
      io_service_.post(bind_handler(handler_, ec, bytes < 0 ? 0 : bytes));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Mutable_Buffers buffers_;
    endpoint_type& sender_endpoint_;
    socket_base::message_flags flags_;
    Handler handler_;
  };

  // Start an asynchronous receive. The buffer for the data being received and
  // the sender_endpoint object must both be valid for the lifetime of the
  // asynchronous operation.
  template <typename Mutable_Buffers, typename Handler>
  void async_receive_from(implementation_type& impl,
      const Mutable_Buffers& buffers, endpoint_type& sender_endpoint,
      socket_base::message_flags flags, Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor, 0));
    }
    else
    {
      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec, 0));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      reactor_.start_read_op(impl.socket_,
          receive_from_handler<Mutable_Buffers, Handler>(
            impl.socket_, io_service(), buffers,
            sender_endpoint, flags, handler));
    }
  }

  // Accept a new connection.
  template <typename Socket>
  boost::system::error_code accept(implementation_type& impl, Socket& peer,
      boost::system::error_code& ec)
  {
    // We cannot accept a socket that is already open.
    if (peer.native() != invalid_socket)
    {
      ec = boost::asio::error::already_connected;
      return ec;
    }

    // Accept a socket.
    for (;;)
    {
      // Try to complete the operation without blocking.
      socket_holder new_socket(socket_ops::accept(impl.socket_, 0, 0, ec));

      // Check if operation succeeded.
      if (new_socket.get() >= 0)
      {
        peer.assign(impl.protocol_, new_socket.get(), ec);
        if (!ec)
          new_socket.release();
        return ec;
      }

      // Operation failed.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
      {
        if (impl.flags_ & implementation_type::user_set_non_blocking)
          return ec;
        // Fall through to retry operation.
      }
      else if (ec == boost::asio::error::connection_aborted)
      {
        if (impl.flags_ & implementation_type::enable_connection_aborted)
          return ec;
        // Fall through to retry operation.
      }
      else
        return ec;

      // Wait for socket to become ready.
      if (socket_ops::poll_read(impl.socket_, ec) < 0)
        return ec;
    }
  }

  // Accept a new connection.
  template <typename Socket>
  boost::system::error_code accept_endpoint(implementation_type& impl,
      Socket& peer, endpoint_type& peer_endpoint, boost::system::error_code& ec)
  {
    // We cannot accept a socket that is already open.
    if (peer.native() != invalid_socket)
    {
      ec = boost::asio::error::already_connected;
      return ec;
    }

    // Accept a socket.
    for (;;)
    {
      // Try to complete the operation without blocking.
      socket_addr_len_type addr_len = peer_endpoint.capacity();
      boost::system::error_code ec;
      socket_holder new_socket(socket_ops::accept(
            impl.socket_, peer_endpoint.data(), &addr_len, ec));

      // Check if operation succeeded.
      if (new_socket.get() >= 0)
      {
        peer_endpoint.resize(addr_len);
        peer.assign(impl.protocol_, new_socket.get(), ec);
        if (!ec)
          new_socket.release();
        return ec;
      }

      // Operation failed.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
      {
        if (impl.flags_ & implementation_type::user_set_non_blocking)
          return ec;
        // Fall through to retry operation.
      }
      else if (ec == boost::asio::error::connection_aborted)
      {
        if (impl.flags_ & implementation_type::enable_connection_aborted)
          return ec;
        // Fall through to retry operation.
      }
      else
        return ec;

      // Wait for socket to become ready.
      if (socket_ops::poll_read(impl.socket_, ec) < 0)
        return ec;
    }
  }

  template <typename Socket, typename Handler>
  class accept_handler
  {
  public:
    accept_handler(socket_type socket, boost::asio::io_service& io_service,
        Socket& peer, const protocol_type& protocol,
        bool enable_connection_aborted, Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        peer_(peer),
        protocol_(protocol),
        enable_connection_aborted_(enable_connection_aborted),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result));
        return true;
      }

      // Accept the waiting connection.
      boost::system::error_code ec;
      socket_holder new_socket(socket_ops::accept(socket_, 0, 0, ec));

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;
      if (ec == boost::asio::error::connection_aborted
          && !enable_connection_aborted_)
        return false;

      // Transfer ownership of the new socket to the peer object.
      if (!ec)
      {
        peer_.assign(protocol_, new_socket.get(), ec);
        if (!ec)
          new_socket.release();
      }

      io_service_.post(bind_handler(handler_, ec));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Socket& peer_;
    protocol_type protocol_;
    bool enable_connection_aborted_;
    Handler handler_;
  };

  // Start an asynchronous accept. The peer object must be valid until the
  // accept's handler is invoked.
  template <typename Socket, typename Handler>
  void async_accept(implementation_type& impl, Socket& peer, Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor));
    }
    else if (peer.native() != invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::already_connected));
    }
    else
    {
      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      reactor_.start_read_op(impl.socket_,
          accept_handler<Socket, Handler>(
            impl.socket_, io_service(), peer, impl.protocol_,
            (impl.flags_ & implementation_type::enable_connection_aborted) != 0,
            handler));
    }
  }

  template <typename Socket, typename Handler>
  class accept_endp_handler
  {
  public:
    accept_endp_handler(socket_type socket, boost::asio::io_service& io_service,
        Socket& peer, endpoint_type& peer_endpoint,
        bool enable_connection_aborted, Handler handler)
      : socket_(socket),
        io_service_(io_service),
        work_(io_service),
        peer_(peer),
        peer_endpoint_(peer_endpoint),
        enable_connection_aborted_(enable_connection_aborted),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result));
        return true;
      }

      // Accept the waiting connection.
      socket_addr_len_type addr_len = peer_endpoint_.capacity();
      boost::system::error_code ec;
      socket_holder new_socket(socket_ops::accept(
            socket_, peer_endpoint_.data(), &addr_len, ec));

      // Check if we need to run the operation again.
      if (ec == boost::asio::error::would_block
          || ec == boost::asio::error::try_again)
        return false;
      if (ec == boost::asio::error::connection_aborted
          && !enable_connection_aborted_)
        return false;

      // Transfer ownership of the new socket to the peer object.
      if (!ec)
      {
        peer_endpoint_.resize(addr_len);
        peer_.assign(peer_endpoint_.protocol(), new_socket.get(), ec);
        if (!ec)
          new_socket.release();
      }

      io_service_.post(bind_handler(handler_, ec));
      return true;
    }

  private:
    socket_type socket_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Socket& peer_;
    endpoint_type& peer_endpoint_;
    bool enable_connection_aborted_;
    Handler handler_;
  };

  // Start an asynchronous accept. The peer and peer_endpoint objects
  // must be valid until the accept's handler is invoked.
  template <typename Socket, typename Handler>
  void async_accept_endpoint(implementation_type& impl, Socket& peer,
      endpoint_type& peer_endpoint, Handler handler)
  {
    if (impl.socket_ == invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::bad_descriptor));
    }
    else if (peer.native() != invalid_socket)
    {
      io_service().post(bind_handler(handler,
            boost::asio::error::already_connected));
    }
    else
    {
      // Make socket non-blocking.
      if (!(impl.flags_ & implementation_type::internal_non_blocking))
      {
        ioctl_arg_type non_blocking = 1;
        boost::system::error_code ec;
        if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        {
          io_service().post(bind_handler(handler, ec));
          return;
        }
        impl.flags_ |= implementation_type::internal_non_blocking;
      }

      reactor_.start_read_op(impl.socket_,
          accept_endp_handler<Socket, Handler>(
            impl.socket_, io_service(), peer, peer_endpoint,
            (impl.flags_ & implementation_type::enable_connection_aborted) != 0,
            handler));
    }
  }

  // Connect the socket to the specified endpoint.
  boost::system::error_code connect(implementation_type& impl,
      const endpoint_type& peer_endpoint, boost::system::error_code& ec)
  {
    // Open the socket if it is not already open.
    if (impl.socket_ == invalid_socket)
    {
      // Get the flags used to create the new socket.
      int family = peer_endpoint.protocol().family();
      int type = peer_endpoint.protocol().type();
      int proto = peer_endpoint.protocol().protocol();

      // Create a new socket.
      impl.socket_ = socket_ops::socket(family, type, proto, ec);
      if (impl.socket_ == invalid_socket)
        return ec;

      // Register the socket with the reactor.
      if (int err = reactor_.register_descriptor(impl.socket_))
      {
        socket_ops::close(impl.socket_, ec);
        ec = boost::system::error_code(err, boost::system::native_ecat);
        return ec;
      }
    }
    else if (impl.flags_ & implementation_type::internal_non_blocking)
    {
      // Mark the socket as blocking while we perform the connect.
      ioctl_arg_type non_blocking = 0;
      if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
        return ec;
      impl.flags_ &= ~implementation_type::internal_non_blocking;
    }

    // Perform the connect operation.
    socket_ops::connect(impl.socket_,
        peer_endpoint.data(), peer_endpoint.size(), ec);
    return ec;
  }

  template <typename Handler>
  class connect_handler
  {
  public:
    connect_handler(socket_type socket, boost::shared_ptr<bool> completed,
        boost::asio::io_service& io_service, Reactor& reactor, Handler handler)
      : socket_(socket),
        completed_(completed),
        io_service_(io_service),
        work_(io_service),
        reactor_(reactor),
        handler_(handler)
    {
    }

    bool operator()(const boost::system::error_code& result)
    {
      // Check whether a handler has already been called for the connection.
      // If it has, then we don't want to do anything in this handler.
      if (*completed_)
        return true;

      // Cancel the other reactor operation for the connection.
      *completed_ = true;
      reactor_.enqueue_cancel_ops_unlocked(socket_);

      // Check whether the operation was successful.
      if (result)
      {
        io_service_.post(bind_handler(handler_, result));
        return true;
      }

      // Get the error code from the connect operation.
      int connect_error = 0;
      size_t connect_error_len = sizeof(connect_error);
      boost::system::error_code ec;
      if (socket_ops::getsockopt(socket_, SOL_SOCKET, SO_ERROR,
            &connect_error, &connect_error_len, ec) == socket_error_retval)
      {
        io_service_.post(bind_handler(handler_, ec));
        return true;
      }

      // If connection failed then post the handler with the error code.
      if (connect_error)
      {
        ec = boost::system::error_code(connect_error,
            boost::system::native_ecat);
        io_service_.post(bind_handler(handler_, ec));
        return true;
      }

      // Post the result of the successful connection operation.
      io_service_.post(bind_handler(handler_, ec));
      return true;
    }

  private:
    socket_type socket_;
    boost::shared_ptr<bool> completed_;
    boost::asio::io_service& io_service_;
    boost::asio::io_service::work work_;
    Reactor& reactor_;
    Handler handler_;
  };

  // Start an asynchronous connect.
  template <typename Handler>
  void async_connect(implementation_type& impl,
      const endpoint_type& peer_endpoint, Handler handler)
  {
    // Open the socket if it is not already open.
    if (impl.socket_ == invalid_socket)
    {
      // Get the flags used to create the new socket.
      int family = peer_endpoint.protocol().family();
      int type = peer_endpoint.protocol().type();
      int proto = peer_endpoint.protocol().protocol();

      // Create a new socket.
      boost::system::error_code ec;
      impl.socket_ = socket_ops::socket(family, type, proto, ec);
      if (impl.socket_ == invalid_socket)
      {
        io_service().post(bind_handler(handler, ec));
        return;
      }

      // Register the socket with the reactor.
      if (int err = reactor_.register_descriptor(impl.socket_))
      {
        socket_ops::close(impl.socket_, ec);
        ec = boost::system::error_code(err, boost::system::native_ecat);
        io_service().post(bind_handler(handler, ec));
        return;
      }
    }

    // Make socket non-blocking.
    if (!(impl.flags_ & implementation_type::internal_non_blocking))
    {
      ioctl_arg_type non_blocking = 1;
      boost::system::error_code ec;
      if (socket_ops::ioctl(impl.socket_, FIONBIO, &non_blocking, ec))
      {
        io_service().post(bind_handler(handler, ec));
        return;
      }
      impl.flags_ |= implementation_type::internal_non_blocking;
    }

    // Start the connect operation. The socket is already marked as non-blocking
    // so the connection will take place asynchronously.
    boost::system::error_code ec;
    if (socket_ops::connect(impl.socket_, peer_endpoint.data(),
          peer_endpoint.size(), ec) == 0)
    {
      // The connect operation has finished successfully so we need to post the
      // handler immediately.
      io_service().post(bind_handler(handler, boost::system::error_code()));
    }
    else if (ec == boost::asio::error::in_progress
        || ec == boost::asio::error::would_block)
    {
      // The connection is happening in the background, and we need to wait
      // until the socket becomes writeable.
      boost::shared_ptr<bool> completed(new bool(false));
      reactor_.start_write_and_except_ops(impl.socket_,
          connect_handler<Handler>(
            impl.socket_, completed, io_service(), reactor_, handler));
    }
    else
    {
      // The connect operation has failed, so post the handler immediately.
      io_service().post(bind_handler(handler, ec));
    }
  }

private:
  // The selector that performs event demultiplexing for the provider.
  Reactor& reactor_;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_REACTIVE_SOCKET_SERVICE_HPP
