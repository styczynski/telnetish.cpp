#include <telnetish/telnet-server.h>

bool TelnetServer::init() {
  if(this->inited) {
    return false;
  }

  this->server = TCPServer();
  this->inited = false;

  this->server.redirectLogTo(*this);

  this->server.setPort(this->port);

  this->server.onClientConnected([this](TCPServer::TCPServerEvent tcpEvent){
    this->log("New client connected to the TELNET");
    TelnetServerEvent event(this, tcpEvent.getConnectionSource());
    this->clientConnected(event);
  });

  return true;
}

void TelnetServer::shutdown() {
  this->server.shutdown();
}

bool TelnetServer::start() {
  this->init();
  this->server.start();
}
