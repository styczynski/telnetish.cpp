#ifndef __TCP_SERVER_HANDLER_H__
#define __TCP_SERVER_HANDLER_H__

#include <functional>

template< typename EventT >
class ServerHandler {
public:
  using serverEventHandler_t = std::function<void(EventT)>;

private:
  serverEventHandler_t clientConnectedFn;
  serverEventHandler_t clientDisconnectedFn;

public:

  ServerHandler() {
    this->clientConnectedFn = [](EventT event)->void {};
    this->clientDisconnectedFn = [](EventT event)->void {};
  }

  void onClientConnected(serverEventHandler_t handler) {
    const serverEventHandler_t next = this->clientConnectedFn;
    this->clientConnectedFn = [next, handler](EventT event)->void {
      handler(event);
      next(event);
    };
  }

  void clientConnected(EventT event) {
    this->clientConnectedFn(event);
  }

  void onClientDisconnected(serverEventHandler_t handler) {
    const serverEventHandler_t next = this->clientDisconnectedFn;
    this->clientDisconnectedFn = [next, handler](EventT event)->void {
      handler(event);
      next(event);
    };
  }

  void clientDisconnected(EventT event) {
    this->clientDisconnectedFn(event);
  }

};

#endif /* __TCP_SERVER_HANDLER_H__ */
