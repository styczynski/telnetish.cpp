#ifndef __TCP_SERVER_EVENT_H__
#define __TCP_SERVER_EVENT_H__

#include <telnetish/tcp-server.h>
#include <telnetish/tcp-connection.h>
#include <functional>

class TCPServerEvent {
private:
  TCPServer* source;
  TCPConnection* connection;

public:
  
  TCPServerEvent(TCPServer* serverSource, TCPConnection* connection);
  TCPServer& getSource();
  TCPConnection* getServerConnection();
  virtual std::string getType() {
    return "UNKNOWN";
  }
};


#endif /* __TCP_SERVER_EVENT_H__ */