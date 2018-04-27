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
#include <vector>
#include <functional>
#include <iostream>

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
    const int len = read(this->socket, output, length);
    return len;
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
    const int len = this->readData(buffer, DEFAULT_MESSAGE_BUFFER_LENGTH);
    if(len > 0) {
      output = std::string(buffer, len);
    }
    return *this;
  }

  Connection& operator>>(Message& output) override {
    static char buffer[DEFAULT_MESSAGE_BUFFER_LENGTH];
    buffer[0] = '\0';
    const int len = this->readData(buffer, DEFAULT_MESSAGE_BUFFER_LENGTH);
    if(len > 0) {
      output = Message(std::string(buffer, len));
    }
    return *this;
  }

  Connection& operator<<(Message input) override {
    this->writeData(input.getContents(), input.getSize());
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

  Connection& send(std::vector<int> input) override {
    const int size = input.size();
    char buffer[size+7];
    int i = 0;
    for(auto c : input ) {
      buffer[i++] = (char) c;
    }
    this->writeData(buffer, size);
    return *this;
  }

};


#endif /* __TCP_CONNECTION_H__ */
