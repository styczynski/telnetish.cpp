#include <telnetish/tcp-server.h>

TCPConnection::TCPConnection(const int socket, const int id) {
  this->socket = socket;
  this->opened = (socket != CONNECTION_NO_SOCKET);
}

int TCPConnection::getSocket() const {
  return this->socket;
}

int TCPConnection::getID() const {
  return this->id;
}

bool TCPConnection::isOpened() const {
  return this->opened;
}

bool TCPConnection::end() {
  this->opened = false;
  if (close(this->socket) < 0) {
    return false;
  }
  return true;
}