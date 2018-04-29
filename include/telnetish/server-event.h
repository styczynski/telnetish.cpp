/** @file
*
*  C++ server event abstraction
*
*  @author Piotr Styczyński <piotrsty1@gmail.com>
*  @copyright MIT
*  @date 2018-04-29
*/
#ifndef __SERVER_EVENT_H__
#define __SERVER_EVENT_H__

#include <telnetish/server.h>
#include <telnetish/connection.h>
#include <functional>

/**
 * Class representing server event with data attached to each event
 * of type @p DataT.
 */
template< typename DataT >
class ServerEvent {
private:
  Server<DataT>* source;
  Connection* connection;
  DataT payload;

public:

  /**
   * Create new event for given server instance, connection and payload.
   */
  ServerEvent(Server<DataT>* serverSource, Connection* connection, DataT payload) {
    this->source = serverSource;
    this->connection = connection;
    this->payload = payload;
  }

  /**
   * Obtain payload attached to this instance of server event. 
   *
   * @return Payload attached to the event
   */
  DataT getPayload() {
    return this->payload;
  }

  /**
   * Set payload attached to this instance of server event. 
   *
   * @param[in] payload : new payload object
   */
  void setPayload(DataT payload) {
    this->payload = payload;
  }

  /**
   * Obtain server source of the event.
   *
   * @return refrence to the server that generated this event
   */
  Server<ServerEvent<DataT>>& getSource() {
    return *(this->source);
  }

  /**
   * Obtain connection attached to this event.
   *
   * @return Connection attached to this server event.
   */
  Connection& getConnection() {
    return *(this->connection);
  }

  /**
   * Obtain the connection pointer attached to this event.
   *
   * @return Connection attached to this server event.
   */
  Connection* getConnectionSource() {
    return this->connection;
  }

  /**
   * Get string descriptions of the event type.
   *
   * @return type of the event
   */
  virtual std::string getType() {
    return "UNKNOWN";
  }
};

#endif /* __SERVER_EVENT_H__ */
