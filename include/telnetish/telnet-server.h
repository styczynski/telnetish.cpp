#ifndef __TELNET_SERVER_H__
#define __TELNET_SERVER_H__

class TelnetServer;

#include <telnetish/server.h>
#include <telnetish/loggable.h>
#include <telnetish/tcp-server.h>
#include <telnetish/server-event.h>
#include <telnetish/server-handler.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <functional>

class TelnetServerEventData {

};

class TelnetServer : public Server<TelnetServerEventData> {
public:
  using TelnetServerEvent = ServerEvent<TelnetServerEventData>;

protected:
  TCPServer server;
  bool inited;

public:

  TelnetServer(const int port=DEFAULT_SERVER_PORT) : Server<TelnetServerEventData>(port) {
    this->inited = false;
  }

  ~TelnetServer() {

  }

  bool init() override;

  bool start() override;

  void shutdown() override;

};

#endif /* __TCP_SERVER_H__ */
