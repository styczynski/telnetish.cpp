#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#define DEFAULT_CONNECTION_ID  -1
#define CONNECTION_NO_SOCKET   -1

#include <telnetish/message.h>
#include <vector>
#include <string>

class Connection {
protected:
  int socket;
  int id;
  bool opened;

public:

  Connection(const int socket=CONNECTION_NO_SOCKET, const int id=DEFAULT_CONNECTION_ID) {
    this->socket = socket;
    this->id = id;
    this->opened = false;
  }

  virtual ~Connection() {
    
  }

  int getSocket() const {
    return this->socket;
  }

  int getID() const {
    return this->id;
  }

  virtual bool end() = 0;

  bool isOpened() const {
    return this->opened;
  }

  virtual int readData(char* output, const int length) = 0;
  virtual int writeData(const char* input, const int length) = 0;

  virtual Connection& operator>>(std::string& output) = 0;
  virtual Connection& operator>>(Message& output) = 0;
  virtual Connection& operator<<(Message input) = 0;
  virtual Connection& operator>>(char* output) = 0;
  virtual Connection& operator<<(const char* input) = 0;
  
  virtual Connection& send(std::vector<int> input) = 0;
};


#endif /* __CONNECTION_H__ */
