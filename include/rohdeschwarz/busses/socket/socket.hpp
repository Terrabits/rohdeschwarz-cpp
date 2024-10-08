/**
 * \file  socket.hpp
 * \brief rohdeschwarz::busses::socket::Socket class definition
 */
#ifndef ROHDESCHWARZ_BUSSES_SOCKET_SOCKET_HPP
#define ROHDESCHWARZ_BUSSES_SOCKET_SOCKET_HPP


// rohdeschwarz
#include "rohdeschwarz/busses/bus.hpp"


// boost
#include <boost/asio.hpp>


// std lib
#include <cstddef>
#include <string>


namespace rohdeschwarz::busses::socket
{


/**
 * \brief System error exception
 *
 * This is a reference to `boost::system::system_error` provided for
 * convenience
 */
using system_error = boost::system::system_error;


/**
 * \brief A class for managing synchronous TCP IP sockets
 */
class Socket : public rohdeschwarz::busses::Bus
{

public:


  /**
   * \brief Constructor
   *
   * Constructs a socket object that is connected to server `host`, `port`
   *
   * \param[in] host host or ip address
   * \param[in] port port number
   * \returns   An object with an open socket
   * \exception `boost::system::system_error` if connection fails
   */
  Socket(const char* host, int port = 5025);


  /**
   * \brief Constructor
   *
   * Constructs a socket object that is connected to server `host`, `port`
   *
   * \param[in] host host or ip address
   * \param[in] port port number
   * \returns   An object with an open socket
   * \exception `boost::system::system_error` if connection fails
   */
  Socket(const std::string& host, int port = 5025);


  /**
   * \brief Destructor
   *
   * The destructor closes the socket if it is currently open
   */
  virtual ~Socket();


  /**
   * \brief returns the host or ip address
   */
  std::string host() const;


  /**
   * \brief returns the port
   */
   int port() const;


  /**
   * \brief Returns string '{host}:{port}'
   */
   virtual std::string endpoint() const;


   /**
    * \brief Get timeout, in ms
    */
   virtual int timeout_ms() const;


   /**
    * \brief Set timeout, in ms
    */
   virtual bool setTimeout(int timeout_ms);


   /**
    * \brief read data into buffer
    *
    * \param[in]  buffer     Buffer for read
    * \param[in]  bufferSize Size of buffer
    * \param[out] readSize   Returns bytes read
    * \returns    true if read succeeded; false otherwise
    */
   virtual bool readData(unsigned char* buffer, std::size_t bufferSize, std::size_t* readSize = nullptr);


   /**
    * \brief read data into buffer
    *
    * \param[in]  data      Data to be written
    * \param[in]  dataSize  Size of data to be written
    * \param[out] writeSize Returns bytes written
    * \returns    true if write succeeded; false otherwise
    */
   virtual bool writeData(const unsigned char* data, std::size_t dataSize, std::size_t* writeSize = nullptr);


   /**
    * \brief Checks socket state for error
    * \returns true if the connection is open; otherwise false
    * otherwise.
    */
    virtual bool isError() const;


   /**
    * \brief human-readable bus status message
    *
    * \returns "connection is open" if connected;
    *   "warning: connection is closed" otherwise.
    */
   virtual std::string statusMessage() const;


private:

  // for endpoint
  std::string _host;
  int         _port;


  // socket
  boost::asio::io_context      _io_context;
  boost::asio::ip::tcp::socket _socket;
  boost::asio::ip::tcp::socket::native_handle_type _native_handle;


  // helpers


  /**
   * \brief Connects to instrument
   */
  bool open();


  /**
   * \brief Disconnects from instrument
   */
  void close();


};  // class Socket


}       // namespace rohdeschwarz::busses::socket
#endif  // ROHDESCHWARZ_BUSSES_SOCKET_SOCKET_HPP
