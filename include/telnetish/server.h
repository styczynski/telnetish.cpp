#ifndef __SERVER_H__
#define __SERVER_H__

#define DEFAULT_SERVER_PORT          3000

template< typename EventT > class Server;

#include <telnetish/server-handler.h>
#include <telnetish/server-event.h>
#include <telnetish/loggable.h>
#include <string>

template< typename EventT >
class Server : public Loggable, public ServerHandler<ServerEvent<EventT>> {
public:
  using ServerHandlerType = ServerHandler<ServerEvent<EventT>>;

protected:
  int port;

public:

  Server(const int port=DEFAULT_SERVER_PORT) : Loggable(), ServerHandlerType() {
    this->port = port;
  }

  void setPort(const int port=DEFAULT_SERVER_PORT) {
    this->port = port;
  }

  void setPort(std::string port) {
    this->port = std::stoi(port);
  }

  virtual bool init() = 0;
  virtual bool start() = 0;
  virtual void shutdown() = 0;

};

#endif /* __SERVER_H__ */
