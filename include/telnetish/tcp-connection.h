/** @file
*
*  Implementation of TCP connection.
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
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

/**
 * Class representing TCP socket connection.
 */
class TCPConnection : public Connection {
protected:
  
  int readData(char* output, const int length) override {
    const int len = read(this->socket, output, length);
    if(len < 0) return 0;
    return len;
  }

  int writeData(const char* input, const int length) override {
    if (write(this->socket, input, length) != length) {
      return -1;
    }
    return 0;
  }


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

};


#endif /* __TCP_CONNECTION_H__ */
