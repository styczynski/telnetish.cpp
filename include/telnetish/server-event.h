#ifndef __TCP_SERVER_EVENT_H__
#define __TCP_SERVER_EVENT_H__

#include <telnetish/server.h>
#include <telnetish/connection.h>
#include <functional>

template< typename DataT >
class ServerEvent {
private:
  Server<DataT>* source;
  Connection* connection;
  DataT* payload;

public:

  ServerEvent(Server<DataT>* serverSource, Connection* connection, DataT* payload = nullptr) {
    this->source = serverSource;
    this->connection = connection;
    this->payload = payload;
  }

  DataT* getPayload() {
    return this->payload;
  }

  void setPayload(DataT* payload = nullptr) {
    this->payload = payload;
  }

  Server<ServerEvent<DataT>>& getSource() {
    return *(this->source);
  }

  Connection& getConnection() {
    return *(this->connection);
  }

  Connection* getConnectionSource() {
    return this->connection;
  }

  virtual std::string getType() {
    return "UNKNOWN";
  }
};

#endif /* __TCP_SERVER_EVENT_H__ */
