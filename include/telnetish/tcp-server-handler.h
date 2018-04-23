#ifndef __TCP_SERVER_HANDLER_H__
#define __TCP_SERVER_HANDLER_H__

#include <telnetish/tcp-server.h>
#include <telnetish/tcp-server-event.h>
#include <functional>

class TCPServerHandler {
public:
  using serverEventHandler_t = std::function<void(TCPServerEvent)>;
  
private:
  serverEventHandler_t clientConnectedFn;
  serverEventHandler_t clientDisconnectedFn;

public:

  TCPServerHandler();
  
  void onClientConnected(serverEventHandler_t handler);
  void clientConnected(TCPServerEvent event);
  
  void onClientDisconnected(serverEventHandler_t handler);
  void clientDisconnected(TCPServerEvent event);
  
};

#endif /* __TCP_SERVER_HANDLER_H__ */