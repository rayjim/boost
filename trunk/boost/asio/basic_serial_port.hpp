//
// basic_serial_port.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2008 Rep Invariant Systems, Inc. (info@repinvariant.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_BASIC_SERIAL_PORT_HPP
#define BOOST_ASIO_BASIC_SERIAL_PORT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if defined(BOOST_ASIO_HAS_SERIAL_PORT) \
  || defined(GENERATING_DOCUMENTATION)

#include <string>
#include <boost/asio/basic_io_object.hpp>
#include <boost/asio/detail/handler_type_requirements.hpp>
#include <boost/asio/detail/throw_error.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/serial_port_service.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {

/// Provides serial port functionality.
/**
 * The basic_serial_port class template provides functionality that is common
 * to all serial ports.
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Unsafe.
 */
template <typename SerialPortService = serial_port_service>
class basic_serial_port
  : public basic_io_object<SerialPortService>,
    public serial_port_base
{
public:
  /// (Deprecated: Use native_handle_type.) The native representation of a
  /// serial port.
  typedef typename SerialPortService::native_handle_type native_type;

  /// The native representation of a serial port.
  typedef typename SerialPortService::native_handle_type native_handle_type;

  /// A basic_serial_port is always the lowest layer.
  typedef basic_serial_port<SerialPortService> lowest_layer_type;

  /// Construct a basic_serial_port without opening it.
  /**
   * This constructor creates a serial port without opening it.
   *
   * @param io_service The io_service object that the serial port will use to
   * dispatch handlers for any asynchronous operations performed on the port.
   */
  explicit basic_serial_port(boost::asio::io_service& io_service)
    : basic_io_object<SerialPortService>(io_service)
  {
  }

  /// Construct and open a basic_serial_port.
  /**
   * This constructor creates and opens a serial port for the specified device
   * name.
   *
   * @param io_service The io_service object that the serial port will use to
   * dispatch handlers for any asynchronous operations performed on the port.
   *
   * @param device The platform-specific device name for this serial
   * port.
   */
  explicit basic_serial_port(boost::asio::io_service& io_service,
      const char* device)
    : basic_io_object<SerialPortService>(io_service)
  {
    boost::system::error_code ec;
    this->service.open(this->implementation, device, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Construct and open a basic_serial_port.
  /**
   * This constructor creates and opens a serial port for the specified device
   * name.
   *
   * @param io_service The io_service object that the serial port will use to
   * dispatch handlers for any asynchronous operations performed on the port.
   *
   * @param device The platform-specific device name for this serial
   * port.
   */
  explicit basic_serial_port(boost::asio::io_service& io_service,
      const std::string& device)
    : basic_io_object<SerialPortService>(io_service)
  {
    boost::system::error_code ec;
    this->service.open(this->implementation, device, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Construct a basic_serial_port on an existing native serial port.
  /**
   * This constructor creates a serial port object to hold an existing native
   * serial port.
   *
   * @param io_service The io_service object that the serial port will use to
   * dispatch handlers for any asynchronous operations performed on the port.
   *
   * @param native_serial_port A native serial port.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  basic_serial_port(boost::asio::io_service& io_service,
      const native_handle_type& native_serial_port)
    : basic_io_object<SerialPortService>(io_service)
  {
    boost::system::error_code ec;
    this->service.assign(this->implementation, native_serial_port, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Get a reference to the lowest layer.
  /**
   * This function returns a reference to the lowest layer in a stack of
   * layers. Since a basic_serial_port cannot contain any further layers, it
   * simply returns a reference to itself.
   *
   * @return A reference to the lowest layer in the stack of layers. Ownership
   * is not transferred to the caller.
   */
  lowest_layer_type& lowest_layer()
  {
    return *this;
  }

  /// Get a const reference to the lowest layer.
  /**
   * This function returns a const reference to the lowest layer in a stack of
   * layers. Since a basic_serial_port cannot contain any further layers, it
   * simply returns a reference to itself.
   *
   * @return A const reference to the lowest layer in the stack of layers.
   * Ownership is not transferred to the caller.
   */
  const lowest_layer_type& lowest_layer() const
  {
    return *this;
  }

  /// Open the serial port using the specified device name.
  /**
   * This function opens the serial port for the specified device name.
   *
   * @param device The platform-specific device name.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  void open(const std::string& device)
  {
    boost::system::error_code ec;
    this->service.open(this->implementation, device, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Open the serial port using the specified device name.
  /**
   * This function opens the serial port using the given platform-specific
   * device name.
   *
   * @param device The platform-specific device name.
   *
   * @param ec Set the indicate what error occurred, if any.
   */
  boost::system::error_code open(const std::string& device,
      boost::system::error_code& ec)
  {
    return this->service.open(this->implementation, device, ec);
  }

  /// Assign an existing native serial port to the serial port.
  /*
   * This function opens the serial port to hold an existing native serial port.
   *
   * @param native_serial_port A native serial port.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  void assign(const native_handle_type& native_serial_port)
  {
    boost::system::error_code ec;
    this->service.assign(this->implementation, native_serial_port, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Assign an existing native serial port to the serial port.
  /*
   * This function opens the serial port to hold an existing native serial port.
   *
   * @param native_serial_port A native serial port.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  boost::system::error_code assign(const native_handle_type& native_serial_port,
      boost::system::error_code& ec)
  {
    return this->service.assign(this->implementation, native_serial_port, ec);
  }

  /// Determine whether the serial port is open.
  bool is_open() const
  {
    return this->service.is_open(this->implementation);
  }

  /// Close the serial port.
  /**
   * This function is used to close the serial port. Any asynchronous read or
   * write operations will be cancelled immediately, and will complete with the
   * boost::asio::error::operation_aborted error.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  void close()
  {
    boost::system::error_code ec;
    this->service.close(this->implementation, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Close the serial port.
  /**
   * This function is used to close the serial port. Any asynchronous read or
   * write operations will be cancelled immediately, and will complete with the
   * boost::asio::error::operation_aborted error.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  boost::system::error_code close(boost::system::error_code& ec)
  {
    return this->service.close(this->implementation, ec);
  }

  /// (Deprecated: Use native_handle().) Get the native serial port
  /// representation.
  /**
   * This function may be used to obtain the underlying representation of the
   * serial port. This is intended to allow access to native serial port
   * functionality that is not otherwise provided.
   */
  native_type native()
  {
    return this->service.native_handle(this->implementation);
  }

  /// Get the native serial port representation.
  /**
   * This function may be used to obtain the underlying representation of the
   * serial port. This is intended to allow access to native serial port
   * functionality that is not otherwise provided.
   */
  native_handle_type native_handle()
  {
    return this->service.native_handle(this->implementation);
  }

  /// Cancel all asynchronous operations associated with the serial port.
  /**
   * This function causes all outstanding asynchronous read or write operations
   * to finish immediately, and the handlers for cancelled operations will be
   * passed the boost::asio::error::operation_aborted error.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  void cancel()
  {
    boost::system::error_code ec;
    this->service.cancel(this->implementation, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Cancel all asynchronous operations associated with the serial port.
  /**
   * This function causes all outstanding asynchronous read or write operations
   * to finish immediately, and the handlers for cancelled operations will be
   * passed the boost::asio::error::operation_aborted error.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  boost::system::error_code cancel(boost::system::error_code& ec)
  {
    return this->service.cancel(this->implementation, ec);
  }

  /// Send a break sequence to the serial port.
  /**
   * This function causes a break sequence of platform-specific duration to be
   * sent out the serial port.
   *
   * @throws boost::system::system_error Thrown on failure.
   */
  void send_break()
  {
    boost::system::error_code ec;
    this->service.send_break(this->implementation, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Send a break sequence to the serial port.
  /**
   * This function causes a break sequence of platform-specific duration to be
   * sent out the serial port.
   *
   * @param ec Set to indicate what error occurred, if any.
   */
  boost::system::error_code send_break(boost::system::error_code& ec)
  {
    return this->service.send_break(this->implementation, ec);
  }

  /// Set an option on the serial port.
  /**
   * This function is used to set an option on the serial port.
   *
   * @param option The option value to be set on the serial port.
   *
   * @throws boost::system::system_error Thrown on failure.
   *
   * @sa SettableSerialPortOption @n
   * boost::asio::serial_port_base::baud_rate @n
   * boost::asio::serial_port_base::flow_control @n
   * boost::asio::serial_port_base::parity @n
   * boost::asio::serial_port_base::stop_bits @n
   * boost::asio::serial_port_base::character_size
   */
  template <typename SettableSerialPortOption>
  void set_option(const SettableSerialPortOption& option)
  {
    boost::system::error_code ec;
    this->service.set_option(this->implementation, option, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Set an option on the serial port.
  /**
   * This function is used to set an option on the serial port.
   *
   * @param option The option value to be set on the serial port.
   *
   * @param ec Set to indicate what error occurred, if any.
   *
   * @sa SettableSerialPortOption @n
   * boost::asio::serial_port_base::baud_rate @n
   * boost::asio::serial_port_base::flow_control @n
   * boost::asio::serial_port_base::parity @n
   * boost::asio::serial_port_base::stop_bits @n
   * boost::asio::serial_port_base::character_size
   */
  template <typename SettableSerialPortOption>
  boost::system::error_code set_option(const SettableSerialPortOption& option,
      boost::system::error_code& ec)
  {
    return this->service.set_option(this->implementation, option, ec);
  }

  /// Get an option from the serial port.
  /**
   * This function is used to get the current value of an option on the serial
   * port.
   *
   * @param option The option value to be obtained from the serial port.
   *
   * @throws boost::system::system_error Thrown on failure.
   *
   * @sa GettableSerialPortOption @n
   * boost::asio::serial_port_base::baud_rate @n
   * boost::asio::serial_port_base::flow_control @n
   * boost::asio::serial_port_base::parity @n
   * boost::asio::serial_port_base::stop_bits @n
   * boost::asio::serial_port_base::character_size
   */
  template <typename GettableSerialPortOption>
  void get_option(GettableSerialPortOption& option)
  {
    boost::system::error_code ec;
    this->service.get_option(this->implementation, option, ec);
    boost::asio::detail::throw_error(ec);
  }

  /// Get an option from the serial port.
  /**
   * This function is used to get the current value of an option on the serial
   * port.
   *
   * @param option The option value to be obtained from the serial port.
   *
   * @param ec Set to indicate what error occured, if any.
   *
   * @sa GettableSerialPortOption @n
   * boost::asio::serial_port_base::baud_rate @n
   * boost::asio::serial_port_base::flow_control @n
   * boost::asio::serial_port_base::parity @n
   * boost::asio::serial_port_base::stop_bits @n
   * boost::asio::serial_port_base::character_size
   */
  template <typename GettableSerialPortOption>
  boost::system::error_code get_option(GettableSerialPortOption& option,
      boost::system::error_code& ec)
  {
    return this->service.get_option(this->implementation, option, ec);
  }

  /// Write some data to the serial port.
  /**
   * This function is used to write data to the serial port. The function call
   * will block until one or more bytes of the data has been written
   * successfully, or until an error occurs.
   *
   * @param buffers One or more data buffers to be written to the serial port.
   *
   * @returns The number of bytes written.
   *
   * @throws boost::system::system_error Thrown on failure. An error code of
   * boost::asio::error::eof indicates that the connection was closed by the
   * peer.
   *
   * @note The write_some operation may not transmit all of the data to the
   * peer. Consider using the @ref write function if you need to ensure that
   * all data is written before the blocking operation completes.
   *
   * @par Example
   * To write a single data buffer use the @ref buffer function as follows:
   * @code
   * serial_port.write_some(boost::asio::buffer(data, size));
   * @endcode
   * See the @ref buffer documentation for information on writing multiple
   * buffers in one go, and how to use it with arrays, boost::array or
   * std::vector.
   */
  template <typename ConstBufferSequence>
  std::size_t write_some(const ConstBufferSequence& buffers)
  {
    boost::system::error_code ec;
    std::size_t s = this->service.write_some(this->implementation, buffers, ec);
    boost::asio::detail::throw_error(ec);
    return s;
  }

  /// Write some data to the serial port.
  /**
   * This function is used to write data to the serial port. The function call
   * will block until one or more bytes of the data has been written
   * successfully, or until an error occurs.
   *
   * @param buffers One or more data buffers to be written to the serial port.
   *
   * @param ec Set to indicate what error occurred, if any.
   *
   * @returns The number of bytes written. Returns 0 if an error occurred.
   *
   * @note The write_some operation may not transmit all of the data to the
   * peer. Consider using the @ref write function if you need to ensure that
   * all data is written before the blocking operation completes.
   */
  template <typename ConstBufferSequence>
  std::size_t write_some(const ConstBufferSequence& buffers,
      boost::system::error_code& ec)
  {
    return this->service.write_some(this->implementation, buffers, ec);
  }

  /// Start an asynchronous write.
  /**
   * This function is used to asynchronously write data to the serial port.
   * The function call always returns immediately.
   *
   * @param buffers One or more data buffers to be written to the serial port.
   * Although the buffers object may be copied as necessary, ownership of the
   * underlying memory blocks is retained by the caller, which must guarantee
   * that they remain valid until the handler is called.
   *
   * @param handler The handler to be called when the write operation completes.
   * Copies will be made of the handler as required. The function signature of
   * the handler must be:
   * @code void handler(
   *   const boost::system::error_code& error, // Result of operation.
   *   std::size_t bytes_transferred           // Number of bytes written.
   * ); @endcode
   * Regardless of whether the asynchronous operation completes immediately or
   * not, the handler will not be invoked from within this function. Invocation
   * of the handler will be performed in a manner equivalent to using
   * boost::asio::io_service::post().
   *
   * @note The write operation may not transmit all of the data to the peer.
   * Consider using the @ref async_write function if you need to ensure that all
   * data is written before the asynchronous operation completes.
   *
   * @par Example
   * To write a single data buffer use the @ref buffer function as follows:
   * @code
   * serial_port.async_write_some(boost::asio::buffer(data, size), handler);
   * @endcode
   * See the @ref buffer documentation for information on writing multiple
   * buffers in one go, and how to use it with arrays, boost::array or
   * std::vector.
   */
  template <typename ConstBufferSequence, typename WriteHandler>
  void async_write_some(const ConstBufferSequence& buffers,
      WriteHandler handler)
  {
    // If you get an error on the following line it means that your handler does
    // not meet the documented type requirements for a WriteHandler.
    BOOST_ASIO_WRITE_HANDLER_CHECK(WriteHandler, handler) type_check;

    this->service.async_write_some(this->implementation, buffers,
        BOOST_ASIO_MOVE_CAST(WriteHandler)(handler));
  }

  /// Read some data from the serial port.
  /**
   * This function is used to read data from the serial port. The function
   * call will block until one or more bytes of data has been read successfully,
   * or until an error occurs.
   *
   * @param buffers One or more buffers into which the data will be read.
   *
   * @returns The number of bytes read.
   *
   * @throws boost::system::system_error Thrown on failure. An error code of
   * boost::asio::error::eof indicates that the connection was closed by the
   * peer.
   *
   * @note The read_some operation may not read all of the requested number of
   * bytes. Consider using the @ref read function if you need to ensure that
   * the requested amount of data is read before the blocking operation
   * completes.
   *
   * @par Example
   * To read into a single data buffer use the @ref buffer function as follows:
   * @code
   * serial_port.read_some(boost::asio::buffer(data, size));
   * @endcode
   * See the @ref buffer documentation for information on reading into multiple
   * buffers in one go, and how to use it with arrays, boost::array or
   * std::vector.
   */
  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers)
  {
    boost::system::error_code ec;
    std::size_t s = this->service.read_some(this->implementation, buffers, ec);
    boost::asio::detail::throw_error(ec);
    return s;
  }

  /// Read some data from the serial port.
  /**
   * This function is used to read data from the serial port. The function
   * call will block until one or more bytes of data has been read successfully,
   * or until an error occurs.
   *
   * @param buffers One or more buffers into which the data will be read.
   *
   * @param ec Set to indicate what error occurred, if any.
   *
   * @returns The number of bytes read. Returns 0 if an error occurred.
   *
   * @note The read_some operation may not read all of the requested number of
   * bytes. Consider using the @ref read function if you need to ensure that
   * the requested amount of data is read before the blocking operation
   * completes.
   */
  template <typename MutableBufferSequence>
  std::size_t read_some(const MutableBufferSequence& buffers,
      boost::system::error_code& ec)
  {
    return this->service.read_some(this->implementation, buffers, ec);
  }

  /// Start an asynchronous read.
  /**
   * This function is used to asynchronously read data from the serial port.
   * The function call always returns immediately.
   *
   * @param buffers One or more buffers into which the data will be read.
   * Although the buffers object may be copied as necessary, ownership of the
   * underlying memory blocks is retained by the caller, which must guarantee
   * that they remain valid until the handler is called.
   *
   * @param handler The handler to be called when the read operation completes.
   * Copies will be made of the handler as required. The function signature of
   * the handler must be:
   * @code void handler(
   *   const boost::system::error_code& error, // Result of operation.
   *   std::size_t bytes_transferred           // Number of bytes read.
   * ); @endcode
   * Regardless of whether the asynchronous operation completes immediately or
   * not, the handler will not be invoked from within this function. Invocation
   * of the handler will be performed in a manner equivalent to using
   * boost::asio::io_service::post().
   *
   * @note The read operation may not read all of the requested number of bytes.
   * Consider using the @ref async_read function if you need to ensure that the
   * requested amount of data is read before the asynchronous operation
   * completes.
   *
   * @par Example
   * To read into a single data buffer use the @ref buffer function as follows:
   * @code
   * serial_port.async_read_some(boost::asio::buffer(data, size), handler);
   * @endcode
   * See the @ref buffer documentation for information on reading into multiple
   * buffers in one go, and how to use it with arrays, boost::array or
   * std::vector.
   */
  template <typename MutableBufferSequence, typename ReadHandler>
  void async_read_some(const MutableBufferSequence& buffers,
      ReadHandler handler)
  {
    // If you get an error on the following line it means that your handler does
    // not meet the documented type requirements for a ReadHandler.
    BOOST_ASIO_READ_HANDLER_CHECK(ReadHandler, handler) type_check;

    this->service.async_read_some(this->implementation, buffers,
        BOOST_ASIO_MOVE_CAST(ReadHandler)(handler));
  }
};

} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_ASIO_HAS_SERIAL_PORT)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // BOOST_ASIO_BASIC_SERIAL_PORT_HPP
