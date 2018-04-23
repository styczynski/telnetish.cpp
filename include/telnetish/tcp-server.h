#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

class TCPServer;

#include <telnetish/loggable.h>
#include <telnetish/tcp-connection.h>
#include <telnetish/tcp-server-event.h>
#include <telnetish/tcp-server-handler.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <functional>

#define DEFAULT_LISTEN_QUEUE_LENGTH  10
#define DEFAULT_SERVER_PORT          3000

class TCPServer : public Loggable, public TCPServerHandler {
private:
  int in_sock;
  int out_sock;
  int port;
  bool inited;
  int next_client_id;
  
public:
  
  TCPServer(const int port=DEFAULT_SERVER_PORT) : Loggable(), TCPServerHandler() {
    this->port = port;
    this->inited = false;
    this->in_sock = 0;
    this->out_sock = 0;
    this->next_client_id = -1;
  }
  
  void setPort(const int port=DEFAULT_SERVER_PORT);
  bool init();
  bool start(const int input_queue_length=DEFAULT_LISTEN_QUEUE_LENGTH);
  
};

#endif /* __TCP_SERVER_H__ */