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

int TCPConnection::readData(char* output, const int length) {
  return read(this->socket, output, length);
}

int TCPConnection::writeData(const char* input, const int length) {
  if (write(this->socket, input, length) != length) {
    return -1;
  }
  return 0;
}

TCPConnection& TCPConnection::operator>>(std::string& output) {
  static char buffer[DEFAULT_MESSAGE_BUFFER_LENGTH];
  buffer[0] = '\0';
  this->readData(buffer, DEFAULT_MESSAGE_BUFFER_LENGTH);
  output = std::string(buffer);
  return *this;
}

TCPConnection& TCPConnection::operator<<(std::string input) {
  this->writeData(input.c_str(), input.size());
  return *this;
}

TCPConnection& TCPConnection::operator>>(char* output) {
  this->readData(output, DEFAULT_MESSAGE_BUFFER_LENGTH);
  return *this;
}

TCPConnection& TCPConnection::operator<<(const char* input) {
  this->writeData(input, strlen(input));
  return *this;
}
