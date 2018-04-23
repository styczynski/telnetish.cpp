#include <telnetish/tcp-server.h>

TCPServerHandler::TCPServerHandler() {
  this->clientConnectedFn = [](TCPServerEvent event)->void {};
  this->clientDisconnectedFn = [](TCPServerEvent event)->void {};
}

void TCPServerHandler::onClientConnected(serverEventHandler_t handler) {
  const serverEventHandler_t next = this->clientConnectedFn;
  this->clientConnectedFn = [next, handler](TCPServerEvent event)->void {
    handler(event);
    next(event);
  };
}

void TCPServerHandler::onClientDisconnected(serverEventHandler_t handler) {
  const serverEventHandler_t next = this->clientDisconnectedFn;
  this->clientDisconnectedFn = [next, handler](TCPServerEvent event)->void {
    handler(event);
    next(event);
  };
}

void TCPServerHandler::clientConnected(TCPServerEvent event) {
  this->clientConnectedFn(event);
}

void TCPServerHandler::clientDisconnected(TCPServerEvent event) {
  this->clientDisconnectedFn(event);
}