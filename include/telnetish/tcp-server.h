#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define DEFAULT_TCP_LISTEN_QUEUE_LENGTH 1000

class TCPServer;

#include <telnetish/server.h>
#include <telnetish/loggable.h>
#include <telnetish/tcp-connection.h>
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

class TCPServerEventData {

};

class TCPServer : public Server<TCPServerEventData> {
public:
  using TCPServerEvent = ServerEvent<TCPServerEventData>;

protected:
  int in_sock;
  int out_sock;
  bool inited;
  bool on;
  int next_client_id;

public:

  TCPServer(const int port=DEFAULT_SERVER_PORT) : Server<TCPServerEventData>(port) {
    this->inited = false;
    this->in_sock = 0;
    this->out_sock = 0;
    this->next_client_id = -1;
    this->on = false;
  }

  ~TCPServer() {

  }

  bool init() override;

  bool start() override;

  void shutdown() override;

};

#endif /* __TCP_SERVER_H__ */
