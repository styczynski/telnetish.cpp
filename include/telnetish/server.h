/** @file
*
*  C++ server abstraction
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __SERVER_H__
#define __SERVER_H__

/**
 * @def DEFAULT_SERVER_PORT
 *
 * Default server port set when its not specified during
 * creation of the server.
 */
#define DEFAULT_SERVER_PORT          3000

template< typename EventT > class Server;

#include <telnetish/server-handler.h>
#include <telnetish/server-event.h>
#include <telnetish/loggable.h>
#include <string>

/**
 * Abstract class representing a server that handles events of type @p EventT
 */
template< typename EventT >
class Server : public Loggable, public ServerHandler<ServerEvent<EventT>> {
public:
  using ServerHandlerType = ServerHandler<ServerEvent<EventT>>;

protected:
  int port;

public:

  /**
   * Create server listening on the port @p port.
   */
  Server(const int port=DEFAULT_SERVER_PORT) : Loggable(), ServerHandlerType() {
    this->port = port;
  }

  /**
   * Sets server port.
   *
   * @param[in] port : server port integer
   */
  void setPort(const int port=DEFAULT_SERVER_PORT) {
    this->port = port;
  }

  /**
   * Sets server port.
   *
   * @param[in] port : server port string number
   */
  void setPort(std::string port) {
    this->port = std::stoi(port);
  }

  /**
   * Initialize server.
   */
  virtual bool init() = 0;
  
  /**
   * Run server.
   */
  virtual bool start() = 0;
  
  /**
   * Shutdown server.
   */
  virtual void shutdown() = 0;

};

#endif /* __SERVER_H__ */
