#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#define DEFAULT_CONNECTION_ID  -1
#define CONNECTION_NO_SOCKET   -1

#include <telnetish/message.h>
#include <telnetish/data-source.h>
#include <vector>
#include <string>

class Connection : public DataSource {
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
  
};


#endif /* __CONNECTION_H__ */
