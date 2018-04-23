#include <telnetish/tcp-server.h>

TCPServerEvent::TCPServerEvent(TCPServer* serverSource, TCPConnection* connection) {
  this->source = serverSource;
  this->connection = connection;
}

TCPConnection* TCPServerEvent::getServerConnection() {
  return this->connection;
}

TCPServer& TCPServerEvent::getSource() {
  return *(this->source);
}