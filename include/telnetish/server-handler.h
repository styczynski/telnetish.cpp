/** @file
*
*  Utilities to handle server events.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __SERVER_HANDLER_H__
#define __SERVER_HANDLER_H__

#include <functional>

/**
 * Class representing event handler for server events of type @p EventT
 */
template< typename EventT >
class ServerHandler {
public:
  using serverEventHandler_t = std::function<void(EventT)>;

private:
  serverEventHandler_t clientConnectedFn;
  serverEventHandler_t clientDisconnectedFn;

public:

  /**
   * Create new empty handler.
   */
  ServerHandler() {
    this->clientConnectedFn = [](EventT event)->void {};
    this->clientDisconnectedFn = [](EventT event)->void {};
  }

  /**
   * Register event listener to the client connected event.
   *
   * @param[in] hadner : event listener
   */
  void onClientConnected(serverEventHandler_t handler) {
    const serverEventHandler_t next = this->clientConnectedFn;
    this->clientConnectedFn = [next, handler](EventT event)->void {
      handler(event);
      next(event);
    };
  }

  /**
   * Trigger event listeners for client connected event.
   *
   * @param[in] event : event data passed to the listeners
   */
  void clientConnected(EventT event) {
    this->clientConnectedFn(event);
  }

  /**
   * Register event listener to the client disconnected event.
   *
   * @param[in] hadner : event listener
   */
  void onClientDisconnected(serverEventHandler_t handler) {
    const serverEventHandler_t next = this->clientDisconnectedFn;
    this->clientDisconnectedFn = [next, handler](EventT event)->void {
      handler(event);
      next(event);
    };
  }

  /**
   * Trigger event listeners for client disconnected event.
   *
   * @param[in] event : event data passed to the listeners
   */
  void clientDisconnected(EventT event) {
    this->clientDisconnectedFn(event);
  }

};

#endif /* __SERVER_HANDLER_H__ */
