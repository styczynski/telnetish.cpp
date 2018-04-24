#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include <telnetish/connection.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <functional>


#define DEFAULT_CONNECTION_ID  -1
#define CONNECTION_NO_SOCKET   -1
#define DEFAULT_MESSAGE_BUFFER_LENGTH 100000


class TCPConnection : public Connection {
private:

public:

  TCPConnection(const int socket=CONNECTION_NO_SOCKET, const int id=DEFAULT_CONNECTION_ID): Connection(socket, id) {

  }

  ~TCPConnection() {

  }

  bool end() override {
    this->opened = false;
    if (close(this->socket) < 0) {
      return false;
    }
    return true;
  }

  int readData(char* output, const int length) override {
    return read(this->socket, output, length);
  }

  int writeData(const char* input, const int length) override {
    if (write(this->socket, input, length) != length) {
      return -1;
    }
    return 0;
  }

  Connection& operator>>(std::string& output) override {
    static char buffer[DEFAULT_MESSAGE_BUFFER_LENGTH];
    buffer[0] = '\0';
    this->readData(buffer, DEFAULT_MESSAGE_BUFFER_LENGTH);
    output = std::string(buffer);
    return *this;
  }

  Connection& operator<<(std::string input) override {
    this->writeData(input.c_str(), input.size());
    return *this;
  }

  Connection& operator>>(char* output) override {
    this->readData(output, DEFAULT_MESSAGE_BUFFER_LENGTH);
    return *this;
  }

  Connection& operator<<(const char* input) override {
    this->writeData(input, strlen(input));
    return *this;
  }
};


#endif /* __TCP_CONNECTION_H__ */
